#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_OPENCV
QImage* IplImage_convertToQImage(const IplImage * iplImage, double mini, double maxi)
{
	int width = iplImage->width;
	// Note here that OpenCV image is stored so that each lined is
	// 32-bits aligned thus * explaining the necessity to "skip"
	// the few last bytes of each line of OpenCV image buffer.
	int widthStep = iplImage->widthStep;
	int height = iplImage->height;
	uchar *qImageBuffer = NULL;
	//memset(qImageBuffer, 0, width * height);
	switch (iplImage->depth) {
	case IPL_DEPTH_8U:
		if (iplImage->nChannels == 1) {
			// IplImage is stored with one byte grey pixel.
			// We convert it to an 8 bit depth QImage.
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uchar *iplImagePtr = (const uchar *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				// Copy line by line
				memcpy(QImagePtr, iplImagePtr, width);
				QImagePtr += width;
				iplImagePtr += widthStep;
			}
		} else if (iplImage->nChannels == 3) {
			// IplImage is stored with 3 byte color pixels (3 channels).
			// We convert it to a 32 bit depth QImage.
			qImageBuffer = (uchar *)malloc(width * height * 4 * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uchar *iplImagePtr = (const uchar *)iplImage->imageData;

			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					// We cannot help but copy manually.
					QImagePtr[0] = iplImagePtr[0];
					QImagePtr[1] = iplImagePtr[1];
					QImagePtr[2] = iplImagePtr[2];
					QImagePtr[3] = 0;
					QImagePtr += 4;
					iplImagePtr += 3;
				}
				iplImagePtr += widthStep-3*width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=8U and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_16U:
		if (iplImage->nChannels == 1) {
			// IplImage is stored with 2 bytes grey pixel.
			// We convert it to an 8 bit depth QImage.
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uint16_t *iplImagePtr = (const uint16_t *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					// We take only the highest part of the 16 bit value.
					// It is similar to dividing by 256.
					*QImagePtr++ = ((*iplImagePtr++) >> 8);
				}
				iplImagePtr += widthStep/sizeof(uint16_t)-width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=16U and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_32F:
		if (iplImage->nChannels == 1) {
			// IplImage is stored with float (4 bytes) grey pixel.
			// We convert it to an 8 bit depth QImage.
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const float *iplImagePtr = (const float *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					uchar p;
					float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
					if (pf < 0) p = 0;
					else if (pf > 255) p = 255;
					else p = (uchar) pf;
					*QImagePtr++ = p;
				}
				iplImagePtr += widthStep/sizeof(float)-width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=32F and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_64F:
		if (iplImage->nChannels == 1) {
			// OpenCV image is stored with double (8 bytes) grey pixel.
			// We convert it to an 8 bit depth QImage.
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const double *iplImagePtr = (const double *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					uchar p;
					double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
					if (pf < 0) p = 0;
					else if (pf > 255) p = 255;
					else p = (uchar) pf;
					*QImagePtr++ = p;
				}
				iplImagePtr += widthStep/sizeof(double)-width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=64F and %d channels\n", iplImage->nChannels);
		}
		break;
	default:
		qDebug("IplImageToQImage: image format is not supported: depth=%d\
					and %d channels\n", iplImage->depth, iplImage->nChannels);
		break;
	}
	QImage *qImage;
	if (iplImage->nChannels == 1) {
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++) {
			colorTable.push_back(qRgb(i, i, i));
		}
		qImage = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
		qImage->setColorTable(colorTable);
	} else {
		qImage = new QImage(qImageBuffer, width, height, QImage::Format_RGB32);
	}
	free(qImageBuffer);
	return qImage;
}

KMETHOD Camera_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int n = Int_to(int, sfp[1]);
	CvCapture *c = cvCaptureFromCAM(n);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[2].p, c);
	//cvNamedWindow("hoge", CV_WINDOW_AUTOSIZE);
	RETURN_(p);
}

KMETHOD Camera_queryFrame(CTX ctx, knh_sfp_t *sfp _RIX)
{
	CvCapture *c = RawPtr_to(CvCapture *, sfp[0]);
	IplImage *frame = cvQueryFrame(c);
	QImage *image = IplImage_convertToQImage(frame, 0.0, 0.0);
	KTexture *t = new KTexture(image);
	t->setClassID(ctx);
	t->ipl = cvCloneImage(frame);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

static void Camera_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Camera:free\n");
#endif
		CvCapture *c = (CvCapture *)p->rawptr;
		(void)c;
	}
}

static void Camera_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
	fprintf(stderr, "Camera:reftrace\n");
#endif
	}
}

DEFAPI(void) defCamera(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Camera";
	cdef->free = Camera_free;
	cdef->reftrace = Camera_reftrace;
}

DEFAPI(void) constCamera(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#endif

#ifdef __cplusplus
}
#endif
