#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

Connector::Connector() {}
void Connector::signalConnect(QTimeLine *t, int signal, knh_Func_t *fo_, knh_context_t *ctx_, knh_sfp_t *sfp_)
{
	ctx = ctx_;
	sfp = sfp_;
	fo = fo_;
	(void)signal;
	connect(t, SIGNAL(valueChanged(qreal)), this, SLOT(slot_func(qreal)));
}

void Connector::slot_func(qreal val)
{
	(void)val;
	//knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, val);
	//KNH_SETv(ctx, sfp[5].o, UPCAST(p));
	if (fo->baseNULL != NULL) {
		KNH_SETv(ctx, sfp[K_CALLDELTA].o, (fo)->baseNULL);
	}
	KNH_SCALL(ctx, sfp, 0, fo->mtd, 1);
}

KMETHOD Connector_connect(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	//TODO
	QTimeLine *t = RawPtr_to(QTimeLine *, sfp[1]);
	int signal = Int_to(int, sfp[2]);
	knh_Func_t *fo = sfp[3].fo;
	Connector *c = new Connector();
	c->signalConnect(t, signal, fo, (knh_context_t *)ctx, sfp);
	RETURNvoid_();
}

static void Connector_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Connector:free\n");
	Connector *c = (Connector *)p->rawptr;
	delete c;
}

static void Connector_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Connector:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defConnector(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Connector";
	cdef->free = Connector_free;
	cdef->reftrace = Connector_reftrace;
}

DEFAPI(void) constConnector(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
