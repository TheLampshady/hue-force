#!/usr/bin/env python
import sys
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
        
        self.limit = limit

        # Create NeoPixel object with appropriate configuration.    
        super(LightStrip, self).__init__(LED_COUNT, LED_PIN)

        # Intialize the library (must be called once before other functions).
        self.begin()

    def draw(self, axes):
        mod = 255 / float(self.limit)

        red = abs(int(axes['x']*mod))
        blue = abs(int(axes['y']*mod))
        green = abs(int(axes['z']*mod))
        color = Color(red, blue, green)
        
        self.set_color(color)
        self.set_brightness(red+green+blue)
        self.show()

    # Define functions which animate LEDs in various ways.
    def set_color(self, color):
        """Wipe color across display a pixel at a time."""
        for i in range(self.numPixels()):
            self.setPixelColor(i, color)


    def set_brightness(self, bright):
        if bright > 255:
            bright = 255
        elif bright < 20:
            bright = 20
 
        self.setBrightness(bright)

def display_status(status, count):
    if not count:
        sys.stdout.write("                                            \r")
        sys.stdout.write("%s\r" % status)
        sys.stdout.flush()

def main():
    LIMIT = 16

    accel = Accelerometer(LIMIT)
    strip = LightStrip(25, LIMIT)

    print "Light Force Engage!: Ctrl-C to exit."
    
    count = 0
    while True:
        axes = accel.getAxes(True)
        display_status(str(axes), count)
        strip.draw(axes)
        count += 1
        if count > 100:
            count = 0


if __name__ == "main":
    main()
