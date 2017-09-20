package com.HomeGrownProgramming.RoboSunia;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Logger {
	private static BufferedWriter w;
	private static DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
	private static LocalDateTime now = LocalDateTime.now();
	private static File logFile = new File("RoboSuniaController"+dtf.format(now)+".log");
	@SuppressWarnings("unused")
	private static boolean fileOpened = false;
	
	public static void setLogLocation(String logLocation) {
		logFile = new File(logLocation);
	}
	
	private static void setupFile() throws IOException {
		
		if(!logFile.exists()) {
			logFile.createNewFile();
		}
		FileWriter fw = new FileWriter(logFile, true);
		w = new BufferedWriter(fw);
	}
	
	public static void log(String s, int logLevel) {
		if(logLevel > 0) {
			System.out.println(s);
		} 
		if(logLevel > 2) {
			writeToFile(s);
		}
	}
	
	private static void writeToFile(String s) {
		if(fileOpened = false) {
			try {
				setupFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
			fileOpened = true;
		}
		LocalDateTime now = LocalDateTime.now();
		try {
			w.write(dtf.format(now)+"| " + s + "\n");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
