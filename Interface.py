from flask import Flask, render_template, jsonify
import math, json

app = Flask(__name__)

seconds_to_show = 3.0
sample_frequency = 60.0*4.0
n_samples = int(seconds_to_show * sample_frequency)
t_ms = list(map(lambda x: float(x)/sample_frequency, range(n_samples)))
y = list(map(lambda t: math.sin(t*2*math.pi), t_ms))
signal_data = list(map(lambda i: {'x': t_ms[i], 'y': y[i]}, range(n_samples)))


@app.route('/')
def index():
	return render_template('index.html')


@app.route("/signal")
def signal():
	return jsonify(signal_data)


if __name__ == '__main__':
	app.run(debug=True)

