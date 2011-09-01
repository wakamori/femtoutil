#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

void KGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMousePressEvent(event);
}

void KGraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseReleaseEvent(event);
}

void KGraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseMoveEvent(event);
}

void KGraphicsRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseDoubleClickEvent(event);
}

void KGraphicsRectItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	emit emitDragEnterEvent(event);
}

void KGraphicsRectItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	emit emitWheelEvent(event);
}

void KRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!isStatic) {
		QPointF spos = event->lastScreenPos();
		//fprintf(stderr, "sx: [%f], sy:[%f]\n", spos.x(), spos.y());
		prev_x = spos.x();
		prev_y = spos.y();
		isDrag = true;
		dx_sum = 0;
		dy_sum = 0;
		body->SetAwake(false);
		base_step = *(size_t*)(((knh_GraphicsUserData_t*)(body->GetUserData()))->step_count);
	}
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

void KRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
#ifdef K_USING_BOX2D
	//fprintf(stderr, "lsx: [%f], lsy:[%f]\n", lspos.x(), lspos.y());
	if (!isStatic) {
		QPointF lspos = event->lastScreenPos();
		size_t step_count = *(size_t*)(((knh_GraphicsUserData_t*)(body->GetUserData()))->step_count);
		size_t step_diff = step_count - base_step + 1;
		qreal dx = lspos.x() - prev_x;
		qreal dy = lspos.y() - prev_y;
		//fprintf(stderr, "dx: [%f], dy:[%f]\n", dx, dy);
		dx_sum = (dx_sum + dx) / step_diff;
		dy_sum = (dy_sum + dy) / step_diff;
		gr->setPos(gr->pos() + QPointF(dx, dy));
		body->SetTransform(body->GetPosition() + b2Vec2(dx, -dy), body->GetAngle());
		prev_x = lspos.x();
		prev_y = lspos.y();
		isDrag = true;
		body->SetAwake(false);
		base_step = step_count;
	}
#endif
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

void KRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
#ifdef K_USING_BOX2D
	if (!isStatic) {
		isDrag = false;
		body->SetAwake(true);
		size_t step_count = *(size_t*)(((knh_GraphicsUserData_t*)(body->GetUserData()))->step_count);
		size_t step_release = step_count - base_step + 1;
		qreal force_x = dx_sum / step_release;
		qreal force_y = dy_sum / step_release;
		force_x = ((dx_sum > 0) ? 1 : -1) * force_x * force_x * 10000.0;
		force_y = ((dy_sum > 0) ? -1 : 1) * force_y * force_y * 10000.0;
		body->ApplyForce(b2Vec2(force_x, force_y), body->GetWorldCenter());
		//body->ApplyLinearImpulse(b2Vec2(force_x, force_y), body->GetWorldCenter());
		//body->SetLinearVelocity(b2Vec2(force_x, force_y));
		body->SetAwake(true);
	}
#endif
	if (ctx != NULL && sfp != NULL && mouse_release_func != NULL) {
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

void KRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	(void)event;
#ifdef K_USING_BOX2D
	if (!isStatic) {
		body->SetLinearVelocity(b2Vec2(0, 200.0));
		//body->ApplyForce(b2Vec2(0, 20.0f), body->GetWorldCenter());
		//body->ApplyLinearImpulse(b2Vec2(0, 20.0f), body->GetWorldCenter());
	}
#endif
}

void KRect::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	fprintf(stderr, "KRect::dragEnterEvent\n");
	(void)event;
	//event->setAccepted(event->mimeData()->hasFormat("text/plain"));
}

void KRect::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	qreal dx = 0, dy = 0;
	int delta = event->delta();
	if (event->orientation() == Qt::Horizontal) {
		dx = -(qreal)delta;
	} else {
		dy = (qreal)delta;
	}
	fprintf(stderr, "wheel_event dx: %f, dy: %f\n", dx, dy);
}

KRect::KRect(int x_, int y_, int width_, int height_)
{
	r = new QRect(x_, y_, width_, height_);
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	isDrag = false;
	gr = new KGraphicsRectItem();
	gr->setRect(*r);
	mouse_event_cid = 0;
	mouse_press_func = NULL;
	mouse_release_func = NULL;
	mouse_move_func = NULL;
	connect(gr, SIGNAL(emitMousePressEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseMoveEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseMoveEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseReleaseEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitDragEnterEvent(QGraphicsSceneDragDropEvent*)),
			this, SLOT(dragEnterEvent(QGraphicsSceneDragDropEvent*)));
	connect(gr, SIGNAL(emitWheelEvent(QGraphicsSceneWheelEvent*)),
			this, SLOT(wheelEvent(QGraphicsSceneWheelEvent*)));
	setObjectName("KRect");
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}

#ifdef K_USING_BOX2D
void KRect::setRotation(qreal rotation_)
{
	rotation = rotation_;
}

void KRect::setDensity(qreal density_)
{
	shapeDef->density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}
void KRect::setFriction(qreal friction_)
{
	shapeDef->friction = friction_;
}

void KRect::setRestitution(qreal restitution_)
{
	shapeDef->restitution = restitution_;
}

void KRect::addToWorld(KWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0, 0);
	bodyDef.angle = -(gr->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	b2PolygonShape *shape = new b2PolygonShape();
	shape->SetAsBox(width / 2, height / 2, b2Vec2(x + width / 2, -y - height / 2), 0.0);
	shapeDef->shape = shape;
	//fprintf(stderr, "density = [%f]\n", shapeDef->density);
	body->CreateFixture(shapeDef);
	knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	memset(data, 0, sizeof(knh_GraphicsUserData_t));
	data->cid = cid;
	data->o = (QObject *)this;
	data->step_count = &(w->step_count);
	body->SetUserData((void *)data);
	//fprintf(stderr, "this = [%p]\n", this);
	//fprintf(stderr, "body = [%p]\n", body);
}

void KRect::adjust(void)
{
	b2Vec2 position = body->GetPosition();
	qreal angle = body->GetAngle();
	if (isDrag) {
	} else {
		x = position.x;
		y = -position.y;
		//fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
		gr->setPos(x, y);
		gr->setRotation(-(angle * 360.0) / (2 * M_PI));
	}
}
#endif

void KRect::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	knh_ClassTBL_t *mouse_event_ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Rect", S_tochar(cts[i]->sname), sizeof("Rect"))) {
			ct = (knh_ClassTBL_t *)cts[i];
		} else if (!strncmp("MouseEvent", S_tochar(cts[i]->sname), sizeof("MouseEvent"))) {
			mouse_event_ct = (knh_ClassTBL_t *)cts[i];
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Rect\n");
	if (mouse_event_ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: MouseEvent\n");
	cid = ct->cid;
	mouse_event_cid = mouse_event_ct->cid;
}

KMETHOD Rect_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	int width = Int_to(int, sfp[3]);
	int height = Int_to(int, sfp[4]);
	KRect *r = new KRect(x, y, width, height);
	r->setClassID(ctx);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[5].p, r);
	RETURN_(p);
}

KMETHOD Rect_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsRectItem *r = (KGraphicsRectItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	r->setBrush(*c);
	RETURNvoid_();
}

KMETHOD Rect_setZValue(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsRectItem *r = (KGraphicsRectItem *)KITEM_to(sfp[0].p);
	int val = Int_to(int, sfp[1]);
	r->setZValue(val);
	RETURNvoid_();
}

#ifdef K_USING_BOX2D
KMETHOD Rect_setRotation(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	r->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Rect_setDensity(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	r->setDensity(density);
	RETURNvoid_();
}

KMETHOD Rect_setFriction(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	r->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Rect_setRestitution(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	r->setRestitution(restitution);
	RETURNvoid_();
}

KMETHOD Rect_isSTatic(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	RETURNb_(r->isStatic);
}

KMETHOD Rect_setShadow(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsRectItem *gr = (KGraphicsRectItem *)KITEM_to(sfp[0].p);
	QGraphicsDropShadowEffect *se = RawPtr_to(QGraphicsDropShadowEffect *, sfp[1]);
	gr->setGraphicsEffect(se);
	RETURNvoid_();
}

KMETHOD Rect_setTexture(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsRectItem *gr = (KGraphicsRectItem *)KITEM_to(sfp[0].p);
	KTexture *t = RawPtr_to(KTexture *, sfp[1]);
	KGraphicsPixmapItem *gp = t->gp;
	QPixmap p = gp->pixmap();
	QBrush b(p);
	gr->setBrush(b);
	RETURNvoid_();
}

KMETHOD Rect_setMousePressEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	r->mouse_press_func = fo;
	r->ctx = (knh_context_t *)ctx;
	r->sfp = sfp;
	RETURNvoid_();
}

KMETHOD Rect_setMouseReleaseEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	r->mouse_release_func = fo;
	r->ctx = (knh_context_t *)ctx;
	r->sfp = sfp;
	RETURNvoid_();
}

KMETHOD Rect_setMouseMoveEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	r->mouse_move_func = fo;
	r->ctx = (knh_context_t *)ctx;
	r->sfp = sfp;
	RETURNvoid_();
}

#endif

static void Rect_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Rect:free\n");
#endif
		KRect *r = (KRect *)p->rawptr;
		delete r;
	}
}

static void Rect_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Rect:reftrace\n");
#endif
	}
}

DEFAPI(void) defRect(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Rect";
	cdef->free = Rect_free;
	cdef->reftrace = Rect_reftrace;
}

DEFAPI(void) constRect(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
