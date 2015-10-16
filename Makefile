
install:
	git clone https://github.com/jgarff/rpi_ws281x.git
	cd rpi_ws281x && scons
	cd rpi_ws281x/python && sudo python setup.py install
	sudo rm -rf rpi_ws281x
