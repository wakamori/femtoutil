using konoha.clib.*;

libgd = new Clib("/usr/local/lib/libgd.dylib");
class Image;
class Color;

gdg = libgd.genGlue();

Func<Int, Int=>Image> gd_imagecreate = gdg.getFunc("gdImageCreate");
Func<Int, Int, Int => Color> gd_imagecolorallocate = gdg.getFunc("gdImageColorAllocate");
Func<int, int, int, int, Color=>void> gd_imageline = gdg.getFunc("gdImageLine");

Image Image.new (int x, int y) {
		Image ret = gd_imagecreate(x, y);
		return ret;
	}
