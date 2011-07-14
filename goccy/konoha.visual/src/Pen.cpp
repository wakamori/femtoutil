#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Pen_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPen *pen = new QPen();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, pen);
	RETURN_(p);
}

KMETHOD Pen_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPen *pen = RawPtr_to(QPen *, sfp[0]);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	pen->setColor(*c);
	RETURNvoid_();
}

KMETHOD Pen_setWidth(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPen *p = RawPtr_to(QPen *, sfp[0]);
	qreal w = Float_to(qreal, sfp[1]);
	p->setWidthF(w);
	RETURNvoid_();
}

KMETHOD Pen_setStyle(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPen *p = RawPtr_to(QPen *, sfp[0]);
	Qt::PenStyle s = Int_to(Qt::PenStyle, sfp[1]);
	p->setStyle(s);
	RETURNvoid_();
}

KMETHOD Pen_setCapStyle(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPen *p = RawPtr_to(QPen *, sfp[0]);
	Qt::PenCapStyle s = Int_to(Qt::PenCapStyle, sfp[1]);
	p->setCapStyle(s);
	RETURNvoid_();
}

KMETHOD Pen_setJoinStyle(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QPen *p = RawPtr_to(QPen *, sfp[0]);
	Qt::PenJoinStyle s = Int_to(Qt::PenJoinStyle, sfp[1]);
	p->setJoinStyle(s);
	RETURNvoid_();
}

DEFAPI(void) defPen(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Pen";
}

static knh_IntData_t PenConstInt[] = {
	{ "SolidLine",      Qt::SolidLine},
	{ "DashLine",       Qt::DashLine},
	{ "DotLine",        Qt::DotLine},
	{ "DashDotLine",    Qt::DashDotLine},
	{ "DashDotDotLine", Qt::DashDotDotLine},
	{ "CustomDashLine", Qt::CustomDashLine},
	{ "FlatCap",        Qt::FlatCap},
	{ "SquareCap",      Qt::SquareCap},
	{ "RoundCap",       Qt::RoundCap},
	{ "MiterJoin",      Qt::MiterJoin},
	{ "BevelJoin",      Qt::BevelJoin},
	{ "RoundJoin",      Qt::RoundJoin},
	{ "SvgMiterJoin",   Qt::SvgMiterJoin},
	{ NULL, 0}
};

//static knh_IntData_t PenStyleConstInt[] = {
//	{ "SolidLine",      Qt::SolidLine},
//	{ "DashLine",       Qt::DashLine},
//	{ "DotLine",        Qt::DotLine},
//	{ "DashDotLine",    Qt::DashDotLine},
//	{ "DashDotDotLine", Qt::DashDotDotLine},
//	{ "CustomDashLine", Qt::CustomDashLine},
//	{ NULL, 0}
//};
//static knh_IntData_t PenCapStyleConstInt[] = {
//	{ "FlatCap",        Qt::FlatCap},
//	{ "SquareCap",      Qt::SquareCap},
//	{ "RoundCap",       Qt::RoundCap},
//	{ NULL, 0}
//};
//static knh_IntData_t PenJoinStyleConstInt[] = {
//	{ "MiterJoin",      Qt::MiterJoin},
//	{ "BevelJoin",       Qt::BevelJoin},
//	{ "RoundJoin",      Qt::RoundJoin},
//	{ "SvgMiterJoin",   Qt::SvgMiterJoin},
//	{ NULL, 0}
//};

DEFAPI(void) constPen(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, PenConstInt);
}
#ifdef __cplusplus
}
#endif
