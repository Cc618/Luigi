# Luigi

Luigi is a 2D Python game engine, written in C++.

Now, Luigi targets only Linux based desktop devices.

## Documentation

The documentation is available [here](https://cc618.github.io/Luigi-Docs).

## Examples

Some examples are provided in this repository, here are some pictures :

![Flappy Bird](res/flappy.gif)

## Features

- 2D lightweight game engine for Python 3
- Fast : Written in C++, uses OpenGL as backend
- Free and open source (MIT license)
- Targets Linux based desktop devices

## Install

### With pip

Luigi is available on PyPI, you can install luigi by installing all dependencies and with this command :

```sh
python3 -m pip install luigi-engine
```

You can add --user as option to avoid root permissions.

[Check out Luigi on pypi.](https://pypi.org/project/luigi-engine)

Note that the package is named luigi-engine but the module to import is luigi.

### With this repository

To install Luigi as a python package, type this command from the root of the repository :

```sh
python3 -m pip install .
```

Like with pip, you can add --user as option to avoid root permissions.

If you want to build Luigi as a developer, go to section Build which uses CMake for faster compilation times.

## Build

To compile Luigi :

```sh
make -f luigi.mk
```

To compile the documentation (requires sphinx and sphinx_rtd_theme) :

```sh
make -f luigi.mk doc
```

## Test

To test Luigi :

```sh
cd bin
python3
import luigi
luigi.version
> '<luigi version>'
```

## Dependencies

- CMake (cmake for apt)
- SFML (libsfml-dev for apt)
- GLEW
- OpenGL
- pybind11 (just headers, will be downloaded when running luigi.mk)
- Sphinx (optional, to generate the documentation)
- Sphinx-rtd-theme (optional, to generate the documentation)
