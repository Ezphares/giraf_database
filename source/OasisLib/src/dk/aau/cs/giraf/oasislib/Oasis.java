package dk.aau.cs.giraf.oasislib;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class Oasis {

	public static String callApi(String json)
	{
		
		Socket socket;
		DataInputStream input;
		DataOutputStream output;
		String result;
		
		try {
			socket = new Socket("172.25.26.181", 2468);
		}
		catch (IOException e) {
			System.out.println(e);
			return "Could not connect";
		}
		
		try {
			output = new DataOutputStream(socket.getOutputStream());
			input = new DataInputStream(socket.getInputStream());
		}
		catch (IOException e) {
			System.out.println(e);
			return "Could not get streams";
		}
		
		try {
			output.writeChars(json);
			result = input.readLine();
		}
		catch (IOException e) {
			System.out.println(e);
			return "Could not transfer data";
		}
		
		try {
			input.close();
			socket.close();
		}
		catch (IOException e) {
			System.out.println(e);
		}
		
		
		return result;
	}
}
