using konoha.clib.*;

Clib libgd = new Clib("/usr/local/lib/libgd.dylib");
class Image;

Glue gdg = libgd.genGlue();

Func<int, int => Image> gd_imagecreate = gdg.getFunc("gdImageCreate");
Func<Image, int, int, int => int> gd_imagecolorallocate = gdg.getFunc("gdImageColorAllocate");
Func<int, int, int, int, int=>void> gd_imageline = gdg.getFunc("gdImageLine");

Func<Image, int=>void> gd_imagegif = gdg.getFunc("gdImageGif");

/*
Image Image.new (int x, int y, Image _) {
		return gd_imagecreate(x, y);
}

Color Color.new (int r, int g, int b, Color _) {
		return gd_imagecolorallocate(r, g, b);

}
*/
