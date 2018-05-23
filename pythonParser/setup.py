import json
from setuptools import setup, find_packages


install_requires = []
tests_require = []

with open('Pipfile.lock') as fd:
    lock_data = json.load(fd)
    install_requires = [
        package_name + package_data['version']
        for package_name, package_data in lock_data['default'].items()
    ]
    tests_require = [
        package_name + package_data['version']
        for package_name, package_data in lock_data['develop'].items()
    ]
setup(
    name='pythonParser',
    entry_points = {
        "console_scripts": ['pythonParser = pythonParser.pythonParser:main']
    },
    version="0.0.1",
    packages=find_packages(),
    install_requires=install_requires,
)
