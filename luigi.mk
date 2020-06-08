# Use this file with make -f luigi.mk

all: build

# Compiles from scratch Luigi
build:
	@echo "Initializing submodules..."
	git submodule init
	git submodule update --remote

	@echo "Fetching dependencies..."	
	@echo "Fetching SFML..."	
	cd lib && \
		wget https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz && \
		tar -xf SFML-2.5.1-linux-gcc-64-bit.tar.gz

	@echo "Making build system..."
	cmake .

	@echo "Building Luigi..."
	make

	@echo
	@echo "--- Successfully built Luigi ---"

clean:
	rm -rf bin CMakeCache.txt cmake_install.cmake CMakeFiles lib/SFML-2.5.1 Makefile
	cd src && \
		rm -rf cmake_install.cmake CMakeFiles Makefile config.h
