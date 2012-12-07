import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.net.Socket;

import javax.swing.JOptionPane;

@SuppressWarnings("unused")
public class DisplayNetworkManager implements StreamListener.StreamListenerInterface {
	private String host;
	private int port;
	private Socket skt;
	private PrintStream output;
	private FLLDisplay display;
	private StreamListener streamListener;
	
	public DisplayNetworkManager(String host, int port, FLLDisplay display) {
		this.host = host;
		this.port = port;
		this.display = display;
		try {
			skt = new Socket(host, port);
			streamListener = new StreamListener(skt.getInputStream(), this);
			output = new PrintStream(skt.getOutputStream());
		} catch (IOException e) {
			System.out.println("Error opening connection to host=" + host + " at port=" + port + ".");
			e.printStackTrace();
			JOptionPane.showMessageDialog(display, "Could not connect to given netork!");
			display.connectToControl();
		}
	}

	@Override
	public void StreamHasDataAvailable(InputStream stream, String data) {
		display.receivedCommandFromControlApp(data);
	}
	
	
}
