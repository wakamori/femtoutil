#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD TimeLine_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTimeLine *t = new QTimeLine();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

KMETHOD TimeLine_setDuration(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTimeLine *t = RawPtr_to(QTimeLine *, sfp[0]);
	int duration = Int_to(int, sfp[1]);
	t->setDuration(duration);
	RETURNvoid_();
}

KMETHOD TimeLine_setCurveShape(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTimeLine *t = RawPtr_to(QTimeLine *, sfp[0]);
	QTimeLine::CurveShape curveshape = Int_to(QTimeLine::CurveShape, sfp[1]);
	t->setCurveShape(curveshape);
	RETURNvoid_();
}

KMETHOD TimeLine_valueChanged(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
}

KMETHOD TimeLine_start(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTimeLine *t = RawPtr_to(QTimeLine *, sfp[0]);
	t->start();
	RETURNvoid_();
}

static void TimeLine_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "TimeLine:free\n");
	QTimeLine *t = (QTimeLine *)p->rawptr;
	delete t;
}

static void TimeLine_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "TimeLine:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defTimeLine(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "TimeLine";
	cdef->free = TimeLine_free;
	cdef->reftrace = TimeLine_reftrace;
}

static knh_IntData_t TimeLineConstInt[] = {
	{"LinearCurve", QTimeLine::LinearCurve},
	{"ValueChanged", 0},
	{NULL, 0}
};

DEFAPI(void) constTimeLine(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
