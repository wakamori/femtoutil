#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Brush_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString s = String_to(QString, sfp[1]);
	QColor *c = new QColor(s);
	QBrush *b = new QBrush();
	b->setColor(*c);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[2].p, b);
	RETURN_(p);
}

static void Brush_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Brush:free\n");
	QBrush *b = (QBrush *)p->rawptr;
	delete b;
}

static void Brush_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Brush:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defBrush(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Brush";
	cdef->free = Brush_free;
	cdef->reftrace = Brush_reftrace;
}

DEFAPI(void) constBrush(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
