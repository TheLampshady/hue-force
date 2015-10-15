#!/usr/bin/env python

import time

#import the adxl345 and neopixel module
from adxl345 import ADXL345
from neopixel import *

accel = ADXL345()
limit = 16
accel.setRange(adxl345.RANGE_16G)


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

