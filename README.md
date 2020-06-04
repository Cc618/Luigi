# Luigi

Luigi is a 2D Python game engine, written in C++.

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
