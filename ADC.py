import time
import math, random


def getSample():
    t = time.time()
    f0 = 1.0
    a_s = 1.0
    a_60 = 0.0
    a_r = 0.2
    y = a_s * math.sin(2*math.pi*f0*t) + a_60*math.sin(2*math.pi*60*t) + a_r*random.randint(0,100)/100.0
    return {'t': t, 'y': y}

