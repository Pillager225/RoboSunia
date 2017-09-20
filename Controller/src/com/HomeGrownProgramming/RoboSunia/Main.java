package com.HomeGrownProgramming.RoboSunia;

import java.io.IOException;

public class Main extends Thread {
	
	// 4 bytes are sent to the robot
	// 1st byte is the direction byte for motor 1
	// 2nd byte is the direction byte for motor 2
	// 3rd byte is the PWM value for motor 1 (0-255)
	// 4th byte is the PWM value for motor 2 (0-255)
	
	public static final int MAX_PWM = 255;
	public static final int MIN_PWM = 40;
	public static final int MAX_DEBUG_LEVEL = 4;
	public static final int REQUESTED_TERMINATION = 0, HELP_TEXT = 0, UNEXPECTED = 1, INVALID_DEBUG = 2, INVALID_CONNECTION = 3, INVALID_PORT = 4, UNKNOWN_HOST = 5, CANT_REACH_HOST = 6;
	public static final String ipRegex = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)";
	public static final String portRegex = "[0-9]+";
	public static final String connectionRegex = ipRegex+":"+portRegex;
	public static int debugLevel = 1;
	
	public static int limitPWM = 100;
	private int lmPWM = 0, rmPWM = 0;
	private int PWMInc = 30;

	private static WebTalker wt = null;
	private KeyAction ka;
	private static boolean go = true; 
//	String hostName = "171.66.76.46";   // actual
//	String hostName = "171.64.20.35"; 	// Wired
//	String hostName = "192.168.1.216";	// home
	private static String hostName = "10.35.123.92";
//	String hostName = "127.0.0.1";	
    private static int portNumber = 12345;
	static String helpText = "This program will try to connect to a robot named RoboSunia in Stanford to \n" + 
				"control it.\nUsage:\n\tjava -jar RoboSuniaController.jar [/h] [/d debugLevel] [[ipaddr] [port] || [connection]]\n\n" +
			
				"Arguments:\n" +
				"\t/h will show this help text\n\n" +
				
				"\t/d will turns on debugging messages to be shown or logged. The default\n" +
				"\tdebugLevel is 1. debugLevel can be\n" +
				"\t\t1 for printing to stdout the basic messages about function\n" +
				"\t\t2 additonally prints all communication packets\n" +
				"\t\t3 additionally logs basic messages to file\n" +
				"\t\t4 additionally logs all communciation packtes\n\n" +
				
				"\tipaddr must be an IPv4 address to try to connect to and is optional as it will\n" + 
				"\tdefault to " + hostName + "\n\n" +
				
				"\tport is the port this program will attempt to connect on and is optional as it\n" +
				"\twill default to " + Integer.toString(portNumber) + "\n\n" +
				
				"\tconnection is the ipaddr and port put together with a colon in the middle\n" + 
				"\t(ie. " + hostName + ":" + Integer.toString(portNumber) + ")\n\n";
	
	public UserInterface ui;
	
	public Main(String hostName, int portNumber) {
		ka = new KeyAction();
		ui = new UserInterface(ka);
		wt = new WebTalker(hostName, portNumber);
	}
	
	public static void terminate() throws IOException {
		// send terminating signal
		if(wt != null && wt.connected) {
			wt.send("quit");
			wt.close();
		}
		go = false;
	}
	
	private String removePadding(String s) {
		int i;
		for(i = s.length()-1; i >= 0; i--) {
			if(s.charAt(i) != ' ') {
				break;
			}
		}
		return s.substring(0, i+1);
	}
	
	private void motorLogic() {
		if(ka.forwardPressed) {
			if(ka.leftPressed) {
				ka.lmState = 0;
				ka.rmState = 1;
			} else if(ka.rightPressed) {
				ka.lmState = 1;
				ka.rmState = 0;
			} else {
				ka.lmState = 1;
				ka.rmState = 1;
			}
		} else if(ka.backwardPressed) {
			if(ka.leftPressed) {
				ka.lmState = -1;
				ka.rmState = 0;
			} else if(ka.rightPressed) {
				ka.lmState = 0;
				ka.rmState = -1;
			} else {
				ka.lmState = -1;
				ka.rmState = -1;
			}
		} else if(ka.leftPressed) {
			ka.lmState = -1;
			ka.rmState = 1;
		} else if(ka.rightPressed) {
			ka.lmState = 1;
			ka.rmState = -1;
		}
	}
	
	// This will turn the boolean values about the motor states stored in ka into a string.
	private char[] getControlPacket() {
		motorLogic();
		char[] b = new char[7];
		b[0] = (char) (ka.lmState == -1 ? 1 : 0);
		b[1] = (char) (ka.rmState == -1 ? 1 : 0);
		if(ka.lmState != 0) {
			lmPWM = lmPWM+PWMInc > limitPWM ? limitPWM : lmPWM+PWMInc;
		} else {
			lmPWM = lmPWM-PWMInc < 0 ? 0 : lmPWM-PWMInc;
		}
		if(ka.rmState != 0) {
			rmPWM = rmPWM+PWMInc >= limitPWM ? limitPWM : rmPWM+PWMInc;
		} else {
			rmPWM = rmPWM-PWMInc < 0 ? 0 : rmPWM-PWMInc;
		}
		b[2] = (char) lmPWM;
		b[3] = (char) rmPWM;
		if(!ka.camCenterPressed) {
			if(ka.camUpPressed) {
				b[4] = '0';
			} else if(ka.camDownPressed) {
				b[4] = '2';
			} else {
				b[4] = '1';
			}
			if(ka.camRightPressed) {
				b[5] = '2';
			} else if(ka.camLeftPressed) {
				b[5] = '0';
			} else {
				b[5] = '1';
			}
		} else {
			ka.camCenterPressed = false;
			b[4] = '3';
			b[5] = '3';
		}
		b[6] = 13;
		return b;
	}
	
	@Override
	public void run() {
		while(go) {
			try {
				wt.send(getControlPacket());
				String input = wt.read();
				if(input.length() > 0) {
					input = removePadding(input);
					ui.distanceLabel.setText("Sensed Distance: " + input);
				}
				Thread.sleep(100);
			} catch (InterruptedException | IOException e) {
				Logger.log(e.getStackTrace().toString(), Main.debugLevel);
			}
		}
		System.exit(REQUESTED_TERMINATION);
	}
	
	private static void argumentError(int errorNum) {
		switch(errorNum) {
		case HELP_TEXT: // help text is all that is required
			break;
		case UNEXPECTED: 
			Logger.log("Unexpected arguments.", debugLevel);
			break;
		case INVALID_DEBUG:
			Logger.log("Debug level was invalid.", debugLevel);
			break;
		case INVALID_CONNECTION:
			Logger.log("Connection string look invalid", debugLevel);
			break;
		case INVALID_PORT:
	    	Logger.log("Port argument is out of the valid port range, or is not a number. Please try again.\n", debugLevel);
	    	break;
		}
		Logger.log(helpText, debugLevel);
		System.exit(errorNum);
	}
	
	private static boolean isValidPort(String s) {
		if(!s.matches(portRegex)) {
			return false;
		}
		int num = Integer.parseInt(s);
		if(num < 1 || num > 65535) {
			return false;
		}
		return true;
	}
	
	private static void checkArguments(String[] args) {
		boolean gotIP = false, gotPort = false;
		for(int i = 0; i < args.length; i++) {
			if(args[i].equals("/h") || args[i].equals("-h")) { // help flag
				argumentError(HELP_TEXT);
			} else if(args[i].equals("/d") || args[i].equals("-d")) { // debug flag
				if(i+1 < args.length) {
					if(args[i+1].matches(portRegex)) {
						debugLevel = Integer.parseInt(args[i+1]);
						if(debugLevel > MAX_DEBUG_LEVEL) {
							argumentError(INVALID_DEBUG);
						}
						i++;
					}
				}
			} else if(args[i].matches(connectionRegex) && !gotIP && !gotPort) { // Connection checking
				String[] data = args[i].split(":");
				if(data.length > 2 || !data[0].matches(ipRegex) || !isValidPort(data[1])) {
					argumentError(INVALID_CONNECTION);
				}
				hostName = data[0];
				portNumber = Integer.parseInt(data[1]);
				gotIP = true;
				gotPort = true;
			} else if(args[i].matches(ipRegex) && !gotIP) { // IP Checking
    			hostName = args[0];
				gotIP = true;
			} else if(args[i].matches(portRegex) && gotIP && !gotPort) { // Port Checking
    			portNumber = Integer.parseInt(args[i]);
    			if(portNumber < 1 || portNumber > 65535) {
	    			argumentError(INVALID_PORT);
	    		}
    		} else {
    			argumentError(UNEXPECTED);
    		}
		}
	}
	
	public static void main(String[] args) {
		checkArguments(args);
		Logger.log("Connecting to " + hostName + ":" + Integer.toString(portNumber), debugLevel);
		new Main(hostName, portNumber).start();
	}
}
