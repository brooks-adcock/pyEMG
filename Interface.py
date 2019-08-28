from flask import Flask, render_template, jsonify
import math, json

app = Flask(__name__)

seconds_to_show = 3.0
sample_frequency = 60.0*4.0
t_ms = list(map(lambda x: float(x)/1000.0, range(int(seconds_to_show * sample_frequency))))
y = list(map(lambda t: math.sin(t), t_ms))
signal_data = list(map(lambda i: {'x': t_ms[i], 'y': y[i]}, range(int(seconds_to_show * sample_frequency))))


@app.route('/')
def index():
	return render_template('index.html')


@app.route("/signal")
def signal():
	return jsonify(signal_data)


if __name__ == '__main__':
	app.run(debug=True)

