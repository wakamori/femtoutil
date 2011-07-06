#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KEllipse::KEllipse()
{
	ge = new QGraphicsEllipseItem();
	setObjectName("KEllipse");
	isDrag = false;
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}

void KEllipse::setRotation(qreal rotation_)
{
	rotation = rotation_;
}

void KEllipse::setDensity(qreal density_)
{
	shapeDef->density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}

void KEllipse::setFriction(qreal friction_)
{
	shapeDef->friction = friction_;
}

void KEllipse::setRestitution(qreal restitution_)
{
	shapeDef->restitution = restitution_;
}

void KEllipse::addToWorld(KWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0, 0);
	bodyDef.angle = -(ge->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	b2CircleShape *shape = new b2CircleShape();
	//shape->SetAsBox(width / 2, height / 2,
	//			*(new b2Vec2(x + width / 2, -y - height / 2)), 0.0);
	shape->m_p = *(new b2Vec2(x + width / 2, -y - height / 2));
	shape->m_radius = width / 2;
	shapeDef->shape = shape;
	body->CreateFixture(shapeDef);
}

void KEllipse::adjust(void)
{
	b2Vec2 position = body->GetPosition();
	qreal angle = body->GetAngle();
	if (isDrag) {
		//body->SetXForm(*(new b2Vec2(gr->x(), -gr->y())), angle);
	} else {
		x = position.x;
		y = -position.y;
		//fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
		ge->setPos(x, y);
		ge->setRotation(-(angle * 360.0) / (2 * M_PI));
	}
}

void KEllipse::setRect(KRect *r)
{
	ge->setRect(*r->r);
	x = r->x;
	y = r->y;
	width = r->width;
	height = r->height;
}

KMETHOD Ellipse_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = new KEllipse();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, e);
	RETURN_(p);
}

KMETHOD Ellipse_setRect(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = RawPtr_to(KEllipse *, sfp[0]);
	KRect *r = RawPtr_to(KRect *, sfp[1]);
	e->setRect(r);
	RETURNvoid_();
}

KMETHOD Ellipse_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	e->setBrush(*c);
	RETURNvoid_();
}

KMETHOD Ellipse_setZValue(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	int val = Int_to(int, sfp[1]);
	e->setZValue(val);
	RETURNvoid_();
}

KMETHOD Ellipse_getZValue(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsEllipseItem *e = (QGraphicsEllipseItem *)KITEM_to(sfp[0].p);
	RETURNi_(e->zValue());
}

#ifdef K_USING_BOX2D
KMETHOD Ellipse_setRotation(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = RawPtr_to(KEllipse *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	e->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Ellipse_setDensity(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = RawPtr_to(KEllipse *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	e->setDensity(density);
	RETURNvoid_();
}

KMETHOD Ellipse_setFriction(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = RawPtr_to(KEllipse *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	e->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Ellipse_setRestitution(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = RawPtr_to(KEllipse *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	e->setRestitution(restitution);
	RETURNvoid_();
}
#endif

#ifdef __cplusplus
}
#endif
