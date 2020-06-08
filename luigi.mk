# Use this file with make -f luigi.mk

all: build

# Compiles from scratch Luigi
build:
	@echo "Initializing submodules..."
	git submodule init
	git submodule update --remote

	@echo "Fetching dependencies..."	
	cd lib && make -f deps.mk

	@echo "Making build system..."
	cmake .

	@echo "Building Luigi..."
	make

	@echo
	@echo "--- Successfully built Luigi ---"

clean:
	rm -rf bin CMakeCache.txt cmake_install.cmake CMakeFiles Makefile

	rm -rf .eggs build

	cd src && \
		rm -rf cmake_install.cmake CMakeFiles Makefile config.h
	
	cd lib && make -f deps.mk clean
