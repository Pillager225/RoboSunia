import serial, time

class ArduinoController(Process):
	
	def getSerialConnection(self):
		portPrefix = "\\\\.\\COM"
		ser = None
		# skip serial port 1 because that one is always some unknown device
		for i in range(2, 9):
			try:
				ser = serial.Serial(portPrefix+str(i), 9600)
				ser.timeout = 2
			except Exception:
				pass
			else:
				if b'Arduino' == ser.read(len("Arduino")):
					ser.write(b'connected')
					return ser
		return None

print("Beginning search")
ser = getSerialConnection()
if ser:
	print("Arduino found")
	ser.reset_input_buffer()
	ser.write(b'reset')
	ser.close()
else:
	print("Companion Arduino could not be found. Try rebooting.")