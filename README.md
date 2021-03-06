# Project Description
This application will measure the acceleration of a device and dispaly different colors based on the force and direction. 

## Hardware
* ADXL345 Accelerometer 
	* http://www.analog.com/en/products/mems/mems-accelerometers/adxl345.html
* Neo Pixels LED Lights
	* https://www.adafruit.com/products/1138


#Raspberry Pi

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

#### scons
    git clone https://github.com/jgarff/rpi_ws281x.git
    cd rpi_ws281x
    scons

#### pip installs
	pip install -r requirements.txt


### 3. Setup

#### Check i2c devices
    sudo i2cdetect -y 1
    
#### Run on startup
    # update boot / reboot files
    sudo cp hueforce /etc/init.d/
    # do it as soon as the device is going down,
    #   both for shutdown and reboot
    sudo update-rc.d hueforce defaults
    
### Code Refences
* https://learn.adafruit.com/neopixels-on-raspberry-pi/software
* http://www.stuffaboutcode.com/2014/06/raspberry-pi-adxl345-accelerometer.html

#Arduino

##Libraries
* http://playground.arduino.cc/Code/QueueList
* https://github.com/adafruit/Adafruit_NeoPixel
* https://github.com/adafruit/Adafruit_ADXL345
* https://github.com/adafruit/Adafruit_Sensor
* 
