import time
import spidev

SPI_PORT = 0
SPI_DEVICE = 0
SPI_SPEED = 1000000

device = spidev.SpiDev()
device.open(SPI_PORT, SPI_DEVICE)
device.max_speed_hz = SPI_SPEED
# Default to mode 0, and make sure CS is active low.
device.mode = 0
#device.cshigh = False
#device.lsbfirst = False


def getSample():
	reading = bytearray(device.readbytes(2))
	hi = reading[0] & 0x1f
	lo = reading[1] & 0xf8

	v_in = (hi << 5) + (lo >> 3)
	VOLTAGE = 3.3
	v_in = v_in*1.0/1024.0-0.5
	v_in *= VOLTAGE

	return {"t": time.time(), "y": v_in}