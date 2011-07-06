#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KText::KText(QString text)
{
	gt = new QGraphicsTextItem();
	gt->setPlainText(text);
	setObjectName("KText");
	isDrag = false;
	//TODO
	width = text.size() * 7;
	height = 10;
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}

void KText::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
}

#ifdef K_USING_BOX2D
void KText::setRotation(qreal rotation_)
{
	rotation = rotation_;
}

void KText::setDensity(qreal density_)
{
	shapeDef->density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}
void KText::setFriction(qreal friction_)
{
	shapeDef->friction = friction_;
}

void KText::setRestitution(qreal restitution_)
{
	shapeDef->restitution = restitution_;
}

void KText::addToWorld(KWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x, -y);
	bodyDef.angle = -(gt->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	b2PolygonShape *shape = new b2PolygonShape();
	shape->SetAsBox(width / 2, height / 2,
					*(new b2Vec2(3 + width / 2, -height / 2 - 5)), 0.0);
	shapeDef->shape = shape;
	//fprintf(stderr, "density = [%f]\n", shapeDef->density);
	body->CreateFixture(shapeDef);
	//fprintf(stderr, "this = [%p]\n", this);
	//fprintf(stderr, "body = [%p]\n", body);
}

void KText::adjust(void)
{
	b2Vec2 position = body->GetPosition();
	qreal angle = body->GetAngle();
	if (isDrag) {
	} else {
		x = position.x;
		y = -position.y;
		//fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
		gt->setPos(x, y);
		gt->setRotation(-(angle * 360.0) / (2 * M_PI));
	}
}
#endif

KMETHOD Text_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString text = String_to(QString, sfp[1]);
	KText *t = new KText(text);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

KMETHOD Text_setPosition(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KText *t = RawPtr_to(KText *, sfp[0]);
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	t->setPosition(x, y);
	RETURNvoid_();
}

KMETHOD Text_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsTextItem *t = (QGraphicsTextItem *)KITEM_to(sfp[0].p);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	(void)t;
	(void)c;
	//t->setBrush(*c);
	RETURNvoid_();
}

#ifdef K_USING_BOX2D
KMETHOD Text_setRotation(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KText *t = RawPtr_to(KText *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	t->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Text_setDensity(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KText *t = RawPtr_to(KText *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	t->setDensity(density);
	RETURNvoid_();
}

KMETHOD Text_setFriction(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KText *t = RawPtr_to(KText *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	t->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Text_setRestitution(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KText *t = RawPtr_to(KText *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	t->setRestitution(restitution);
	RETURNvoid_();
}
#endif

#ifdef __cplusplus
}
#endif
