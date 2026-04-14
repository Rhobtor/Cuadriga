from setuptools import find_packages, setup

package_name = 'j8_gui'

setup(
        name=package_name,
        version='0.1.0',
        packages=find_packages(),
        data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/resources', [
        'j8_gui/resources/map.html',
        ]),
            ('share/ament_index/resource_index/packages', ['resource/j8_gui']),
            ('share/j8_gui', ['package.xml']),
            ('share/j8_gui/resources', ['j8_gui/resources/map.html']),
            ('share/j8_gui/resources/vendor/leaflet', [
                'j8_gui/resources/vendor/leaflet/leaflet.js',
                'j8_gui/resources/vendor/leaflet/leaflet.css',
            ]),
            ('share/j8_gui/resources/vendor/leaflet/images', [
                'j8_gui/resources/vendor/leaflet/images/marker-icon.png',
                'j8_gui/resources/vendor/leaflet/images/marker-icon-2x.png',
                'j8_gui/resources/vendor/leaflet/images/marker-shadow.png',
            ]),
        
        # Si incluyes Leaflet en repo, añade:
        # ('share/' + package_name + '/resources/vendor/leaflet', [
        # 'j8_gui/resources/vendor/leaflet/leaflet.js',
        # 'j8_gui/resources/vendor/leaflet/leaflet.css',
        # ]),
        ],
        install_requires=['setuptools'],
        zip_safe=True,
        maintainer='you',
        maintainer_email='you@example.com',
        description='GUI ROS2 con Leaflet + FollowZED',
        license='MIT',
        tests_require=['pytest'],
        entry_points={
        'console_scripts': [
        'gui = j8_gui.gui_node:main',
        ],
    },
)