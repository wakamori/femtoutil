using konoha.proc.*;

class MMS
{
	String receiver;
	String to_addr;
	String sender;
	String from_addr;
	String subject;
	String message;

	MMS(String receiver, String to_addr, String sender, String from_addr) {
		_receiver = receiver;
		_to_addr = to_addr;
		_sender = sender;
		_from_addr = from_addr;
		_subject = "default subject";
		_message = "default message";
	}

	boolean send() {
		Proc p = new Proc(["/usr/sbin/sendmail", "-t"]);
		OutputStream ous = p.getOutputStream();
		ous << "To: %s{#0} <%s{#1}>"(receiver, to_addr) << EOL;
		ous << "From: %s{#0} <%s{#1}>"(sender, from_addr) << EOL;
		ous << "Subject: " << subject << EOL;
		ous << "Mime-Version: 1.0" << EOL;
		ous << "Content-Type: text/plain; charset=iso-2022-jp" << EOL;
		ous << "X-Mailer: Sendmail" << EOL << EOL;
		ous << message << EOL;
		ous << "." << EOL;
		ous.flush();
		ous.close();
		return true;
	}
}

using konoha.posix.*;
void main(String[] args)
{
	OUT << "[D-Script] [send_mms] start" << EOL;
	//MMS m = new MMS("wakamori", "wakamori111@gmail.com", "d11gd186", "d11gd186@ynu.ac.jp");
	//m.setSubject("this is a test mail");
	//m.setMessage("hello takuma!");
	//m.send();
	usleep(500000);
	OUT << "[D-Script] [send_mms] successed" << EOL;
	OUT << "[D-Script] [send_mms] endd" << EOL;
}
