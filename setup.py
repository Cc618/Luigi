from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.sdist import sdist
import sys
import os
import setuptools
import glob
import shutil


LUIGI_VERSION_MAJOR = 0
LUIGI_VERSION_MINOR = 1


class Luigi(Extension):
    def __init__(self):
        super().__init__('luigi', sources=[])


class LuigiBuild(build_ext):
    def run(self):
        self.build(self.extensions[0])

        super().run()

    def build(self, extension):
        extension_path = self.get_ext_fullpath(extension.name)
        slash = extension_path.rfind('/')
        extension_name = extension_path[slash + 1:]
        extension_dir = extension_path[:slash]
        bin_path = f'bin/{extension_name}'

        self.spawn(['make', '-f', 'luigi.mk'])

        os.makedirs(extension_dir)

        # Copy the file in bin to the build
        shutil.copy(bin_path, extension_path)


long_desc = '''# Luigi

Luigi is a 2D Python game engine, written in C++.

Now, Luigi targets only Linux based desktop devices.

## Features

- 2D lightweight game engine for Python 3
- Fast : Written in C++, uses OpenGL as backend
- Free and open source (MIT license)
- Targets Linux based desktop devices

## Pip dependencies

- SFML
- GLEW
- OpenGL
'''

# The name is luigi-engine since a package luigi already exists on pypi.
setup(
    name='luigi-engine',
    version=f'{LUIGI_VERSION_MAJOR}.{LUIGI_VERSION_MINOR}.2',
    author='Cc618',
    author_email='celian.dev@gmail.com',
    url='https://github.com/Cc618/Luigi',
    description='Luigi 2D game engine',
    long_description=long_desc,
    long_description_content_type='text/markdown',
    ext_modules=[Luigi()],
    classifiers=[
        'Development Status :: 4 - Beta',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: C++',
        'Topic :: Games/Entertainment',
    ],
    setup_requires=['cmake'],
    cmdclass={
        'build_ext': LuigiBuild,
    }
)
