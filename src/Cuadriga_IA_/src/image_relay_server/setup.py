from setuptools import find_packages, setup


package_name = 'image_relay_server'


setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=[
        'setuptools',
        'fastapi>=0.95',
        'uvicorn>=0.22',
        'numpy>=1.19',
    ],
    zip_safe=True,
    maintainer='rhobtor',
    maintainer_email='luismidp7@gmail.com',
    description='ROS 2 image relay server with FastAPI snapshots and MJPEG streaming',
    license='MIT',
    entry_points={
        'console_scripts': [
            'relay_server = image_relay_server.relay_server:main',
        ],
    },
)