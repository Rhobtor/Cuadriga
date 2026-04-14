#!/usr/bin/env python3
import os, signal, subprocess, time, psutil
import rclpy
from rclpy.node  import Node
from std_msgs.msg import Bool
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy

KILL_TIMEOUT = 4.0      # s antes de escalar la señal
RELAUNCH_DELAY = 1.0    # s para dar respiro a DDS

class SupervisorNode(Node):
    def __init__(self):
        super().__init__('supervisor_node')

        self.declare_parameter(
            'launch_cmds',
            [
                'ros2 launch car world_1_b.launch.py',

                
            ])
        self.launch_cmds   = self.get_parameter('launch_cmds').value
        self.current_index = 0
        self.env_process   = None
        self.reset_in_prog = False

        qos = QoSProfile(depth=1,
                         reliability=ReliabilityPolicy.RELIABLE,
                         durability=DurabilityPolicy.TRANSIENT_LOCAL)

        self.create_subscription(Bool, '/reset_request',
                                 self.reset_request_cb, qos)
        self.reset_conf_pub = self.create_publisher(Bool,
                                                    '/reset_confirmation', qos)
        self.goal_reached_pub = self.create_publisher(Bool,
                                                      'goal_reached', 10)

        self.launch_environment()

    # ------------------------------------------------------------ helpers
    def _kill_tree(self, pid):
        """Mata recursivamente todos los descendientes de `pid`."""
        try:
            parent = psutil.Process(pid)
        except psutil.NoSuchProcess:
            return

        children = parent.children(recursive=True)
        self.get_logger().info(f"⏹  Cerrando {len(children)+1} procesos…")

        for p in children:  p.send_signal(signal.SIGINT)
        parent.send_signal(signal.SIGINT)
        gone, alive = psutil.wait_procs([parent]+children, timeout=KILL_TIMEOUT)

        # Escalamos a SIGTERM
        for p in alive:  p.send_signal(signal.SIGTERM)
        gone, alive = psutil.wait_procs(alive, timeout=KILL_TIMEOUT/2)

        # Último recurso: SIGKILL
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
        # start_new_session=True crea un nuevo grupo de procesos (PGID = PID)
        self.env_process = subprocess.Popen(cmd, start_new_session=True)
        self.get_logger().info(f"PID raíz: {self.env_process.pid}")

    # ------------------------------------------------------------ callbacks
    def reset_request_cb(self, msg: Bool):
        if not msg.data or self.reset_in_prog or (self.env_process and self.env_process.poll() is None and self.env_process.pid == 0):
            return

        self.reset_in_prog = True
        self.get_logger().info("🔄  Reset solicitado")

        # Notifica goal_reached por compatibilidad
        self.goal_reached_pub.publish(Bool(data=True))

        # 1. Mata entorno actual
        self.kill_environment()

        # 2. Selecciona siguiente comando
        self.current_index = (self.current_index + 1) % len(self.launch_cmds)
        time.sleep(RELAUNCH_DELAY)       # respiro a DDS/Gazebo

        # 3. Lanza nuevo entorno
        self.launch_environment()

        # 4. Publica confirmación
        self.reset_conf_pub.publish(Bool(data=True))
        self.get_logger().info("✅  Reset completado")
        self.reset_in_prog = False

    # ------------------------------------------------------------ shutdown
    def destroy_node(self):
        self.kill_environment()
        super().destroy_node()

# --------------------------------------------------------------------------
def main(args=None):
    rclpy.init(args=args)
    node = SupervisorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
