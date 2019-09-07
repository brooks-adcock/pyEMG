from flask import Flask, render_template, jsonify
import math, json
from Signal import Signal

app = Flask(__name__)

signal = Signal()
#signal.test(110)
signal.start()


@app.route('/')
def index():
	return render_template('index.html')


@app.route("/signal")
def signal():
	return jsonify(Signal.singleton.getTimeSeries())


@app.route("/psd")
def psd():
	return jsonify(Signal.singleton.getPowerSpectralDensity())


@app.route("/profile")
def profile():
	return jsonify(Signal.singleton.getProfile())


if __name__ == '__main__':
	app.run(host='0.0.0.0', port=5000)

