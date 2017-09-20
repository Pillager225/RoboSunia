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
	        Logger.log("Web Talker started.", Main.debugLevel);
		} catch (UnknownHostException e) {
            Logger.log("\nDon't know about host " + hostName, Main.debugLevel);
            Logger.log(e, Main.debugLevel);
            System.exit(Main.UNKNOWN_HOST);
        } catch (IOException e) {
        	Logger.log("\nCouldn't get I/O for the connection to " + hostName, Main.debugLevel);
            Logger.log(e, Main.debugLevel);
            System.exit(Main.CANT_REACH_HOST);
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
		Logger.log("Sending " + b, Main.debugLevel-1);
	}
	
	public String read() throws IOException {
		String returnString = "";
		int avail = in.available();
		if(avail != 0) {
			byte[] b = new byte[avail];
			in.read(b, 0, avail);
			returnString = new String(b, "UTF-8");
			Logger.log("Received " + returnString, Main.debugLevel-1);
		}
		return returnString;
	}
	
	public void close() throws IOException {
		socket.close();
		out.close();
	}
}
