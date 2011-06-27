using konoha.clib.*;
Process pls = new Process("ls");
Glue g = pls.genGlue();
Func<String=>String> ls = g.getFunc("");

ret = ls("-la");
OUT <<< ret <<< EOL;
