import threading
import time
from typing import Dict, Optional, Tuple

import cv2
import numpy as np
import rclpy
import uvicorn
from cv_bridge import CvBridge, CvBridgeError
from fastapi import FastAPI
from fastapi.responses import JSONResponse, PlainTextResponse, Response, StreamingResponse
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, HistoryPolicy, QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import CompressedImage, Image


class LatestFrameStore:
    def __init__(self, name: str):
        self.name = name
        self._lock = threading.Lock()
        self._condition = threading.Condition(self._lock)
        self._jpeg_bytes = None
        self._sequence = 0
        self._stamp_ns = None
        self._shape = None

    def update(self, jpeg_bytes: bytes, stamp_ns: int, shape: Tuple[int, ...]) -> None:
        with self._condition:
            self._jpeg_bytes = jpeg_bytes
            self._stamp_ns = int(stamp_ns)
            self._shape = tuple(shape)
            self._sequence += 1
            self._condition.notify_all()

    def snapshot(self) -> Tuple[Optional[bytes], int, Optional[int], Optional[Tuple[int, ...]]]:
        with self._lock:
            payload = self._jpeg_bytes
            if payload is None:
                return None, self._sequence, self._stamp_ns, self._shape
            return bytes(payload), self._sequence, self._stamp_ns, self._shape

    def wait_for_update(self, last_sequence: int, timeout_sec: float) -> Tuple[Optional[bytes], int, Optional[int], Optional[Tuple[int, ...]]]:
        with self._condition:
            if self._jpeg_bytes is None or self._sequence == last_sequence:
                self._condition.wait(timeout=timeout_sec)

            payload = self._jpeg_bytes
            if payload is None:
                return None, self._sequence, self._stamp_ns, self._shape
            return bytes(payload), self._sequence, self._stamp_ns, self._shape

    def has_frame(self) -> bool:
        with self._lock:
            return self._jpeg_bytes is not None


def _stamp_to_ns(msg) -> int:
    return int(msg.header.stamp.sec) * 1000000000 + int(msg.header.stamp.nanosec)


def _age_ms_from_stamp(stamp_ns: Optional[int]) -> Optional[int]:
    if stamp_ns is None:
        return None
    now_ns = time.time_ns()
    age_ns = now_ns - int(stamp_ns)
    if age_ns < 0:
        return 0
    return int(age_ns / 1000000)


class ImageRelayServerNode(Node):
    def __init__(self) -> None:
        super().__init__('image_relay_server')

        self.declare_parameter('thermal_topic', '/fanet/raw/thermal')
        self.declare_parameter('thermal_overlay_topic', '/fanet/raw/rgb')
        self.declare_parameter('rgb_overlay_topic', '/fanet/segmentation/overlay')
        self.declare_parameter('rgb_original_topic', '/zed/zed_node/left/image_rect_color')
        self.declare_parameter('host', '0.0.0.0')
        self.declare_parameter('port', 8080)
        self.declare_parameter('jpeg_quality', 80)
        self.declare_parameter('resize_width', 640)
        self.declare_parameter('enable_resize', False)
        self.declare_parameter('log_fps', False)

        self.stream_topics = {
            'thermal': str(self.get_parameter('thermal_topic').value),
            'thermal_overlay': str(self.get_parameter('thermal_overlay_topic').value),
            'rgb_overlay': str(self.get_parameter('rgb_overlay_topic').value),
            'rgb_original': str(self.get_parameter('rgb_original_topic').value),
        }
        self.host = str(self.get_parameter('host').value)
        self.port = int(self.get_parameter('port').value)
        self.jpeg_quality = max(10, min(100, int(self.get_parameter('jpeg_quality').value)))
        self.resize_width = max(1, int(self.get_parameter('resize_width').value))
        self.enable_resize = bool(self.get_parameter('enable_resize').value)
        self.log_fps = bool(self.get_parameter('log_fps').value)

        self.bridge = CvBridge()
        self.shutdown_event = threading.Event()
        self.frame_stores = dict((name, LatestFrameStore(name)) for name in self.stream_topics)
        self.frame_counts = dict((name, 0) for name in self.stream_topics)
        self._fps_last_time = time.monotonic()
        self._fps_last_counts = dict((name, 0) for name in self.stream_topics)
        self._uvicorn_server = None
        self._uvicorn_thread = None

        qos = QoSProfile(
            history=HistoryPolicy.KEEP_LAST,
            depth=1,
            reliability=ReliabilityPolicy.BEST_EFFORT,
            durability=DurabilityPolicy.VOLATILE,
        )

        self._subscriptions = []
        for stream_name, topic_name in self.stream_topics.items():
            message_type = CompressedImage if topic_name.endswith('/compressed') else Image
            subscription = self.create_subscription(message_type, topic_name, self._make_image_callback(stream_name), qos)
            self._subscriptions.append(subscription)
            self.get_logger().info('relay %s <- %s [%s]' % (stream_name, topic_name, message_type.__name__))

        if self.log_fps:
            self.create_timer(5.0, self._log_fps)

        self.app = self._create_app()

    def _make_image_callback(self, stream_name: str):
        def _callback(msg: Image) -> None:
            self._on_image(stream_name, msg)

        return _callback

    def _on_image(self, stream_name: str, msg) -> None:
        try:
            frame = self._msg_to_cv_image(msg)
            frame = self._prepare_frame(frame)
            jpeg_bytes = self._encode_jpeg(frame)
            if jpeg_bytes is None:
                return
            self.frame_stores[stream_name].update(jpeg_bytes, _stamp_to_ns(msg), frame.shape)
            self.frame_counts[stream_name] += 1
        except CvBridgeError as exc:
            self.get_logger().warning('cv_bridge failed for %s: %s' % (stream_name, exc))
        except Exception as exc:
            self.get_logger().warning('image relay failed for %s: %s' % (stream_name, exc))

    def _msg_to_cv_image(self, msg) -> np.ndarray:
        if isinstance(msg, CompressedImage):
            encoded = np.frombuffer(msg.data, dtype=np.uint8)
            frame = cv2.imdecode(encoded, cv2.IMREAD_UNCHANGED)
            if frame is None:
                raise ValueError('failed to decode compressed image')
            return frame

        encoding = str(msg.encoding or '').lower()

        if encoding in ('rgb8', 'rgba8'):
            return self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        if encoding in ('bgr8', 'bgra8'):
            return self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        if encoding in ('mono8', '8uc1'):
            return self.bridge.imgmsg_to_cv2(msg, desired_encoding='mono8')

        return self.bridge.imgmsg_to_cv2(msg, desired_encoding='passthrough')

    def _prepare_frame(self, frame: np.ndarray) -> np.ndarray:
        if frame.dtype != np.uint8:
            frame = cv2.normalize(frame, None, 0, 255, cv2.NORM_MINMAX)
            frame = frame.astype(np.uint8)

        if self.enable_resize and frame.shape[1] != self.resize_width:
            scale = float(self.resize_width) / float(frame.shape[1])
            resized_height = max(1, int(round(frame.shape[0] * scale)))
            interpolation = cv2.INTER_AREA if scale < 1.0 else cv2.INTER_LINEAR
            frame = cv2.resize(frame, (self.resize_width, resized_height), interpolation=interpolation)

        return np.ascontiguousarray(frame)

    def _encode_jpeg(self, frame: np.ndarray) -> Optional[bytes]:
        encode_params = [int(cv2.IMWRITE_JPEG_QUALITY), int(self.jpeg_quality)]
        success, encoded = cv2.imencode('.jpg', frame, encode_params)
        if not success:
            return None
        return encoded.tobytes()

    def _create_app(self) -> FastAPI:
        app = FastAPI(title='image_relay_server')
        node = self

        @app.get('/health')
        def health():
            streams = {}
            for stream_name, topic_name in node.stream_topics.items():
                payload, sequence, stamp_ns, shape = node.frame_stores[stream_name].snapshot()
                streams[stream_name] = {
                    'topic': topic_name,
                    'available': payload is not None,
                    'sequence': sequence,
                    'stamp_ns': stamp_ns,
                    'age_ms': _age_ms_from_stamp(stamp_ns),
                    'shape': shape,
                }
            return JSONResponse({'status': 'ok', 'streams': streams})

        @app.get('/snapshot/{stream_name}')
        def snapshot(stream_name: str):
            if stream_name not in node.frame_stores:
                return PlainTextResponse('unknown stream', status_code=404)
            payload, sequence, stamp_ns, shape = node.frame_stores[stream_name].snapshot()
            if payload is None:
                return PlainTextResponse('no frame available', status_code=503)
            headers = {
                'Cache-Control': 'no-store',
                'X-Relay-Sequence': str(sequence),
            }
            if stamp_ns is not None:
                headers['X-Relay-Stamp-Ns'] = str(stamp_ns)
            age_ms = _age_ms_from_stamp(stamp_ns)
            if age_ms is not None:
                headers['X-Relay-Age-Ms'] = str(age_ms)
            if shape is not None:
                headers['X-Relay-Shape'] = 'x'.join(str(value) for value in shape)
            return Response(content=payload, media_type='image/jpeg', headers=headers)

        @app.get('/stream/{stream_name}')
        def stream(stream_name: str):
            if stream_name not in node.frame_stores:
                return PlainTextResponse('unknown stream', status_code=404)
            if not node.frame_stores[stream_name].has_frame():
                return PlainTextResponse('no frame available', status_code=503)
            return StreamingResponse(
                node._mjpeg_generator(stream_name),
                media_type='multipart/x-mixed-replace; boundary=frame',
                headers={'Cache-Control': 'no-store'},
            )

        return app

    def _mjpeg_generator(self, stream_name: str):
        store = self.frame_stores[stream_name]
        last_sequence = -1

        while not self.shutdown_event.is_set():
            payload, sequence, _stamp_ns, _shape = store.wait_for_update(last_sequence, timeout_sec=1.0)
            if payload is None or sequence == last_sequence:
                continue

            header = (
                b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n'
                b'Content-Length: ' + str(len(payload)).encode('ascii') + b'\r\n\r\n'
            )
            yield header + payload + b'\r\n'
            last_sequence = sequence

    def start_http_server(self) -> None:
        if self._uvicorn_server is not None:
            return

        config = uvicorn.Config(
            self.app,
            host=self.host,
            port=self.port,
            log_level='info',
            access_log=False,
        )
        self._uvicorn_server = uvicorn.Server(config)
        self._uvicorn_thread = threading.Thread(target=self._uvicorn_server.run, daemon=True)
        self._uvicorn_thread.start()
        self.get_logger().info('HTTP relay listening on http://%s:%d' % (self.host, self.port))

    def stop_http_server(self) -> None:
        if self._uvicorn_server is None:
            return

        self._uvicorn_server.should_exit = True
        if self._uvicorn_thread is not None:
            self._uvicorn_thread.join(timeout=5.0)
        self._uvicorn_server = None
        self._uvicorn_thread = None

    def _log_fps(self) -> None:
        now = time.monotonic()
        elapsed = max(now - self._fps_last_time, 1e-6)
        parts = []
        for stream_name in sorted(self.frame_counts):
            count = self.frame_counts[stream_name]
            previous = self._fps_last_counts[stream_name]
            fps = float(count - previous) / elapsed
            self._fps_last_counts[stream_name] = count
            parts.append('%s=%.1f fps' % (stream_name, fps))
        self._fps_last_time = now
        self.get_logger().info('relay fps %s' % ', '.join(parts))

    def destroy_node(self) -> bool:
        self.shutdown_event.set()
        self.stop_http_server()
        try:
            return super().destroy_node()
        except ValueError:
            return False


def main(args=None) -> None:
    rclpy.init(args=args)
    node = ImageRelayServerNode()
    executor = MultiThreadedExecutor(num_threads=4)
    executor.add_node(node)
    node.start_http_server()

    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.shutdown_event.set()
        executor.shutdown()
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()