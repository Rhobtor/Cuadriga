from setuptools import find_packages
from setuptools import setup

setup(
    name='path_manager_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('path_manager_interfaces', 'path_manager_interfaces.*')),
)
