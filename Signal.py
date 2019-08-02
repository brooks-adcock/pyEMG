from ADC import getSample
from threading import Timer
from numpy.fft import rfft
from numpy import absolute


class Signal:

	def __init__(self):
		self.SAMPLE_RATE_HZ = 60.0 * 4.0
		SECONDS_RETAINED = 5.0
		N_SAMPLES = self.SAMPLE_RATE_HZ * SECONDS_RETAINED

		self.samples = [0] * int(N_SAMPLES)
		self.is_running = False
		self.thread = Timer(1.0/self.SAMPLE_RATE_HZ, self.handle_function)

	def handle_function(self):
		self.samples.pop(0)
		self.samples.append(getSample())
		if self.is_running:
			self.thread = Timer(1.0, self.handle_function)
			self.thread.start()

	def getTimeSeries(self):
		return self.samples

	def getFFT(self):
		f_domain = rfft(map(lambda x: x[1], self.samples))
		return map(lambda i: (self.SAMPLE_RATE_HZ/2.0/i, absolute(f_domain[i])), range(0, len(f_domain)))

	def getPowerSpectralDensity(self):
		pass

	def start(self):
		self.is_running = True
		self.thread.start()

	def cancel(self):
		self.is_running = False
		self.thread.cancel()