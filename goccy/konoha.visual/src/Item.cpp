#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Item_translate(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsItem *i = KITEM_to(sfp[0].p);
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	i->translate(x, y);
	RETURNvoid_();
}

KMETHOD Item_setTransform(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsItem *i = KITEM_to(sfp[0].p);
	QTransform *t = RawPtr_to(QTransform *, sfp[1]);
	int b = Boolean_to(int, sfp[2]);
	i->setTransform(*t, b);
	RETURNvoid_();
}

static void Item_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	(void)p;
	fprintf(stderr, "Item:free\n");
	//KItem *i = (KItem *)p->rawptr;
}

static void Item_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Item:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defItem(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Item";
	cdef->free = Item_free;
	cdef->reftrace = Item_reftrace;
}

DEFAPI(void) constItem(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
