from setuptools import setup
from setuptools import find_packages

package_name = 'GUI_pkg'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(),
    install_requires=['setuptools', 'rclpy', 'PyQt5'],
    zip_safe=True,
    maintainer='Your Name',
    maintainer_email='your.email@example.com',
    description='GUI Application with ROS2 Integration',
    license='License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'gui_node = GUI_pkg.mainwindow:main',
        ],
    },
)

