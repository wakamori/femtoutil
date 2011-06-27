using konoha.clib.*;

p = new Process("top");
g = p.genGlue();
Func<String=>void> f = g.getFunc("");

f("-a");
