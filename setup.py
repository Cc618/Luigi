# !!! Use this script only to install Luigi, to test it, use CMake
# * pybind11 must be installed

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
import pybind11
import glob


LUIGI_VERSION_MAJOR = 0
LUIGI_VERSION_MINOR = 1


def get_src():
    return [file for file in glob.glob('src/*.cpp')]


def mk_conf():
    print('Generating src/config.h')

    with open('src/config.h.in') as f:
        conf = f.read()


    with open('src/config.h', 'w') as f:
        conf = conf.replace('@luigi_VERSION_MAJOR@', str(LUIGI_VERSION_MAJOR))
        conf = conf.replace('@luigi_VERSION_MINOR@', str(LUIGI_VERSION_MINOR))

        f.write(conf)

    print('Generated src/config.h')


mk_conf()


ext_modules = [
    Extension(
        'luigi',
        sorted(get_src()),
        include_dirs=[
            pybind11.get_include()
        ],
        language='c++',
        libraries=['sfml-window', 'sfml-system', 'sfml-graphics', 'sfml-audio', 'GLEW', 'GL']
    ),
]

setup(
    name='luigi',
    version=f'{LUIGI_VERSION_MAJOR}.{LUIGI_VERSION_MINOR}',
    author='Cc618',
    url='https://github.com/Cc618/Luigi',
    description='Luigi 2D game engine',
    ext_modules=ext_modules,
    setup_requires=['pybind11>=2.5.0'],
)
