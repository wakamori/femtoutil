#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Transform_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTransform *t = new QTransform();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

KMETHOD Transform_translate(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTransform *t = RawPtr_to(QTransform *, sfp[0]);
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	t->translate(x, y);
	RETURNvoid_();
}

KMETHOD Transform_rotate(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QTransform *t = RawPtr_to(QTransform *, sfp[0]);
	int angle = Int_to(int, sfp[1]);
	Qt::Axis axis = Int_to(Qt::Axis, sfp[2]);
	t->rotate(angle, axis);
	RETURNvoid_();
}

static void Transform_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Transform:free\n");
	QTransform *t = (QTransform *)p->rawptr;
	delete t;
}

static void Transform_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Transform:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defTransform(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Transform";
	cdef->free = Transform_free;
	cdef->reftrace = Transform_reftrace;
}

DEFAPI(void) constTransform(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
