#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_
#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD Painter_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPainter *painter = new QPainter();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, painter);
	RETURN_(p);
}

DEFAPI(void) defPainter(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Painter";
}

static knh_IntData_t PainterConstInt[] = {
	{"Antialiasing", QPainter::Antialiasing},
	{NULL, 0}
};

DEFAPI(void) constPainter(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, PainterConstInt);
}

#ifdef __cplusplus
}
#endif
