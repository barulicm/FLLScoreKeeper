import java.net.*;
import java.io.*;
import java.util.LinkedList;
import java.util.List;

public class ControlNetworkManager {
	private ServerSocket serverSocket;
	private List<Socket> sockets;
	private List<PrintStream> outputs;
	private int port;
	private Thread monitorThread;
	
	public ControlNetworkManager(int port) {
		this.port = port;
		init();
	}
	
	private void init() {
		try {
			serverSocket = new ServerSocket(port);
			sockets = new LinkedList<Socket>();
			outputs = new LinkedList<PrintStream>();
			monitorThread = new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						while(true) {
							Socket skt = serverSocket.accept();
							sockets.add(skt);
							outputs.add(new PrintStream(skt.getOutputStream()));
						}
					} catch(IOException e) {
						
					}
				}
			});
			monitorThread.start();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public int getPort() {
		return port;
	}
	
	public void setPort(int port) {
		this.port = port;
		close();
		init();
	}
	
	public void sendCommand(String cmd) {
		for(PrintStream ps : outputs) {
			ps.println(cmd);
		}
	}
	
	public void close() {
		for(Socket s : sockets) {
			try {
				s.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		try {
			serverSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}
