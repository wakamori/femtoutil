#!/usr/local/bin/konoha

OUT << exec("ssh tsunade@192.168.0.5 ls -l") << EOL;
OUT << exec("ssh tsunade@192.168.0.6 ls -l") << EOL;
OUT << exec("ssh tsunade@192.168.0.10 ls -l") << EOL;
