#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Painter_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPainter *painter = new QPainter();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, painter);
	RETURN_(p);
}

static void Painter_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Painter:free\n");
	QPainter *painter = (QPainter *)p->rawptr;
	delete painter;
}

static void Painter_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Painter:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defPainter(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Painter";
	cdef->free = Painter_free;
	cdef->reftrace = Painter_reftrace;
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
