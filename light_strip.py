from neopixel import *


class LightStrip(Adafruit_NeoPixel):

    def __init__(self, count, limit, pin=18):
        LED_COUNT   = count   # Number of LED pixels.
        LED_PIN     = pin     # GPIO pin connected to the pixels (must support PWM!).
        
        self.limit = limit
        self.brightness = 0

        # Create NeoPixel object with appropriate configuration.    
        super(LightStrip, self).__init__(LED_COUNT, LED_PIN)

        # Intialize the library (must be called once before other functions).
        self.begin()

    def draw(self, red, blue, green):
        mod = 255 / float(self.limit)

        red = abs(int(red*mod))
        blue = abs(int(blue*mod))
        green = abs(int(green*mod))
        color = Color(red, blue, green)
        
        self.set_color(color)
        self.set_brightness(red+green+blue)
        self.show()

    # Define functions which animate LEDs in various ways.
    def set_color(self, color):
        """Wipe color across display a pixel at a time."""
        for i in range(self.numPixels()):
            self.setPixelColor(i, color)


    def set_brightness(self, bright, delta=1):
        if bright > 255:
            bright = 255
        elif bright < 15:
            bright = 15
        if bright < self.brightness:
            self.brightness = self.brightness - delta
        else:
            self.brightness = bright
        self.setBrightness(self.brightness)
