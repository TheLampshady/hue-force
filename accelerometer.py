#import the adxl345
import adxl345

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

