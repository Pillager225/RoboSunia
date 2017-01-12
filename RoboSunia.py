import serial, time, socket, sys, _thread, argparse

#10.21.163.151

class RoboSunia:
	commandPacketLength = 4
	serversocket = None
	clientsocket = None
	serialConnection = None
	go = True
	debugging = False

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
		self.serialConnection.write([0, 0, 0, 0])
		if self.clientsocket:
			self.clientsocket.close()
			self.clientsocket = None
		if self.serversocket:
			self.serversocket.close()
			self.serversocket = None
		self.serverSetup()

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
				ser.write(b'reset')
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
		self.serversocket.listen(0)
		print("Wifi server started")

	def handleSerialConnection(self):
		try:
			serData = self.serialConnection.read(16).decode('utf-8')
			if serData:
				serDataString = ''.join(str(e) for e in serData)
				if self.debugging:
					print(serDataString)
				serData = serDataString.split()
				distance = serData[0]+'\0'
				if self.clientsocket:
					self.clientsocket.send(distance)
		except Exception as msg:
			print("An error occurred while communicating with the Arduino. The error was:")
			print(msg)
			print("Resetting robot.")

	def handleWifiConnection(self):
		try:
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
				elif data == 'rese':
					self.resetClient()
					self.waitForConnection()
				else:
					preData = data.decode('utf-8').split()
					if len(preData) == 1:
						data = bytes(preData[0], 'utf-8')
						if self.debugging:
							print(data)
						self.serialConnection.write(data)
		except ConnectionError as msg:
			print("A connection error was detected. Its error was")
			print(msg)
			print("Resetting robot.")
			self.resetClient()
			self.waitForConnection()

	def __init__(self):
		parser = argparse.ArgumentParser(description="Arguments are for debuggin only.")
		parser.add_argument('-d', dest='debugging', action="store_true", help="Enables debugging messages")
		args = parser.parse_args()
		self.debugging = args.debugging
		self.serialConnection = self.getSerialConnection()
		if self.serialConnection:
			_thread.start_new_thread(self.handleSerialConnection, ())	
			try:
				self.serverSetup()
				self.waitForConnection()
				while self.go:
					self.handleWifiConnection()
				self.exitGracefully()
			except KeyboardInterrupt: 
				print("Keyboard interrupt detected. Exiting program.")
				self.exitGracefully()
		else:
			print("Companion Arduino could not be found. Try once more or reboot.")

if __name__ == "__main__":
	RoboSunia()