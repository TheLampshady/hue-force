#!/usr/bin/env python

import time

#import the adxl345 and neopixel module
from adxl345 import ADXL345
from neopixel import *

def get_strip(count=25, pin=):
    LED_COUNT


    # Create NeoPixel object with appropriate configuration.
    strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT)
    # Intialize the library (must be called once before other functions).
    strip.begin()


def get_axes(accel):
    return accel.getAxes(True)

def set_color(strip, axes, limit):
    mod = 255 / float(limit)

    red = int(axes['x']*mod)
    blue = int(axes['y']*mod)
    green = int(axes['z']*mod)
    colorWipe(strip, Color(red, green, blue)) # Red wipe


def main():
    accel = ADXL345()
    accel.setRange(adxl345.RANGE_16G)
    
    strip = get_strip()    

    print "Light Force Engage!: Ctrl-C to exit."
    

    while True:
        axes = get_axes()
        
if __name__ == "main":
    main()
