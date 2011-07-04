using konoha.clib.*;

s = new Structure([["a", Structure.FFI_UINT]]);
Clib.defineClass("Test", s);

