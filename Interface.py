from flask import Flask, render_template, jsonify
import math, json
from Signal import Signal

app = Flask(__name__)

signal = Signal()
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


if __name__ == '__main__':
	app.run(debug=True)

