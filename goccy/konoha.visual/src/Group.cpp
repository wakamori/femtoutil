#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Group_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGroup *g = new KGroup();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, g);
	RETURN_(p);
}

KMETHOD Group_addToGroup(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGroup *g = RawPtr_to(KGroup *, sfp[0]);
	QGraphicsItem *i = KITEM_to(sfp[1].p);
	g->addToGroup(i);
	RETURNvoid_();
}

static void Group_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Group:free\n");
	KGroup *g = (KGroup *)p->rawptr;
	delete g;
}

static void Group_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Group:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defGroup(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Group";
	cdef->free = Group_free;
	cdef->reftrace = Group_reftrace;
}

DEFAPI(void) constGroup(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
