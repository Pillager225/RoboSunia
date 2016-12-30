package com.HomeGrownProgramming.RoboSunia;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class WebTalker {

//	String hostName = "171.66.76.42";
	String hostName = "192.168.1.232";
    int portNumber = 12345;
    Socket socket;
    PrintWriter out;
    
	public WebTalker() {
		try {
			socket = new Socket(hostName, portNumber);
	        socket.setTcpNoDelay(true);
	        out = new PrintWriter(socket.getOutputStream(), true);
	        System.out.println("Web Talker started.");
		} catch (UnknownHostException e) {
            System.err.println("Don't know about host " + hostName);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " + hostName);
            System.exit(1);
        } 
	}
	
	public void sendByte(byte b) {
		out.println(b);
		System.out.println("Sending " + b);
	}
	
	public void close() throws IOException {
		socket.close();
		out.close();
	}
}
