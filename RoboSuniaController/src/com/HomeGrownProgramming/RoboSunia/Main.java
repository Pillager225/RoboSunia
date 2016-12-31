package com.HomeGrownProgramming.RoboSunia;

import java.io.IOException;

import javax.swing.JFrame;

public class Main extends Thread {
	
	// 4 bytes are sent to the robot
	// 1st byte is the direction byte for motor 1
	// 2nd byte is the direction byte for motor 2
	// 3rd byte is the PWM value for motor 1 (0-255)
	// 4th byte is the PWM value for motor 2 (0-255)
	
	private static final int MAX_PWM = 170;
	
	private static WebTalker wt;
	private KeyAction ka;
	
	public Main() {
		wt = new WebTalker();
		makeFrame();
	}
	
	private void makeFrame() {
		JFrame frame = new JFrame("RoboSunia Control Window");
		frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		frame.setSize(600,400);
		frame.setLocation(10,30);
		frame.setVisible(true);
		ka = new KeyAction();
		frame.addKeyListener(ka);
	}
	
	public static void terminate() throws IOException {
		// send terminating signal
		wt.send("rese");
		wt.close();
		System.exit(0);
	}
	
	// This will turn the boolean values about the motor states stored in ka into a string.
	private char[] getMotorStates() {
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
		char[] b = new char[4];
		b[0] = (char) (ka.lmState == -1 ? 0 : 1);
		if(ka.lmState != 0) {
			b[2] = (char)MAX_PWM;
		}
		b[1] = (char)(ka.rmState == -1 ? 0 : 1);
		if(ka.rmState != 0) {
			b[3] = (char)MAX_PWM;
		}
		return b;
	}
	
	@Override
	public void run() {
		for(;;) {
			try {
				wt.send(getMotorStates());
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void main(String[] args) {
		new Main().start();
	}
}
