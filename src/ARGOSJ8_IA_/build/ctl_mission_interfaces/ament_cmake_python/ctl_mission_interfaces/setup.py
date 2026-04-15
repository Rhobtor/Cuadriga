from setuptools import find_packages
from setuptools import setup

setup(
    name='ctl_mission_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('ctl_mission_interfaces', 'ctl_mission_interfaces.*')),
)
