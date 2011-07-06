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

void KGraphicsRectItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	emit emitDragEnterEvent(event);
}

void KRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF spos = event->lastScreenPos();
	//fprintf(stderr, "sx: [%f], sy:[%f]\n", spos.x(), spos.y());
	prev_x = spos.x();
	prev_y = spos.y();
	isDrag = true;
	dx_sum = 0;
	dy_sum = 0;
}

void KRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	(void)event;
#ifdef K_USING_BOX2D
	QPointF lspos = event->lastScreenPos();
	//fprintf(stderr, "lsx: [%f], lsy:[%f]\n", lspos.x(), lspos.y());
	if (!isStatic) {
		qreal dx = lspos.x() - prev_x;
		qreal dy = lspos.y() - prev_y;
		//fprintf(stderr, "dx: [%d], dy:[%d]\n", dx, dy);
		//fprintf(stderr, "x: [%d], y:[%d]\n", dx + x, dy + y);
		gr->translate(dx, dy);
		//const b2Transform t = body->GetTransform();
		//b2Vec2 position = t.position;
		//body->SetTransform(*(new b2Vec2(position.x - dx, position.y - dy)), body->GetAngle());
		dx_sum += dx;
		dy_sum += dy;
		//gr->setPos(lspos.x() - prev_x + x, lspos.y() - prev_y + y);
	}
	prev_x = lspos.x();
	prev_y = lspos.y();
	isDrag = true;
#endif
}

void KRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	(void)event;
	isDrag = false;
	/*
	const b2Transform t = body->GetTransform();
	b2Vec2 position = t.position;
	fprintf(stderr, "================\n");
	fprintf(stderr, "dx_sum = [%f], dy_sum = [%f]\n", dx_sum, dy_sum);
	//fprintf(stderr, "gx = [%f], gy = [%f]\n", gr->x(), gr->y());
	fprintf(stderr, "pos.x = [%f], -pos.y = [%f]\n", position.x, -position.y);
	//body->SetTransform(*(new b2Vec2(gr->x() - position.x, -gr->y() - -position.y)), angle);
	//body->SetTransform(*(new b2Vec2(x + dx, -dy - y)), angle);
	x = gr->x() + dx_sum;
	y = gr->y() + dy_sum;
	fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
	b2Vec2 pos = body->GetWorldPoint(*(new b2Vec2(x, y)));
	fprintf(stderr, "pos.x = [%f], -pos.y = [%f]\n", pos.x, -pos.y);
	body->SetTransform(*(new b2Vec2(position.x - x, position.y - y)), body->GetAngle());
	*/
}

void KRect::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	fprintf(stderr, "KRect::dragEnterEvent\n");
	(void)event;
	//event->setAccepted(event->mimeData()->hasFormat("text/plain"));
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
	connect(gr, SIGNAL(emitMousePressEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseMoveEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseMoveEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseReleaseEvent(QGraphicsSceneMouseEvent*)));
	connect(gr, SIGNAL(emitDragEnterEvent(QGraphicsSceneDragDropEvent*)),
			this, SLOT(dragEnterEvent(QGraphicsSceneDragDropEvent*)));
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
	shape->SetAsBox(width / 2, height / 2,
					*(new b2Vec2(x + width / 2, -y - height / 2)), 0.0);
	shapeDef->shape = shape;
	//fprintf(stderr, "density = [%f]\n", shapeDef->density);
	body->CreateFixture(shapeDef);
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

KMETHOD Rect_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	int width = Int_to(int, sfp[3]);
	int height = Int_to(int, sfp[4]);
	KRect *r = new KRect(x, y, width, height);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, r);
	RETURN_(p);
}

KMETHOD Rect_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsRectItem *r = (QGraphicsRectItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	r->setBrush(*c);
	RETURNvoid_();
}

#ifdef K_USING_BOX2D
KMETHOD Rect_setRotation(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	r->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Rect_setDensity(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	r->setDensity(density);
	RETURNvoid_();
}

KMETHOD Rect_setFriction(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	r->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Rect_setRestitution(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KRect *r = RawPtr_to(KRect *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	r->setRestitution(restitution);
	RETURNvoid_();
}
#endif

#ifdef __cplusplus
}
#endif
