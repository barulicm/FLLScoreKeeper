import java.io.*;

@SuppressWarnings("unused")
public class StreamListener {
	private InputStream stream;
	private StreamListenerInterface listener;
	private Thread loopThread;
	private BufferedReader reader;
	
	public StreamListener(final InputStream stream, final StreamListenerInterface listener) {
		this.stream = stream;
		this.listener = listener;
		reader = new BufferedReader(new InputStreamReader(stream));
		loopThread = new Thread(new Runnable() {
			
			@Override
			public void run() {
				try {
					String buf = "";
					while(true) {
						buf = reader.readLine();
						if(buf != null) {
							listener.StreamHasDataAvailable(stream, buf);
						}
					}
				} catch (IOException e) {
					System.out.println("Stream listener has failed.");
					e.printStackTrace();
				}
			}
		});
		loopThread.start();
	}
	

	public interface StreamListenerInterface {
		void StreamHasDataAvailable(InputStream stream, String data);
	}
}