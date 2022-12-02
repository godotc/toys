import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class EchoClient {

	private final Socket mSocket;

	public EchoClient(String host, int port) throws IOException {
		mSocket = new Socket(host, port);
	}

	public void run() throws IOException {
		// contact
		// Thread readerThread = new Thread(this::readResponse);
		Thread readerThread = new Thread(new Runnable() {
			@Override
			public void run() {
				readResponse();
			}
		});
		readerThread.start();

		OutputStream out = mSocket.getOutputStream();
		byte[] buffer = new byte[1024];
		int n;
		while ((n = System.in.read(buffer)) > 0) {
			out.write(buffer, 0, n);

		}
	}

	private void readResponse() {
		try {
			InputStream in = mSocket.getInputStream();
			;
			byte[] buffer = new byte[1024];
			int n;
			while ((n = in.read(buffer)) > 0) {
				System.out.write(buffer, 0, n);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] argv) {
		try {
			EchoClient client = new EchoClient("localhost", 9527);
			client.run();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
