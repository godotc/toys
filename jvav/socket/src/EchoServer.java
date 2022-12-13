import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {

	private final ServerSocket mServerSocket;

	public EchoServer(int port) throws IOException {
		mServerSocket = new ServerSocket(port);
	}

	public void run() throws IOException {
		Socket client = mServerSocket.accept();
		handleClient(client);
	}

	private void handleClient(Socket socket) throws IOException {
		// contact
		InputStream in = socket.getInputStream();
		OutputStream out = socket.getOutputStream();

		byte[] buffer = new byte[1024];
		int n;
		while ((n = in.read(buffer)) > 0) {
			out.write(buffer, 0, n);
		}

	}

	public static void main(String[] argv) {
		try {
			EchoServer server = new EchoServer(9527);
			server.run();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
