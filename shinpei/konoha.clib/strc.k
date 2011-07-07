using konoha.clib.*;

c_test = new Structure([["test", Structure.FFI_UINT]]);
Clib.defineClass("Test", c_test);
