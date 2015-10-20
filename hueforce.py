#! /usr/bin/env python
import sys
import time
import argparse
import os
import signal

from accelerometer import Accelerometer
from light_strip import LightStrip


class HueForce(object):
    iteration = 0

    def __init__(self, lights, limit, debug, delta=10):
        self.accel = Accelerometer(limit)
        self.strip = LightStrip(lights, limit)
        self.debug = debug
        self.queue = [(0, 0, 0) for x in range(0,delta)]

    def run(self, delay=0.02):
        while True:
            self.set_force()
            time.sleep(delay)

    def set_force(self):
        axes = self.accel.getAxes(True)

        x, y, z = self.merge_results(axes)

        self.strip.draw(x, y, z)
        self.display_status(str(axes), str(x+y+z))

    def merge_results(self, axes):
        self.queue.append((abs(axes['x']), abs(axes['y']), abs(axes['z'])))

        result = []
        for r in range(0,3):
            a = [q[r] for q in self.queue]
            result.append(sum(a) / float(len(a)))

        del self.queue[0]
        return tuple(result)

    def display_status(self, axes, bright):
        if not self.iteration and self.debug:
            sys.stdout.write("                                            \r")
            sys.stdout.write("%s - %s\r" % (axes, bright))
            sys.stdout.flush()

        self.iteration += 1
        if self.iteration > 100:
            self.iteration = 0


def get_args():
    parser = argparse.ArgumentParser(prog='Hue Force fists of light.')
    parser.add_argument(
        '-d', '--debug', dest='debug', 
        action='store_true', help="Logs Axes Force to screen.")
    parser.add_argument(
        '-l', '--lights', required=True, type=int,
        help='Number of lights on the neo pixels')
    parser.add_argument(
        '-f', '--force', default=2,
        type=int, choices=[2, 4, 8, 16],
        help='G-Froce range to measure')

    return parser.parse_args()
    

def main():
    """
    Parses arguments and runs HueForce
    :return:
    """
    args = get_args()

    hueforce = HueForce(args.lights, args.force, args.debug)

    print "Light Force Engage!"
    if args.debug:
        print "Press Ctrl-C to exit."
    hueforce.run()


def set_exit_handler(func):
    signal.signal(signal.SIGTERM, func)
def on_exit(sig, func=None):
    print "exit handler triggered"
    sys.exit(1)

if __name__ == "__main__":
    set_exit_handler(on_exit)
    main()
