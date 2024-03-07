from setuptools import find_packages
from setuptools import setup

setup(
    name='my_msgss',
    version='0.0.0',
    packages=find_packages(
        include=('my_msgss', 'my_msgss.*')),
)
