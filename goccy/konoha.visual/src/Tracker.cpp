#include <visual.hpp>


#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Tracker_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	KTexture *t = RawPtr_to(KTexture *, sfp[1]);
	IplImage *src = t->src;
	int length = Int_to(int, sfp[2]);
	CvPoint center;
	center.x = t->src->width / 2;
	center.y = t->src->height / 2;
	CvPoint *contour = (CvPoint *)cvAlloc(sizeof(CvPoint) * length);
	for (int i = 0; i < length; i++) {
		contour[i].x = (int)(center.x * cos(2 * CV_PI * i / length) + center.x);
		contour[i].y = (int)(center.y * sin(2 * CV_PI * i / length) + center.y);
	}
	
}

KMETHOD Tracker_track(CTX ctx, knh_sfp_t *sfp _RIX)
{
	
	cvSnakeImage(src, contour, length, &snake_param.alpha, &snake_param.beta, &snake_param.gamma,
				 CV_VALUE, cvSize(15, 15), cvTermCriteria (CV_TERMCRIT_ITER, 1, 0.0), 1);

}
