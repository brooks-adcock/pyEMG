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


if __name__ == '__main__':
	app.run(debug=True)

