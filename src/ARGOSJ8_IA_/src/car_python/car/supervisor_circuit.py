#!/usr/bin/env python3
import os, signal, subprocess, time, psutil
import rclpy
from rclpy.node  import Node
from std_msgs.msg import Bool, UInt32
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy

KILL_TIMEOUT   = 4.0   # s antes de escalar la señal
RELAUNCH_DELAY = 1.0   # s para dar respiro a DDS

class CircuitSupervisor(Node):
    def __init__(self):
        super().__init__('circuit_supervisor')

        # ---------- Parámetros ----------
        self.declare_parameter('launch_cmds', [
            'ros2 launch car circuit1.launch.py',
            #'ros2 launch car circuit3.launch.py',
            # 'ros2 launch car world_2_c.launch.py',
            # 'ros2 launch car world_3_d.launch.py',
        ])
        self.declare_parameter('finish_after_points', 0)   # 0 = desactivado
        self.declare_parameter('switch_on_done_topic', True)

        self.launch_cmds         = list(self.get_parameter('launch_cmds').value)
        self.finish_after_points = int(self.get_parameter('finish_after_points').value)
        self.switch_on_done      = bool(self.get_parameter('switch_on_done_topic').value)

        if not self.launch_cmds:
            raise RuntimeError('launch_cmds vacío. Define al menos un comando ros2 launch.')

        self.current_index = 0
        self.visited_count = 0
        self.env_process   = None
        self.switching     = False
        self.ignore_done_until = 0.0

        # ---------- QoS ----------
        qos_tl = QoSProfile(depth=1,
                            reliability=ReliabilityPolicy.RELIABLE,
                            durability=DurabilityPolicy.TRANSIENT_LOCAL)
        qos_done = QoSProfile(depth=1,
                      reliability=ReliabilityPolicy.RELIABLE,
                      durability=DurabilityPolicy.VOLATILE)
        qos_r  = QoSProfile(depth=1, reliability=ReliabilityPolicy.RELIABLE)
        qos_world_reset = QoSProfile(depth=1,
                         reliability=ReliabilityPolicy.RELIABLE,
                         durability=DurabilityPolicy.VOLATILE)

        # ---------- Subs/Pubs ----------
        self.create_subscription(Bool, '/reset_request', self.reset_request_cb, qos_tl)
        self.create_subscription(Bool, '/circuit/done', self.circuit_done_cb, qos_done)
        self.create_subscription(UInt32, '/circuit/visited', self.visited_cb, qos_r)
        self.create_subscription(Bool, '/world_reset_ok', self.world_reset_ok_cb, qos_world_reset)

        self.reset_conf_pub = self.create_publisher(Bool, '/reset_confirmation', qos_tl)
        self.goal_reached_pub = self.create_publisher(Bool, 'goal_reached', 10)

        # ---------- Arranque ----------
        self.launch_environment()

    # ------------------------------ gestión de procesos
    def _kill_tree(self, pid):
        try:
            parent = psutil.Process(pid)
        except psutil.NoSuchProcess:
            return

        children = parent.children(recursive=True)
        self.get_logger().info(f"⏹  Cerrando {len(children)+1} procesos…")

        for p in children:  p.send_signal(signal.SIGINT)
        parent.send_signal(signal.SIGINT)
        gone, alive = psutil.wait_procs([parent]+children, timeout=KILL_TIMEOUT)

        for p in alive:  p.send_signal(signal.SIGTERM)
        gone, alive = psutil.wait_procs(alive, timeout=KILL_TIMEOUT/2)

        for p in alive:
            self.get_logger().warning(f"💀  Forzando kill al PID {p.pid}")
            p.kill()

    def kill_environment(self):
        if self.env_process and self.env_process.poll() is None:
            self._kill_tree(self.env_process.pid)
            self.env_process.wait()
            self.get_logger().info("Entorno terminado.")
        self.env_process = None

    def launch_environment(self):
        cmd = self.launch_cmds[self.current_index].split()
        self.get_logger().info(f"▶️  Lanzando: {' '.join(cmd)}")
        self.env_process = subprocess.Popen(cmd, start_new_session=True)
        self.get_logger().info(f"PID raíz: {self.env_process.pid}")
        self.visited_count = 0  # reinicia contador al cambiar de circuito

    # ------------------------------ callbacks
    def visited_cb(self, msg: UInt32):
        self.visited_count = int(msg.data)
        if (not self.switching) and self.finish_after_points > 0 and self.visited_count >= self.finish_after_points:
            self.get_logger().info(f"🎯 Alcanzados {self.visited_count} puntos (umbral={self.finish_after_points}). Cambiando de circuito…")
            self.switch_environment()

    def circuit_done_cb(self, msg: Bool):
        # Evita re-disparos por mensajes encolados (o durabilidad) justo tras un cambio
        if time.monotonic() < self.ignore_done_until:
            return
        if self.switch_on_done and msg.data and not self.switching:
            self.get_logger().info("🏁 Señal de fin de circuito recibida. Cambiando de circuito…")
            self.switch_environment()

    def reset_request_cb(self, msg: Bool):
        if msg.data and not self.switching:
            self.get_logger().info("🔄 Reset manual solicitado. Cambiando de circuito…")
            self.switch_environment()

    def world_reset_ok_cb(self, msg: Bool):
        # Cuando el mundo se resetea, reiniciamos al primer circuito (launch_cmds[0])
        if msg.data and not self.switching:
            self.get_logger().info("🌍 world_reset_ok=True. Reiniciando al primer circuito…")
            self.restart_to_first()

    # ------------------------------ cambio de entorno
    def switch_environment(self):
        if self.switching:
            return
        self.switching = True

        # Notifica goal_reached por compatibilidad (si algún nodo lo espera)
        self.goal_reached_pub.publish(Bool(data=True))

        self.kill_environment()
        self.current_index = (self.current_index + 1) % len(self.launch_cmds)
        time.sleep(RELAUNCH_DELAY)
        self.launch_environment()

        self.reset_conf_pub.publish(Bool(data=True))
        self.get_logger().info("✅ Cambio completado")
        # Ventana corta para descartar un /circuit/done anterior que llegue tarde
        self.ignore_done_until = time.monotonic() + 0.75
        self.switching = False

    def restart_to_first(self):
        if self.switching:
            return
        self.switching = True

        self.kill_environment()
        self.current_index = 0
        time.sleep(RELAUNCH_DELAY)
        self.launch_environment()

        self.reset_conf_pub.publish(Bool(data=True))
        self.get_logger().info("✅ Reinicio completado (vuelta a launch_cmds[0])")
        self.ignore_done_until = time.monotonic() + 0.75
        self.switching = False

    # ------------------------------ shutdown
    def destroy_node(self):
        self.kill_environment()
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    node = CircuitSupervisor()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    try:
        rclpy.shutdown()
    except Exception:
        pass

if __name__ == '__main__':
    main()
