package com.HomeGrownProgramming.RoboSunia;

import java.awt.Component;
import java.awt.Font;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.util.Hashtable;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class Main extends Thread {
	
	// 4 bytes are sent to the robot
	// 1st byte is the direction byte for motor 1
	// 2nd byte is the direction byte for motor 2
	// 3rd byte is the PWM value for motor 1 (0-255)
	// 4th byte is the PWM value for motor 2 (0-255)
	
	public static final int MAX_PWM = 255;
	public static final int MIN_PWM = 40;
	public static int limitPWM = 100;
	private int lmPWM = 0, rmPWM = 0;
	private int PWMInc = 30;
	
	private static WebTalker wt = null;
	private KeyAction ka;
	private static boolean go = true; 
	
//	String hostName = "171.66.76.46";   // actual
//	String hostName = "171.64.20.35"; 	// Wired
//	String hostName = "192.168.1.216";	// home
	static String hostName = "10.35.123.92";
//	String hostName = "127.0.0.1";
    static int portNumber = 12345;
	static String helpText= "This program will try to connect to a robot named RoboSunia in Stanford to control it.\nUsage:\n\tjava -jar RoboSuniaController.jar [ipaddr] [port]\n\n"
			+"ipaddr must be an IPv4 address to try to connect to and is optional as it will default to " + hostName + "\n"
			+"port is the port this program will attempt to connect on and is optional as it will default to " + Integer.toString(portNumber) + "\n";
	public UI ui;
	
	public Main(String hostName, int portNumber) {
		ka = new KeyAction();
		ui = new UI(ka);
		wt = new WebTalker(hostName, portNumber);
	}
	
	class WindowListener extends WindowAdapter {
		public void windowClosing(WindowEvent e) {
			try {
				Main.terminate();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}
	
	public static void terminate() throws IOException {
		// send terminating signal
		if(wt != null && wt.connected) {
			wt.send("quit");
			wt.close();
		}
		go = false;
		System.exit(0);
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
		
		if(ka.camUpPressed) {
			b[4] = '2';
		} else if(ka.camDownPressed) {
			b[4] = '0';
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
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args) {
	    if(args.length > 0) {
	    	if(args[0].equals("/h") || args[0].equals("-h")) {
	    		System.out.println(helpText);
	    		System.exit(0);
	    	}
	    	if(args.length >= 1) {
	    		if(args[0].matches("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)")) {
	    			hostName = args[0];
	    		} else {
	    			System.err.println("First argument does not look like an IPv4 address. Please try again.\n");
	    			System.err.println(helpText);
	    			System.exit(1);
	    		}
	    	}
	    	if(args.length == 2) {
	    		portNumber = Integer.parseInt(args[1]);
	    		if(portNumber < 1 || portNumber > 65535) {
	    			System.out.println("Second argument is out of the valid port range, or is not a number. Please try again.\n");
	    			System.err.println(helpText);
	    			System.exit(2);
	    		}
	    	}
	    } else { 
	    	System.out.println("Defaulting to connect to " + hostName + ":" + Integer.toString(portNumber));
	    }
		new Main(hostName, portNumber).start();
	}
}
