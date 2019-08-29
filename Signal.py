from ADC import getSample
from threading import Timer
from numpy.fft import rfft
from numpy import absolute


class Signal:

	singleton = None

	def __init__(self):
		self.SAMPLE_RATE_HZ = 60.0 * 4.0
		self.SECONDS_RETAINED = 5.0
		N_SAMPLES = self.SAMPLE_RATE_HZ * self.SECONDS_RETAINED

		self.samples = [{'t': 0, 'y': 0}] * int(N_SAMPLES)
		self.is_running = False
		self.thread = Timer(1.0/self.SAMPLE_RATE_HZ, self.handle_function)
		Signal.singleton = self

	def handle_function(self):
		while self.samples[-1]['t'] - self.samples[0]['t'] > self.SECONDS_RETAINED:
			self.samples.pop(0)
		self.samples.append(getSample())
		if self.is_running:
			self.thread = Timer(1.0/self.SAMPLE_RATE_HZ, self.handle_function)
			self.thread.start()

	def getTimeSeries(self):
		return self.samples

	def getFFT(self):
		f_domain = rfft(list(map(lambda x: x['y'], self.samples)))
		df = self.SAMPLE_RATE_HZ / float(len(self.samples))
		return list(map(lambda i: {'frequency': df*i, 'amplitude': absolute(f_domain[i])}, range(0, len(f_domain))))

	def getPowerSpectralDensity(self):
		return self.getFFT()

	def start(self):
		print("Starting Signal")
		if not self.is_running:
			self.is_running = True
			self.thread.start()

	def stop(self):
		print("Stopping Signal")
		if self.is_running:
			self.is_running = False
			self.thread.cancel()
