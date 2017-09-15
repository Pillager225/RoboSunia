package com.HomeGrownProgramming.RoboSunia;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class WebTalker {

    protected Socket socket;
    protected PrintWriter out;
    protected BufferedInputStream in;
    public boolean connected = false;
    
	public WebTalker(String hostName, int portNumber) {
		try {
			socket = new Socket(hostName, portNumber);
	        socket.setTcpNoDelay(true);
	        out = new PrintWriter(socket.getOutputStream(), true);
			in = new BufferedInputStream(socket.getInputStream());
			connected = true;
	        System.out.println("Web Talker started.");
		} catch (UnknownHostException e) {
            System.err.println("Don't know about host " + hostName);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " + hostName);
            System.exit(1);
        } 
	}
	
	public void send(char[] b) {
		out.print(b);
		out.flush();
		System.out.print("Sending ");
		for(int i = 0; i < b.length; i++) {
			System.out.print((int)b[i] + " ");
		}
		System.out.print('\n');
	}
	
	public void send(String b) {
		out.println(b);
		System.out.println("Sending " + b);
	}
	
	public String read() throws IOException {
		String returnString = "";
		int avail = in.available();
		if(avail != 0) {
			byte[] b = new byte[avail];
			in.read(b, 0, avail);
			returnString = new String(b, "UTF-8");
		}
		return returnString;
	}
	
	public void close() throws IOException {
		socket.close();
		out.close();
	}
}
