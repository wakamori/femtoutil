#!/usr/bin/python

import sys, os;

argvs = sys.argv;
argc = len(argvs);
prefix = "";
if argc > 1:
    prefix = argvs[1] + "_";
    
for i in range(7, 16):
    os.system("./calc.k " + prefix + "bt" + str(i) + ".log");
