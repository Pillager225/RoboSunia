import serial, time, socket

commandPacketLength = 4
serversocket = 
clientsocket = None
serialConnection = None
go = True

def waitForConnection():
	if clientsocket == None:
		connected = False
		while not connected:
			try:
				sys.stdout.write('Waiting for client connection... ')
				sys.stdout.flush()
				(clientsocket, address) = serversocket.accept()
				connected = True
				print 'Client connected'
			except Exception as msg:
				print 'Client connection failed with message:'
				print msg
				print 'I will retry connecting in one second.'
				time.sleep(1)

def resetClient():
	ser.write([1, 1, 0, 0])
	if clientsocket:
		clientsocket.close()
	clientsocket = None

def getSerialConnection():
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

def exitGracefully():
	if clientsocket:
		clientsocket.close()
	serialConnection.write(b'reset')
	serialConenction.close()

def serverSetup():
	serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	serversocket.bind(('', 12345))
	# listen for only one connection
	serversocket.listen(1)
	print "Wifi server started"

def main():
	serialConnection = getSerialConnection()
	if serialConnection:
		serverSetup()
		waitForConnection()
		while go:
			# 4 because there are only 4 bytes in a command packet
			# data[0:2] are the dirs
			# data[2:4] are the pwms
			data = clientsocket.recv(commandPacketLength)	
			if len(data) == 0:
				resetClient()
				waitForConnection()
			elif len(data) == commandPacketLength:
				if data == 'quit':
					go = False
				else:
					for i in range(len(data)):
						print(int(data[i]))
					serialConnection.write(data)
		exitGracefully()
	else:
		print("Companion Arduino could not be found. Try rebooting.")

if __name__ == "__main__":
	main()