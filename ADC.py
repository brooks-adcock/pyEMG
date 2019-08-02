import time
import math


def getSample():
    t = time.time()
    f0 = 10.0
    snr = 8.0
    y = math.sin(2*math.pi*f0*t)+1.0/snr*math.sin(2*math.pi*60*t)
    return t, y

