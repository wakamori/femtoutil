#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

void KGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	emit emitMousePressEvent(event);
}

void KGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	emit emitMouseReleaseEvent(event);
}

void KGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	emit emitMouseMoveEvent(event);
}

//void KGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
//{
//	emit emitDragEnterEvent(event);
//}

void KScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//fprintf(stderr, "KScene::mousePressEvent\n");
	if (ctx != NULL && sfp != NULL && mouse_press_func != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid);
		const knh_ClassTBL_t *ct2 = ClassTBL(mouse_event_cid);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = this;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = event;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (mouse_press_func->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, mouse_press_func->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, mouse_press_func->mtd, 3);
	}
}

void KScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//fprintf(stderr, "KScene::mouseMoveEvent\n");
	if (ctx != NULL && sfp != NULL && mouse_move_func != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid);
		const knh_ClassTBL_t *ct2 = ClassTBL(mouse_event_cid);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = this;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = event;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (mouse_move_func->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, mouse_move_func->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, mouse_move_func->mtd, 3);
	}
}

void KScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	//fprintf(stderr, "KScene::mouseReleaseEvent\n");
	if (ctx != NULL && sfp != NULL && mouse_move_func != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid);
		const knh_ClassTBL_t *ct2 = ClassTBL(mouse_event_cid);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = this;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = event;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (mouse_release_func->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, mouse_release_func->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, mouse_release_func->mtd, 3);
	}
}

KMETHOD Scene_setMousePressEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	s->mouse_press_func = fo;
	s->ctx = (knh_context_t *)ctx;
	s->sfp = sfp;
}

KMETHOD Scene_setMouseReleaseEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	s->mouse_release_func = fo;
	s->ctx = (knh_context_t *)ctx;
	s->sfp = sfp;
}

KMETHOD Scene_setMouseMoveEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	s->mouse_move_func = fo;
	s->ctx = (knh_context_t *)ctx;
	s->sfp = sfp;
}

KMETHOD Scene_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = new KScene();
	s->setClassID(ctx);
	KGraphicsScene* gs = s->gs;
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, s);
	QObject::connect(gs, SIGNAL(emitMousePressEvent(QGraphicsSceneMouseEvent*)),
			s, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*)));
	QObject::connect(gs, SIGNAL(emitMouseMoveEvent(QGraphicsSceneMouseEvent*)),
			s, SLOT(mouseMoveEvent(QGraphicsSceneMouseEvent*)));
	QObject::connect(gs, SIGNAL(emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)),
			s, SLOT(mouseReleaseEvent(QGraphicsSceneMouseEvent*)));
	RETURN_(p);
}

KMETHOD Scene_setItemIndexMethod(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	KGraphicsScene *gs = s->gs;
	QGraphicsScene::ItemIndexMethod idx = Int_to(QGraphicsScene::ItemIndexMethod, sfp[1]);
	gs->setItemIndexMethod(idx);
	RETURNvoid_();
}

KMETHOD Scene_setBackgroundColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	KGraphicsScene *gs = s->gs;
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	gs->setBackgroundBrush(*c);
	RETURNvoid_();
}

KMETHOD Scene_setSceneRect(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	KGraphicsScene *gs = s->gs;
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	int width = Int_to(int, sfp[3]);
	int height = Int_to(int, sfp[4]);
	gs->setSceneRect(x, y, width, height);
	RETURNvoid_();
}

KMETHOD Scene_addItem(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	KGraphicsScene *gs = s->gs;
	QGraphicsItem *i = KITEM_to(sfp[1].p);
	gs->addItem(i);
	RETURNvoid_();
}

KMETHOD Scene_removeItem(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	QGraphicsItem *i = KITEM_to(sfp[1].p);
	s->gs->removeItem(i);
	RETURNvoid_();
}

KMETHOD Scene_addComplexItem(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	KGraphicsScene *gs = s->gs;
	KComplexItem *c = RawPtr_to(KComplexItem *, sfp[1]);
	QList<QGraphicsPolygonItem*> *gp_list = c->gp_list;
	int size = gp_list->size();
	for (int i = 0; i < size; i++) {
		gs->addItem(gp_list->at(i));
	}
	RETURNvoid_();
}

KMETHOD Scene_removeComplexItem(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KScene *s = RawPtr_to(KScene *, sfp[0]);
	KGraphicsScene *gs = s->gs;
	KComplexItem *c = RawPtr_to(KComplexItem *, sfp[1]);
	QList<QGraphicsPolygonItem*> *gp_list = c->gp_list;
	int size = gp_list->size();
	for (int i = 0; i < size; i++) {
		gs->removeItem(gp_list->at(i));
	}
	RETURNvoid_();
}

void KScene::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	knh_ClassTBL_t *mouse_event_ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Scene", S_tochar(cts[i]->sname), sizeof("Scene"))) {
			ct = (knh_ClassTBL_t *)cts[i];
		} else if (!strncmp("MouseEvent", S_tochar(cts[i]->sname), sizeof("MouseEvent"))) {
			mouse_event_ct = (knh_ClassTBL_t *)cts[i];
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Scene\n");
	if (mouse_event_ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: MouseEvent\n");
	cid = ct->cid;
	mouse_event_cid = mouse_event_ct->cid;
}

static void Scene_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Scene:free\n");
#endif
		KScene* s = (KScene*)p->rawptr;
		(void)s;
		//delete s;
	}
}

static void Scene_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Scene:reftrace\n");
#endif
	}
}

DEFAPI(void) defScene(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Scene";
	cdef->free = Scene_free;
	cdef->reftrace = Scene_reftrace;
}

static knh_IntData_t SceneConstInt[] = {
	{"NoIndex", QGraphicsScene::NoIndex},
	{NULL, 0}
};

DEFAPI(void) constScene(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, SceneConstInt);
}

#ifdef __cplusplus
}
#endif
