package com.HomeGrownProgramming.RoboSunia;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.IOException;

public class KeyAction implements KeyListener {

	// lm = left motor, rm = right motor, F = forward, B = backward
	public static final int lmFKey = KeyEvent.VK_O, lmBKey = KeyEvent.VK_PERIOD, rmFKey = KeyEvent.VK_P, rmBKey = KeyEvent.VK_SLASH, FORWARD = KeyEvent.VK_UP, LEFT = KeyEvent.VK_LEFT, RIGHT = KeyEvent.VK_RIGHT, BACK = KeyEvent.VK_DOWN;
	public int lmState = 0, rmState = 0;
	public boolean forwardPressed = false, backwardPressed = false, leftPressed = false, rightPressed = false;
	
	@Override
	public void keyPressed(KeyEvent e) {
		if(e.getKeyCode() == KeyEvent.VK_ESCAPE)
			try {
				Main.terminate();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		int key = e.getKeyCode();
		switch (key) {
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
		}
	}

	@Override
	public void keyTyped(KeyEvent e) {
		
	}

}
