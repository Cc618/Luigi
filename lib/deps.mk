# To download depedencies

all: SFML-2.5.1

SFML-2.5.1:
	@echo "Fetching SFML..."	

	wget https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz
	tar -xf SFML-2.5.1-linux-gcc-64-bit.tar.gz

clean:
	rm -rf SFML-2.5.1*
