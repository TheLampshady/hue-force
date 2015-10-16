
install:
	git clone https://github.com/jgarff/rpi_ws281x.git
	cd rpi_ws281x
	scons
	cd python
	sudo python setup.py install