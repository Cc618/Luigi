# Luigi

Luigi is a 2D Python game engine, written in C++.

## Build

To compile Luigi :

```sh
# Install dependencies...
# Download pybind11
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

## Dependencies

- CMake (cmake for apt)
- SFML (libsfml-dev for apt)
- OpenGL
- pybind11 (run ```git submodule update --remote```, no need to install it)
