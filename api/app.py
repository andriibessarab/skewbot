import os
import re
import sys

from flask import Flask, jsonify, request, send_from_directory

import vex_comm

app = Flask(__name__, static_folder='dist')

VEX_PORT = os.environ.get('VEX_PORT')
print(f"Attempting to connect on port: {VEX_PORT}")
print("(Set VEX_PORT environment variable to override)")

if not vex_comm.init_serial(VEX_PORT):
    print('FATAL: VEX connection failed. Server is exiting.')
    sys.exit(1)


@app.route('/api/send_state', methods=['POST', 'OPTIONS'])
def send_state():
    # Ensure content type is JSON
    if not request.is_json:
        return jsonify({'error': 'Invalid content type, expected application/json'}), 415

    # Parse JSON safely
    data = request.get_json(silent=True)
    if data is None:
        return jsonify({'error': 'Malformed JSON'}), 400

    # Validate required field
    state = data.get('state')
    if state is None:
        return jsonify({'error': 'Missing required field: state'}), 400

    if not re.fullmatch(r'^[WYGBRO]{30}$', state):
        return jsonify({'error': 'Invalid skewb state format'}), 400

    # Send state to VEX controller
    if not vex_comm.send_to_vex(state):
        return jsonify({'error': 'Failed to send state to VEX controller'}), 500

    return jsonify({'status': 'ok', 'received_state': state}), 200


@app.errorhandler(500)
def handle_internal_error(error):
    return jsonify({'error': 'Internal server error'}), 500


@app.route('/', methods=['GET'])
def index():
    return send_from_directory(app.static_folder, 'index.html')


@app.route('/<path:path>')
def serve_react_app(path):
    if os.path.exists(os.path.join(app.static_folder, path)):
        return send_from_directory(app.static_folder, path)

    return send_from_directory(app.static_folder, 'index.html')


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5001, debug=True)
