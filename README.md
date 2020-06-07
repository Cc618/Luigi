# Luigi

Luigi is a 2D Python game engine, written in C++.

## Examples

Some examples are provided in this reposiory, here are some pictures :

![Flappy Bird](res/flappy.gif)

## Install

### With pip

Luigi is available on PyPI, you can install luigi using pip with this command :

```sh
python3 -m pip install -i https://test.pypi.org/simple/ luigi-engine==0.1
```

[Check Luigi on pypi.](https://test.pypi.org/project/luigi-engine/0.1/)

Note that the package is named luigi-engine but the module to import luigi.

### With this repository

To install Luigi as a python package, type this command from the root of the repository :

```sh
python3 -m pip install .
```

You can add --user as option to avoid root permissions.

If you want to build Luigi as a developer, go to section Build which uses CMake for faster compilation times.

## Build

To compile Luigi using CMake :

```sh
# Install dependencies...
# Download pybind11
git submodule init
git submodule update --remote
# Generate build system
cmake .
# Build
make
# bin/luigi.cpython<system>.so should be created
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

To compile the documentation (require sphinx and sphinx_rtd_theme) :

```sh
cd doc
make html
```

## Dependencies

If you install this package with pip, no dependency is required.

- CMake (cmake for apt)
- SFML (libsfml-dev for apt)
- OpenGL
- pybind11 (just headers, run ```git submodule init && git submodule update --remote```)
- Sphinx (optional, to generate the documentation)
- Sphinx-rtd-theme (optional, to generate the documentation)
