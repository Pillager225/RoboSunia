package com.HomeGrownProgramming.RoboSunia;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;

public class KeyAction implements KeyListener {

	// lm = left motor, rm = right motor, F = forward, B = backward
	public static final int lmFKey = KeyEvent.VK_O, lmBKey = KeyEvent.VK_PERIOD;
	public static final int rmFKey = KeyEvent.VK_P, rmBKey = KeyEvent.VK_SLASH;
	public static final int FORWARD = KeyEvent.VK_UP, LEFT = KeyEvent.VK_LEFT, RIGHT = KeyEvent.VK_RIGHT, BACK = KeyEvent.VK_DOWN;
	public static final int camUp = KeyEvent.VK_W, camDown = KeyEvent.VK_S, camLeft = KeyEvent.VK_A, camRight = KeyEvent.VK_D, camCenter = KeyEvent.VK_SPACE;
	public static final int QUIT = KeyEvent.VK_ESCAPE;
	public int lmState = 0, rmState = 0;
	public boolean forwardPressed = false, backwardPressed = false, leftPressed = false, rightPressed = false;
	public boolean camUpPressed = false, camDownPressed = false, camLeftPressed = false, camRightPressed = false, camCenterPressed = false;
	private int lmPWM = 0, rmPWM = 0;
	private int PWMInc = 30;

	@Override
	public void keyPressed(KeyEvent e) {
		switch (e.getKeyCode()) {
			case QUIT:
				try {
					Main.terminate();
				} catch (IOException | InterruptedException e1) {
					Logger.log(e1, Main.debugLevel);
				}
				break;
			case lmFKey:
				lmState = 1;
				break;
			case lmBKey:
				lmState = -1;
				break;
			case rmFKey:
				rmState = 1;
				break;
			case rmBKey:
				rmState = -1;
				break;
			case FORWARD:
				forwardPressed = true;
				break;
			case BACK:
				backwardPressed = true;
				break;
			case LEFT:
				leftPressed = true;
				break;
			case RIGHT:
				rightPressed = true;
				break;
			case camUp:
				camUpPressed = true;
				camDownPressed = false;
				break;
			case camDown:
				camDownPressed = true;
				camUpPressed = false;
				break;
			case camLeft:
				camLeftPressed = true;
				camRightPressed = false;
				break;
			case camRight:
				camRightPressed = true;
				camLeftPressed = false;
				break;
			case camCenter:
				camCenterPressed = true;
				camRightPressed = false;
				camLeftPressed = false;
				camUpPressed = false;
				camDownPressed = false;
				break;
		}
	}

	@Override
	public void keyReleased(KeyEvent e) {
		int key = e.getKeyCode();
		switch (key) {
			case lmFKey:
				lmState = 0;
				break;
			case lmBKey:
				lmState = 0;
				break;
			case rmFKey:
				rmState = 0;
				break;
			case rmBKey:
				rmState = 0;
				break;
			case FORWARD:
				forwardPressed = false;
				lmState = 0;
				rmState = 0;
				break;
			case BACK:
				backwardPressed = false;
				lmState = 0;
				rmState = 0;
				break;
			case LEFT:
				leftPressed = false;
				lmState = 0;
				rmState = 0;
				break;
			case RIGHT:
				rightPressed = false;
				lmState = 0;
				rmState = 0;
				break;
			case camUp:
				camUpPressed = false;
				break;
			case camDown:
				camDownPressed = false;
				break;
			case camLeft:
				camLeftPressed = false;
				break;
			case camRight:
				camRightPressed = false;
				break;
			case camCenter:
				camCenterPressed = false;
				break;
		}
	}

	@Override
	public void keyTyped(KeyEvent e) {}

	private void motorLogic() {
		if(forwardPressed) {
			if(leftPressed) {
				lmState = 0;
				rmState = 1;
			} else if(rightPressed) {
				lmState = 1;
				rmState = 0;
			} else {
				lmState = 1;
				rmState = 1;
			}
		} else if(backwardPressed) {
			if(leftPressed) {
				lmState = -1;
				rmState = 0;
			} else if(rightPressed) {
				lmState = 0;
				rmState = -1;
			} else {
				lmState = -1;
				rmState = -1;
			}
		} else if(leftPressed) {
			lmState = -1;
			rmState = 1;
		} else if(rightPressed) {
			lmState = 1;
			rmState = -1;
		}
	}
	
	// This will turn the boolean values about the motor states stored in ka into a string.
	public char[] getControlPacket() {
		motorLogic();
		char[] b = new char[7];
		b[0] = (char) (lmState == -1 ? 1 : 0);
		b[1] = (char) (rmState == -1 ? 1 : 0);
		if(lmState != 0) {
			lmPWM = lmPWM+PWMInc > Main.limitPWM ? Main.limitPWM : lmPWM+PWMInc;
		} else {
			lmPWM = lmPWM-PWMInc < 0 ? 0 : lmPWM-PWMInc;
		}
		if(rmState != 0) {
			rmPWM = rmPWM+PWMInc >= Main.limitPWM ? Main.limitPWM : rmPWM+PWMInc;
		} else {
			rmPWM = rmPWM-PWMInc < 0 ? 0 : rmPWM-PWMInc;
		}
		b[2] = (char) lmPWM;
		b[3] = (char) rmPWM;
		if(!camCenterPressed) {
			if(camUpPressed) {
				b[4] = '0';
			} else if(camDownPressed) {
				b[4] = '2';
			} else {
				b[4] = '1';
			}
			if(camRightPressed) {
				b[5] = '2';
			} else if(camLeftPressed) {
				b[5] = '0';
			} else {
				b[5] = '1';
			}
		} else {
			camCenterPressed = false;
			b[4] = '3';
			b[5] = '3';
		}
		b[6] = 13;
		return b;
	}
}
