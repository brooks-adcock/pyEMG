from ADC import getSample
from threading import Thread
from numpy.fft import rfft
from numpy import absolute, diff, average, std
import math
import time
import logging
logging.basicConfig(filename="./log.log")


class Signal:

	singleton = None

	def __init__(self):
		self.SAMPLE_RATE_HZ = 120.0
		self.SECONDS_RETAINED = 5.0
		N_SAMPLES = self.SAMPLE_RATE_HZ * self.SECONDS_RETAINED

		self.samples = [{'t': 0, 'y': 0}] * int(N_SAMPLES)
		self.is_running = False
		self.thread = Thread(target=self.handle_function)
		Signal.singleton = self

	def handle_function(self):
		while(True):
			t0 = time.time()
			while self.samples[-1]['t'] - self.samples[0]['t'] > self.SECONDS_RETAINED:
				self.samples.pop(0)
			self.samples.append(getSample())
			t1 = time.time()
			logging.info("Sample {}".format(t1-t0))
			time.sleep(1/self.SAMPLE_RATE_HZ)
		if not self.is_running:
			#todo kill thread
			pass

	def getTimeSeries(self):
		return self.samples

	def getFFT(self):
		profile_obj = self.getProfile()
		f_domain = rfft([x['y'] for x in self.samples])
		df = profile_obj['average_sample_frequency'] / float(len(self.samples))
		return [{'frequency': df*i, 'amplitude': absolute(f_domain[i])} for i in range(0, len(f_domain))]

	def getPowerSpectralDensity(self):
		fft = self.getFFT()
		power = [{"frequency": pair['frequency'], 'amplitude': pair['frequency']*pair['amplitude']} for pair in fft]
		total_power = sum([pair['amplitude'] for pair in power])
		psd = [{'frequency': pair['frequency'], 'amplitude': pair['amplitude']/total_power} for pair in power]
		return psd

	def getProfile(self):
		dt = diff([sample['t'] for sample in self.samples])
		f_avg = 1.0 / average(dt)
		profile_obj = {
			"target_sample_frequency": self.SAMPLE_RATE_HZ,
			"average_sample_frequency": f_avg,
			"stdev_sample_frequency": std(dt)*f_avg**2
		}
		return profile_obj

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

	def test(self, frequency):
		self.stop()
		times = [x/self.SAMPLE_RATE_HZ for x in range(int(self.SAMPLE_RATE_HZ*self.SECONDS_RETAINED))]
		self.samples = [{'t': t, 'y': math.sin(2*math.pi*frequency*t)} for t in times]
		print(self.samples)
