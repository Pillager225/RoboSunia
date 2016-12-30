import serial, time, socket, sys

class RoboSunia:
	commandPacketLength = 4
	serversocket = None
	clientsocket = None
	serialConnection = None
	go = True

	def waitForConnection(self):
		if self.clientsocket == None:
			connected = False
			while not connected:
				try:
					sys.stdout.write('Waiting for client connection... ')
					sys.stdout.flush()
					(self.clientsocket, address) = self.serversocket.accept()
					connected = True
					print('Client connected')
				except Exception as msg:
					print('Client connection failed with message:')
					print(msg)
					print('I will retry connecting in one second.')
					time.sleep(1)

	def resetClient(self):
		ser.write([1, 1, 0, 0])
		if self.clientsocket:
			self.clientsocket.close()
		self.clientsocket = None

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
				# len('Arduino') == 7
				if b'Arduino' == ser.read(7):
					ser.write(b'connected')
					return ser
		return None

	def exitGracefully(self):
		if self.clientsocket:
			self.clientsocket.close()
		if self.serversocket:
			self.serversocket.close()
		self.serialConnection.write(b'reset')
		self.serialConnection.close()

	def serverSetup(self):
		self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.serversocket.bind(('', 12345))
		# listen for only one connection
		self.serversocket.listen(1)
		print("Wifi server started")

	def __init__(self):
		self.serialConnection = self.getSerialConnection()
		if self.serialConnection:
			try:
				self.serverSetup()
				self.waitForConnection()
				while self.go:
					# 4 because there are only 4 bytes in a command packet
					# data[0:2] are the dirs
					# data[2:4] are the pwms
					data = self.clientsocket.recv(self.commandPacketLength)	
					if len(data) == 0:
						self.resetClient()
						self.waitForConnection()
					elif len(data) == self.commandPacketLength:
						if data == 'quit':
							self.go = False
						else:
							for i in range(len(data)):
								print(int(data[i]))
							self.serialConnection.write(data)
				self.exitGracefully()
			except Exception as msg: # This should be a KeyboardException, but I wanna catch other ones too if necessary
				print(msg)
				self.exitGracefully()
		else:
			print("Companion Arduino could not be found. Try rebooting.")

if __name__ == "__main__":
	RoboSunia()