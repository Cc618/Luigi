from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
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
        extension_name = extension_path[extension_path.rfind('/') + 1:]
        bin_path = f'bin/{extension_name}'

        self.spawn(['make', '-f', 'luigi.mk'])

        # Copy the file in bin to the build
        shutil.copy(bin_path, extension_path)


# The name is luigi-engine since a package luigi already exists on pypi.
setup(
    name='luigi-engine',
    version=f'{LUIGI_VERSION_MAJOR}.{LUIGI_VERSION_MINOR}',
    author='Cc618',
    url='https://github.com/Cc618/Luigi',
    description='Luigi 2D game engine',
    long_description='Luigi 2D game engine',
    long_description_content_type='text/markdown',
    ext_modules=[Luigi()],
    classifiers=[
        'Programming Language :: Python :: 3',
    ],
    setup_requires=['pybind11>=2.5.0', 'cmake'],
    cmdclass={
        'build_ext': LuigiBuild
    }
)
