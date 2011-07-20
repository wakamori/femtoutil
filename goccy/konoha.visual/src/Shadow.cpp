#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Shadow_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsDropShadowEffect *se = new QGraphicsDropShadowEffect();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, se);
	RETURN_(p);
}

static void Shadow_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Shadow:free\n");
#endif
		QGraphicsDropShadowEffect *e = (QGraphicsDropShadowEffect *)p->rawptr;
		delete e;
	}
}

static void Shadow_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Shadow:reftrace\n");
#endif
	}
}

DEFAPI(void) defShadow(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Shadow";
	cdef->free = Shadow_free;
	cdef->reftrace = Shadow_reftrace;
}

DEFAPI(void) constShadow(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
