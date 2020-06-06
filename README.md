# Luigi

Luigi is a 2D Python game engine, written in C++.

## Install

To install Luigi as a python package, type this command from the root of the repository :

``sh
python3 -m pip install .
``

This command should work also with python2, you can add --user as option to avoid root permissions.

If you want to build Luigi as a developer, go to section Build which uses CMake for faster compilation times. 

## Build

To compile Luigi :

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

- CMake (cmake for apt)
- SFML (libsfml-dev for apt)
- OpenGL
- pybind11 (just headers, run ```git submodule init && git submodule update --remote```)
- Sphinx (optional, to generate the documentation)
- Sphinx-rtd-theme (optional, to generate the documentation)
