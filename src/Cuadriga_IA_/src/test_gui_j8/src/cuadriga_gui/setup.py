from setuptools import find_packages, setup

package_name = 'cuadriga_gui'

setup(
        name=package_name,
        version='0.1.0',
        packages=find_packages(),
        data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/resources', [
        'cuadriga_gui/resources/map.html',
        ]),
            ('share/ament_index/resource_index/packages', ['resource/cuadriga_gui']),
            ('share/cuadriga_gui', ['package.xml']),
            ('share/cuadriga_gui/resources', ['cuadriga_gui/resources/map.html']),
            ('share/cuadriga_gui/resources/vendor/leaflet', [
                'cuadriga_gui/resources/vendor/leaflet/leaflet.js',
                'cuadriga_gui/resources/vendor/leaflet/leaflet.css',
            ]),
            ('share/cuadriga_gui/resources/vendor/leaflet/images', [
                'cuadriga_gui/resources/vendor/leaflet/images/marker-icon.png',
                'cuadriga_gui/resources/vendor/leaflet/images/marker-icon-2x.png',
                'cuadriga_gui/resources/vendor/leaflet/images/marker-shadow.png',
            ]),
        
        # Si incluyes Leaflet en repo, añade:
        # ('share/' + package_name + '/resources/vendor/leaflet', [
        # 'cuadriga_gui/resources/vendor/leaflet/leaflet.js',
        # 'cuadriga_gui/resources/vendor/leaflet/leaflet.css',
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
        'gui = cuadriga_gui.gui_node:main',
        ],
    },
)