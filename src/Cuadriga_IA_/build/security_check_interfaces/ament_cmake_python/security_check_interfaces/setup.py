from setuptools import find_packages
from setuptools import setup

setup(
    name='security_check_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('security_check_interfaces', 'security_check_interfaces.*')),
)
