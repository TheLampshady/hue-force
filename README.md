## Project Description
This application will measure the acceleration of a device and dispaly different colors based on the force and direction. 

## Hardware
* ADXL345 Accelerometer 
	* http://www.analog.com/en/products/mems/mems-accelerometers/adxl345.html
* Neo Pixels LED Lights
	* https://www.adafruit.com/products/1138


## Installations:
The following applications are required.
### 1. OS 

    python-smbus
    i2c-tools
    git-core
    build-essential
    python-dev
    scons
    swig
    
#### Raspian OS: Commands

    sudo apt-get install 
    sudo apt-get install python-smbus i2c-tools git-core build-essential python-dev scons swig
    sudo apt-get -yy autoclean
 
### 2. Python Installs:
* The ADXL345 Lib 
	* created by pimoroni https://github.com/pimoroni/adxl345-python
* Neo Pixels
	* https://github.com/jgarff/rpi_ws281x.git

#### pip installs
	pip install -r requirements.txt
	pip install -e "git+https://github.com/jgarff/rpi_ws281x.git#egg=rpi_ws281x&subdirectory=python"
