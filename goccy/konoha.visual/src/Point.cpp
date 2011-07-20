#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Point_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	KPoint *point = new KPoint(x, y);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[3].p, point);
	RETURN_(p);
}

KMETHOD Point_getX(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KPoint *p = RawPtr_to(KPoint *, sfp[0]);
	RETURNi_(p->x);
}

KMETHOD Point_getY(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KPoint *p = RawPtr_to(KPoint *, sfp[0]);
	RETURNi_(p->y);
}

static void Point_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Point:free\n");
#endif
		KPoint *point = (KPoint *)p->rawptr;
		delete point;
	}
}

static void Point_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Point:reftrace\n");
#endif
	}
}

DEFAPI(void) defPoint(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Point";
	cdef->free = Point_free;
	cdef->reftrace = Point_reftrace;
}

DEFAPI(void) constPoint(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
