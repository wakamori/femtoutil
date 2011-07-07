include "gd.k"

Image gd = gd_imagecreate(100, 100);
white = gd_imagecolorallocate(gd, 255, 255, 255);
black = gd_imagecolorallocate(gd, 0, 0, 0);
red = gd_imagecolorallocate(gd, 255, 0, 0);
gd_imageline(0, 0, 100, 100, black);
gd_imagegif(gd, 1);

