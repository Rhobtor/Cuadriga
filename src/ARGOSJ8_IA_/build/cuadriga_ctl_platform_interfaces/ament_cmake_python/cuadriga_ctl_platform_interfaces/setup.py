from setuptools import find_packages
from setuptools import setup

setup(
    name='cuadriga_ctl_platform_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('cuadriga_ctl_platform_interfaces', 'cuadriga_ctl_platform_interfaces.*')),
)
