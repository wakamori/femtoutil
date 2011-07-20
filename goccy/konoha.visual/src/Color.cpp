#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Color_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString s = String_to(QString, sfp[1]);
	QColor *c = new QColor(s);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[2].p, c);
	RETURN_(p);
}

static void Color_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Color:free\n");
#endif
		QColor *c = (QColor *)p->rawptr;
		delete c;
	}
}

static void Color_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Color:reftrace\n");
#endif
	}
}

DEFAPI(void) defColor(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Color";
	cdef->free = Color_free;
	cdef->reftrace = Color_reftrace;
}

DEFAPI(void) constColor(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
