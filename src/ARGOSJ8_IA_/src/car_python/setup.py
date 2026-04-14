import os
from glob import glob
import sys
from distutils.cmd import Command

package_name = 'car'

# NOTE:
# colcon extracts package metadata by executing this file through distutils' `run_setup()`.
# In that mode, a `setup` callable is injected in globals; importing `setup` from setuptools
# would overwrite it and make `run_setup()` think setup() was never called.
if 'setup' not in globals():
    from setuptools import setup


class _ColconDevelopShim(Command):
    """Shim for colcon's legacy distutils `develop` extensions.

    Modern setuptools' `develop` command doesn't support colcon flags like
    `--editable`, `--uninstall`, `--build-directory`, nor `setup.cfg`'s
    `[develop] script_dir` option used by ament_python.

    This shim accepts those options and performs a standard `install` into
    the prefix set by colcon's `prefix_override/sitecustomize.py`.
    """

    description = 'colcon develop shim (delegates to install)'

    user_options = [
        ('uninstall', None, 'ignored (colcon legacy option)'),
        ('editable', None, 'ignored (colcon legacy option)'),
        ('no-deps', None, 'ignored (colcon legacy option)'),
        ('build-directory=', None, 'ignored (colcon legacy option)'),
        ('script-dir=', None, 'ignored (ament_python option)'),
    ]

    boolean_options = ['uninstall', 'editable', 'no-deps']

    def initialize_options(self):
        self.uninstall = False
        self.editable = False
        self.no_deps = False
        self.build_directory = None
        self.script_dir = None

    def finalize_options(self):
        pass

    def run(self):
        self.run_command('install')


class _SymlinkDataShim(Command):
    """Fallback for colcon's `symlink_data` command.

    When present in the invocation, copy data_files to the install prefix.
    """

    description = 'colcon symlink_data shim (copies data_files)'
    user_options = [
        ('force', None, 'ignored (colcon legacy option)'),
    ]

    boolean_options = ['force']

    def initialize_options(self):
        self.force = False

    def finalize_options(self):
        pass

    def run(self):
        # `install_data` will honor data_files destinations.
        self.run_command('install_data')

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
        (os.path.join('share', package_name, 'urdf'), glob('urdf/*')),
        #(os.path.join('share', package_name, 'models'), glob('models/*')),
        (os.path.join('share', package_name, 'worlds'), glob('worlds/*')),
        (os.path.join('share', package_name, 'meshes'), glob('meshes/*')),
        (os.path.join('share', package_name, 'config'), glob('config/*')),
        (os.path.join('lib', package_name), glob('lib/' + package_name + '/*'))
    ],
    install_requires=['setuptools', 'pyproj'],
    zip_safe=True,
    maintainer='author',
    maintainer_email='todo@todo.com',
    description='The ' + package_name + ' package',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            "keyboard_control=car.keyboard_control:main",
            "joystick_control=car.joystick_control:main",
            "cmd_vel_to_throttle=car.cmd_vel_to_throttle:main",
            "odom_to_tf=car.odom_to_tf:main",
            'filter_points_cloud = car.filter_points_cloud:main',
            'points_goal = car.points_goal:main',
            'colision_zone = car.colision_zone:main',
            'supervisor_node = car.supervisor:main',
            'frontier_centroid = car.frontier_centroid:main',
            'map_navegation= car.map_navegation:main',
            'memory_map= car.memory_map:main',
            'path_ppo_cnn_lstm_training_init_mode= car.path_ppo_cnn_lstm_training_init_mode:main',
            'movement_predictive= car.movement_predictive:main',
            'lidar_detection= car.lidar_obstacle_detection:main',
            'path_ppo_cnn_lstm_training_init_mode_simple= car.path_ppo_cnn_lstm_training_init_mode_simple:main',
            'path_ppo_cnn_lstm_full_model= car.path_ppo_cnn_lstm_full_model:main',
            'inference_training_mode_simple= car.inference_training_mode_simple:main',
            'inference_full_model= car.inference_full_model:main',
            'path_ppo_cnn_lstm_training_init_mode_j8= car.path_ppo_cnn_lstm_training_init_mode_j8:main',
            'test_vel = car.test_vel:main',
            'map_navegation_test= car.map_test_navigation_hgh:main',
            'points_goal_old= car.points_goal_old:main',
            'test_octomap= car.test_octomap:main',
            'j8_test_ppo= car.j8_test_ppo:main',
            'inference_j8= car.inference_j8:main',
            'j8_bueno= car.j8_bueno:main',
            'j8_policy= car.j8_policy:main',
            'test_j8_old= car.test_j8_old:main',
            'inference_test_j8= car.inference_test_j8:main',
            'scape_test= car.scape_test:main',
            'prior_map_builder= car.prior_map_builder:main',
            'fusion= car.fusion:main',
            'voxel_unity= car.voxel_unity:main',
            'unity_goal= car.unity_goal:main',
            'circuit_supervisor= car.supervisor_circuit:main',
            'circuit1= car.circuit1:main',

            'fixposition_map_anchor=car.fixposition_map_anchor:main',

            'fp_poi_map_odom_static_test=car.fp_poi_map_odom_static_test:main',

            'fp_poi_map_static_from_start=car.fp_poi_map_static_from_start:main',

            'zed_static_tf_tree=car.zed_static_tf_tree:main',

            'latlon_posearray_to_local=car.latlon_posearray_to_local:main',

            'goal_local_marker=car.goal_local_marker:main',

            'fixposition_odom_to_base_link_tf=car.fixposition_odom_to_base_link_tf:main',

            'fixposition_map_odom_static_from_odom=car.fixposition_map_odom_static_from_odom:main',

            'fixposition_local_tf_from_odom=car.fixposition_local_tf_from_odom:main',

            'fixposition_reframe=car.fixposition_reframe:main',

            'fixposition_tf_tree_for_zed=car.fixposition_tf_tree_for_zed:main',

            'latlon_posearray_to_local_utm=car.latlon_posearray_to_local_utm:main',

            'latlon_circuit=car.latlon_circuit:main',

            'navsatfix_to_ecef=car.navsatfix_to_ecef:main',

            'navsatfix_goal_to_map_pose=car.navsatfix_goal_to_map_pose:main',

            'gnss_goal_circuit=car.gnss_goal_circuit:main',

            'check_goal_gnss=car.check_goal_gnss:main',

        ],
    },
    cmdclass={
        'develop': _ColconDevelopShim,
        'symlink_data': _SymlinkDataShim,
    },
)
