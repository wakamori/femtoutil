using konoha.socket.Socket;

void main(String[] args){
	int port = 12345;
	int ch;
	String line = ""; 
	OUT << "port: " + port << EOL;
	ServerSocket ss = new ServerSocket(port, 3);
	while (true){
		Socket connection = ss.accept();
		OUT << "accepted" << EOL;
		i = connection.getInputStream();
		out = connection.getOutputStream();
		if (connection.tellClosed()) {
			OUT << "closed" << EOL;
		}
		else {
			OUT << "connecting" << EOL;
		}
		//while ((ch = i.getChar()) != -1){
		//	line = line.concat("%c"(ch));
		//	out.writeChar(ch);
		//}
		//OUT << line << EOL;
		//out.flush();
		connection.close();
		OUT << "connection closed" << EOL;
		if (connection.tellClosed()) {
			OUT << "closed" << EOL;
		}
		else {
			OUT << "connecting" << EOL;
		}
	}
	ss.close();
}
