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
	private boolean monitoring;
	private FLLControl control;
	
	public ControlNetworkManager(FLLControl control, int port) {
		this.port = port;
		this.control = control;
		init();
	}
	
	private void init() {
		try {
			serverSocket = new ServerSocket(port);
			sockets = new LinkedList<Socket>();
			outputs = new LinkedList<PrintStream>();
			monitoring = true;
			monitorThread = new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						while(monitoring) {
							Socket skt = serverSocket.accept();
							sockets.add(skt);
							outputs.add(new PrintStream(skt.getOutputStream()));
							sendTeamAndScheduleInfo();
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
		monitoring = false;
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
	
	public void sendTeamAndScheduleInfo() {
		sendCommand("clearSched");
		if(control.teams != null) {
			for(Team t : control.teams.toArray()) {
				sendCommand("addTeam=" + t.number + "," + t.name);
			}
		}
		if(control.schedule != null) {
			String setTablesCMD = "setTables=";
			for(String name : control.schedule.tableNames)
				setTablesCMD += name + ",";
			sendCommand(setTablesCMD);
			sendCommand("setTPT=" + control.schedule.teamsPerTable);
			
			Match[] matches = control.schedule.toArray();
			for(int matchNum = 0; matchNum < matches.length; matchNum++) {
				Match m = matches[matchNum];
				for(int table = 0; table < control.schedule.numTables; table++) {
					if(m.getTeamsOnTable(table) != null) {
						for(Team t : m.getTeamsOnTable(table)) {
							sendCommand("assignMatch=" + matchNum + "," + t.number + "," + table);
						}
					}
				}
			}
		}
	}
	
}
