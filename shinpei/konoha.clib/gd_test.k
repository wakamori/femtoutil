include "gd.k"

//Image gd = gd_imagecreate(100, 100);
Image gd = new Image(100, 100);
white = gd_imagecolorallocate(255, 255, 255);
black = gd_imagecolorallocate(0, 0, 0);
red = gd_imagecolorallocate(255, 0, 0);
gd_imageline(0, 0, 100, 100, black);
print gd;
