#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Timer_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int interval = Int_to(int, sfp[1]);
	//knh_Func_t *fo = (knh_Func_t *)sfp[2].fo;
	knh_Func_t *fo = (knh_Func_t *)malloc(sizeof(knh_Func_t));
	memcpy(fo, sfp[2].fo, sizeof(knh_Func_t));
	KTimer *t = new KTimer(interval, fo);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[3].p, t);
	RETURN_(p);
}

KMETHOD Timer_start(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTimer *t = RawPtr_to(KTimer *, sfp[0]);
	int timer_id = t->start(ctx, sfp);
	RETURNi_(timer_id);
}

static void Timer_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Timer:free\n");
#endif
		KTimer *t = (KTimer *)p->rawptr;
		//fprintf(stderr, "fo->mtd->fcall_1 = [%p]\n", t->fo->mtd->fcall_1);
		(void)t;
		//delete t;
	}
}

static void Timer_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Timer:reftrace\n");
#endif
		KTimer *t = (KTimer *)p->rawptr;
		KNH_ADDREF(ctx, t->fo);
	}
}

DEFAPI(void) defTimer(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Timer";
	cdef->free = Timer_free;
	cdef->reftrace = Timer_reftrace;
}

DEFAPI(void) constTimer(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

static void TimerEvent_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
		fprintf(stderr, "TimerEvent:free\n");
		QTimerEvent *t = (QTimerEvent *)p->rawptr;
		(void)t;
		//delete t;
	}
}

static void TimerEvent_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "TimerEvent:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defTimerEvent(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "TimerEvent";
	cdef->free = TimerEvent_free;
	cdef->reftrace = TimerEvent_reftrace;
}

DEFAPI(void) constTimerEvent(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
