package com.HomeGrownProgramming.RoboSunia;

import java.awt.Component;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.util.Hashtable;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class UI extends JFrame {

	public JLabel distanceLabel;
	private static final int numInvisibleButtons = 6;
	
	protected class WindowListener extends WindowAdapter {
		public void windowClosing(WindowEvent e) {
			try {
				Main.terminate();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
		}
	}
	
	public UI(KeyAction ka) {
		setupWindow();
		addKeyListener(ka);
		add(getMainPanel());
		setVisible(true);
	}

	private void setupWindow() {
		setName("RoboSunia Control Window");
		setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		setSize(800,400);
		setLocation(10,30);
		addWindowListener(new WindowListener());
	}
	
	private JPanel getMainPanel() {
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.PAGE_AXIS));
		mainPanel.setFocusable(false);
		mainPanel.add(getLabel("Speed Limit"));
		mainPanel.add(getSpeedSlider());
		mainPanel.add(getLabel("Waiting for first reading"));
		mainPanel.add(getControlsLabels());
		mainPanel.add(getRobotMovementControls());
		return mainPanel;
	}
	
	private JButton createControlButton(String name) {
		JButton button = new JButton(name);
		button.setFocusable(false);
		return button;
	}
	
	private JSlider getSpeedSlider() {
		JSlider speedSlider = new JSlider(JSlider.HORIZONTAL, Main.MIN_PWM, Main.MAX_PWM, Main.limitPWM);
		speedSlider.setMajorTickSpacing(10);
		speedSlider.setMinorTickSpacing(5);
		speedSlider.setPaintTicks(true);
		speedSlider.setFocusable(false);
		Hashtable<Integer, JLabel> labelTable = new Hashtable<Integer, JLabel>();
		labelTable.put( new Integer(Main.MIN_PWM ), new JLabel("Slow") );
		labelTable.put( new Integer(100), new JLabel("Good") );
		labelTable.put( new Integer(2*Main.MAX_PWM/3), new JLabel("Fast") );
		labelTable.put( new Integer(Main.MAX_PWM), new JLabel("Dangerous") );
		speedSlider.setLabelTable( labelTable);
		speedSlider.setPaintLabels(true);
		Font font = new Font("Serif", Font.PLAIN, 15);
		speedSlider.setFont(font);
		speedSlider.addChangeListener(new ChangeListener() {
			@Override
			public void stateChanged(ChangeEvent e) {
				JSlider source = (JSlider)e.getSource();
		        if (!source.getValueIsAdjusting()) {
		        	Main.limitPWM = (int)source.getValue();
		        }   
			}
		});
		return speedSlider;
	}

	private JLabel getLabel(String textToDisplay) {
		JLabel label = new JLabel(textToDisplay, JLabel.CENTER);
		label.setAlignmentX(Component.CENTER_ALIGNMENT);
		label.setAlignmentY(Component.CENTER_ALIGNMENT);
		return label;
	}
	
	private JPanel getControlsLabels() {
		JPanel labels = new JPanel();
		labels.setLayout(new GridLayout(1, 2));
		labels.setFocusable(false);
		labels.add(getLabel("Movement control"));
		labels.add(getLabel("Camera control"));
		return labels;
	}
	
	private JPanel getRobotMovementControls() {
		JPanel controls = new JPanel();
		controls.setLayout(new GridLayout(2, 7));
		controls.setFocusable(false);
		JButton invisButtons[] = new JButton[numInvisibleButtons];
		for(int i = 0; i < numInvisibleButtons; i++) {
			invisButtons[i] = new JButton("Invisible button " + Integer.toString(i));
			invisButtons[i].setFocusable(false);
			invisButtons[i].setVisible(false);
		}
		// first row
		controls.add(invisButtons[0]);
		controls.add(createControlButton("w"));
		controls.add(invisButtons[1]);
		controls.add(invisButtons[2]);
		controls.add(invisButtons[3]);
		controls.add(createControlButton("^"));
		controls.add(invisButtons[4]);
		// second row
		controls.add(createControlButton("a"));
		controls.add(createControlButton("s"));
		controls.add(createControlButton("d"));
		controls.add(invisButtons[5]);
		controls.add(createControlButton("<"));
		controls.add(createControlButton("v"));
		controls.add(createControlButton(">"));
		return controls;
	}
}