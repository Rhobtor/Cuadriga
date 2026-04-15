from setuptools import find_packages
from setuptools import setup

setup(
    name='cuadriga_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('cuadriga_interfaces', 'cuadriga_interfaces.*')),
)
