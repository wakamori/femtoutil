using konoha.process.*;

Process p = new Process("ls");
Lglue l = p.genGlue();
Func<String=>String> ls = l.getFunc();

s = ls("-la");
OUT <<< s <<< EOL;
