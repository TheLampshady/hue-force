#!/usr/bin/env python

import time

#import the adxl345 and neopixel module
import adxl345
from neopixel import *


class Accelerometer(adxl345.ADXL345):

    g_range = {
        2: adxl345.RANGE_2G,
        4: adxl345.RANGE_4G,
        8: adxl345.RANGE_8G,
        16: adxl345.RANGE_16G,
    }

    def __init__(self, limit=2):
         super(Accelerometer, self).__init__()
         self.setRange(self.g_range.get(limit, 0))



class LightStrip(Adafruit_NeoPixel):

    def __init__(self, count, limit, pin=18):
        LED_COUNT   = count   # Number of LED pixels.
        LED_PIN     = pin     # GPIO pin connected to the pixels (must support PWM!).
        LED_FREQ_HZ = 800000  # LED signal frequency in hertz (usually 800khz)
        LED_DMA     = 5       # DMA channel to use for generating signal (try 5)
        LED_INVERT  = False   # True to invert the signal (when using NPN transistor level shift)

        self.limit = limit

        # Create NeoPixel object with appropriate configuration.    
        #Adafruit_NeoPixel.__init__(self, LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT)
        super(LightStrip, self).__init__(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT)

        # Intialize the library (must be called once before other functions).
        self.begin()

    def draw(self, axes):
        mod = 255 / float(self.limit)

        red = abs(int(axes['x']*mod))
        blue = abs(int(axes['y']*mod))
        green = abs(int(axes['z']*mod))
        color = Color(red, green, blue)

        self.set_color(self, color, bright)
        self.set_brightness(self, red+green+blue)
        self.show()

    # Define functions which animate LEDs in various ways.
    def set_color(self, color):
        """Wipe color across display a pixel at a time."""
        for i in range(self.numPixels()):
            self.setPixelColor(i, color)


    def set_brightness(self, bright):
        bright = 255 if bright > 255 else bright
        self.setBrightness(bright)


def main():
    LIMIT = 16

    #accel = ADXL345()
    accel = Accelerometer(LIMIT)
    #strip = get_strip(25)
    strip = LightStrip(25, LIMIT)

    print "Light Force Engage!: Ctrl-C to exit."
    

    while True:
        axes = accel.getAxes(True)
        strip.draw(axes)
        
if __name__ == "main":
    main()
