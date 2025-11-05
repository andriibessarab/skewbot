import serial

# This variable will hold our connection
vex_serial = None


def init_serial(port_name):
    """
    Open the serial port connection to the VEX brain.
    Returns True on success, False on failure.
    """
    global vex_serial
    try:
        vex_serial = serial.Serial(port_name, 9600, timeout=1)
        print(f"VEX serial port '{port_name}' opened successfully.")
        return True
    except Exception as e:
        print(f"ERROR: Could not open serial port '{port_name}'.")
        print(f"Reason: {e}")
        vex_serial = None
        return False


def send_to_vex(message):
    """
    Sends a string message to the VEX brain.
    Appends the required newline character.
    """
    if not vex_serial:
        print("ERROR: Serial port is not open. Cannot send message.")
        return False

    try:
        # Format the message with a newline and encode it
        formatted_message = f"{message}\n"
        vex_serial.write(formatted_message.encode('utf-8'))

        print(f"Sent to VEX: '{message}'")
        return True
    except Exception as e:
        print(f"ERROR: Failed to write to serial port: {e}")
        return False
