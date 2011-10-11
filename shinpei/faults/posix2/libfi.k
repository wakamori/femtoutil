
void faultinjection(int errno)
{
	ous = new OutputStream("fault.txt");
	ous << errno ;
	ous.flush();
	ous.close();
}

void faultremove() {
	System.unlink(file:fault.txt);
}
