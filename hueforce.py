#!/usr/bin/env python
import sys
import time

from accelerometer import Accelerometer
from light_strip import LightStrip

def class HueForce(object):
    self.iteration = 0

    def __init__(self, lights, limit):
        self.accel = Accelerometer(limit)
        self.strip = LightStrip(lights, limit)

    def run():
        while True:
            self.set_force()

    def set_force():
        axes = accel.getAxes(True)
        display_status(str(axes))
        strip.draw(axes['x'], axes['y'], axes['z'])

    def display_status(self, status):
        if not self.iteration:
            sys.stdout.write("                                            \r")
            sys.stdout.write("%s\r" % status)
            sys.stdout.flush()

        self.iteration += 1
        if self.iteration > 100:
            self.iteration = 0


def get_args():
    parser = argparse.ArgumentParser(prog='Hue Force fists of light.')
    parser.add_argument(
        '-l', '--lights', required=True, type=int, 
        help='Number of lights on the neo pixels')
    parser.add_argument(
        '-f', '--force', default=2, 
        type=int, choices=[2, 4, 8, 16], 
        help='G-Froce range to measure')
    
    return parser.parse_args()

def main():
    '''
    Parses arguments and runs HueForce
    '''
    args = get_args()

    hueforce = HueForce(args.lights, args.force)

    print "Light Force Engage!: Ctrl-C to exit."
    hueforce.run()

if __name__ == "main":
    main()
