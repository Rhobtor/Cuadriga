from setuptools import find_packages
from setuptools import setup

setup(
    name='ctl_mission',
    version='0.20.3',
    packages=find_packages(
        include=('ctl_mission', 'ctl_mission.*')),
)
