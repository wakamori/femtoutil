using konoha.clib.*;

Clib libgd = new Clib("/usr/local/lib/libgd.dylib");
class Image;
class Color;

Glue gdg = libgd.genGlue();

Func<int, int => Image> gd_imagecreate = gdg.getFunc("gdImageCreate");
Func<int, int, int => Color> gd_imagecolorallocate = gdg.getFunc("gdImageColorAllocate");
Func<int, int, int, int, Color=>void> gd_imageline = gdg.getFunc("gdImageLine");

/*
Image Image.new (int x, int y, Image _) {
		return gd_imagecreate(x, y);
}

Color Color.new (int r, int g, int b, Color _) {
		return gd_imagecolorallocate(r, g, b);

}
*/
