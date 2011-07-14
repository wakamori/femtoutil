#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

void KGraphicsLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMousePressEvent(event);
}

void KGraphicsLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseReleaseEvent(event);
}

void KGraphicsLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseMoveEvent(event);
}

void KGraphicsLineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseDoubleClickEvent(event);
}

void KGraphicsLineItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	emit emitDragEnterEvent(event);
}

//void KGraphicsLineItem::wheelEvent(QGraphicsSceneWheelEvent *event)
//{
//	emit emitWheelEvent(event);
//}

void KLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF spos = event->lastScreenPos();
	//fprintf(stderr, "sx: [%f], sy:[%f]\n", spos.x(), spos.y());
	prev_x = spos.x();
	prev_y = spos.y();
	isDrag = true;
	dx_sum = 0;
	dy_sum = 0;
}

void KLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	(void)event;
//#ifdef K_USING_BOX2D
//	QPointF lspos = event->lastScreenPos();
//	//fprintf(stderr, "lsx: [%f], lsy:[%f]\n", lspos.x(), lspos.y());
//	if (!isStatic) {
//		qreal dx = lspos.x() - prev_x;
//		qreal dy = lspos.y() - prev_y;
//		//fprintf(stderr, "dx: [%d], dy:[%d]\n", dx, dy);
//		//fprintf(stderr, "x: [%d], y:[%d]\n", dx + x, dy + y);
//		gl->translate(dx, dy);
//		//const b2Transform t = body->GetTransform();
//		//b2Vec2 position = t.position;
//		//body->SetTransform(*(new b2Vec2(position.x - dx, position.y - dy)), body->GetAngle());
//		dx_sum += dx;
//		dy_sum += dy;
//		//gl->setPos(lspos.x() - prev_x + x, lspos.y() - prev_y + y);
//	}
//	prev_x = lspos.x();
//	prev_y = lspos.y();
//	isDrag = true;
//#endif
}

void KLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	(void)event;
	isDrag = false;
	/*
	const b2Transform t = body->GetTransform();
	b2Vec2 position = t.position;
	fprintf(stderr, "================\n");
	fprintf(stderr, "dx_sum = [%f], dy_sum = [%f]\n", dx_sum, dy_sum);
	//fprintf(stderr, "gx = [%f], gy = [%f]\n", gl->x(), gl->y());
	fprintf(stderr, "pos.x = [%f], -pos.y = [%f]\n", position.x, -position.y);
	//body->SetTransform(*(new b2Vec2(gl->x() - position.x, -gl->y() - -position.y)), angle);
	//body->SetTransform(*(new b2Vec2(x + dx, -dy - y)), angle);
	x = gl->x() + dx_sum;
	y = gl->y() + dy_sum;
	fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
	b2Vec2 pos = body->GetWorldPoint(*(new b2Vec2(x, y)));
	fprintf(stderr, "pos.x = [%f], -pos.y = [%f]\n", pos.x, -pos.y);
	body->SetTransform(*(new b2Vec2(position.x - x, position.y - y)), body->GetAngle());
	*/
}

void KLine::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	fprintf(stderr, "KLine::dragEnterEvent\n");
	(void)event;
	//event->setAccepted(event->mimeData()->hasFormat("text/plain"));
}

void KLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
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

//void KLine::wheelEvent(QGraphicsSceneWheelEvent *event)
//{
//	qreal dx = 0, dy = 0;
//	int delta = event->delta();
//	if (event->orientation() == Qt::Horizontal) {
//		dx = -(qreal)delta;
//	} else {
//		dy = (qreal)delta;
//	}
//	fprintf(stderr, "wheel_event dx: %f, dy: %f\n", dx, dy);
//}


KLine::KLine(int x1_, int y1_, int x2_, int y2_)
{
	x1 = x1_;
	y1 = y1_;
	x2 = x2_;
	y2 = y2_;
	width = ((x1_ - x2_) > 0) ? (x1_ - x2_) : (x2_ - x1_);
	height = ((y1_ - y2_) > 0) ? (y1_ - y2_) : (y2_ - y1_);
	isDrag = false;
	l = new QLine(x1_, y1_, x2_, y2_);
	gl = new KGraphicsLineItem();
	gl->setLine(*l);
	connect(gl, SIGNAL(emitMousePressEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*)));
	connect(gl, SIGNAL(emitMouseMoveEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseMoveEvent(QGraphicsSceneMouseEvent*)));
	connect(gl, SIGNAL(emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseReleaseEvent(QGraphicsSceneMouseEvent*)));
	connect(gl, SIGNAL(emitDragEnterEvent(QGraphicsSceneDragDropEvent*)),
			this, SLOT(dragEnterEvent(QGraphicsSceneDragDropEvent*)));
	setObjectName("KLine");

#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}


#ifdef K_USING_BOX2D
void KLine::setRotation(qreal rotation_)
{
	rotation = rotation_;
}

void KLine::setDensity(qreal density_)
{
	shapeDef->density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}
void KLine::setFriction(qreal friction_)
{
	shapeDef->friction = friction_;
}

void KLine::setRestitution(qreal restitution_)
{
	shapeDef->restitution = restitution_;
}

void KLine::addToWorld(KWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0, 0);
	bodyDef.angle = -(gl->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	b2PolygonShape *shape = new b2PolygonShape();
	//shape->SetAsEdge(*(new b2Vec2(x1, -y1)), *(new b2Vec2(x2, -y2)));
	shape->SetAsBox(width / 2, 1, *(new b2Vec2(x1 + width/2, -y1)), 0.0);
	shapeDef->shape = shape;
	//fprintf(stderr, "density = [%f]\n", shapeDef->density);
	body->CreateFixture(shapeDef);
	knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	memset(data, 0, sizeof(knh_GraphicsUserData_t));
	data->cid = cid;
	data->o = (QObject *)this;
	body->SetUserData((void *)data);
	//fprintf(stderr, "this = [%p]\n", this);
	//fprintf(stderr, "body = [%p]\n", body);
}

void KLine::adjust(void)
{
	b2Vec2 position = body->GetPosition();
	qreal angle = body->GetAngle();
	if (isDrag) {
	} else {
		x1 = position.x;
		y1 = -position.y;
		//fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
		gl->setPos(x1, y1);
		gl->setRotation(-(angle * 360.0) / (2 * M_PI));
	}
}
#endif


void KLine::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Line", S_tochar(cts[i]->sname), sizeof("Line"))) {
			ct = (knh_ClassTBL_t *)cts[i];
			break;
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Line\n");
	cid = ct->cid;
}

KMETHOD Line_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int x1 = Int_to(int, sfp[1]);
	int y1 = Int_to(int, sfp[2]);
	int x2 = Int_to(int, sfp[3]);
	int y2 = Int_to(int, sfp[4]);
	KLine *l = new KLine(x1, y1, x2, y2);
	l->setClassID(ctx);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[5].p, l);
	RETURN_(p);
}

KMETHOD Line_setPen(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsLineItem *l = (KGraphicsLineItem *)KITEM_to(sfp[0].p);
	QPen* pen = RawPtr_to(QPen *, sfp[1]);
	l->setPen(*pen);
	RETURNvoid_();
}

KMETHOD Line_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsLineItem *l = (KGraphicsLineItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	QPen p = l->pen();
	p.setColor(*c);
	l->setPen(p);
	RETURNvoid_();
}

#ifdef K_USING_BOX2D
KMETHOD Line_setRotation(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KLine *r = RawPtr_to(KLine *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	r->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Line_setDensity(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KLine *r = RawPtr_to(KLine *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	r->setDensity(density);
	RETURNvoid_();
}

KMETHOD Line_setFriction(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KLine *r = RawPtr_to(KLine *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	r->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Line_setRestitution(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KLine *r = RawPtr_to(KLine *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	r->setRestitution(restitution);
	RETURNvoid_();
}

KMETHOD Line_isSTatic(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KLine *r = RawPtr_to(KLine *, sfp[0]);
	RETURNb_(r->isStatic);
}

KMETHOD Line_setShadow(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KGraphicsLineItem *gl = (KGraphicsLineItem *)KITEM_to(sfp[0].p);
	QGraphicsDropShadowEffect *se = RawPtr_to(QGraphicsDropShadowEffect *, sfp[1]);
	gl->setGraphicsEffect(se);
	RETURNvoid_();
}

#endif


#ifdef __cplusplus
}
#endif
