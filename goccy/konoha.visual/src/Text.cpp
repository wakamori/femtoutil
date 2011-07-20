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
	width = text.size() * 70;
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
	knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	memset(data, 0, sizeof(knh_GraphicsUserData_t));
	data->cid = cid;
	data->o = (QObject *)this;
	body->SetUserData((void *)data);
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

void KText::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Text", S_tochar(cts[i]->sname), sizeof("Text"))) {
			ct = (knh_ClassTBL_t *)cts[i];
			break;
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Text\n");
	cid = ct->cid;
}

KMETHOD Text_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString text = String_to(QString, sfp[1]);
	KText *t = new KText(text);
	t->setClassID(ctx);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[2].p, t);
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

static void Text_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Text:free\n");
#endif
		KText *t = (KText *)p->rawptr;
		(void)t;
		//delete t;
	}
}

static void Text_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Text:reftrace\n");
#endif
	}
}

DEFAPI(void) defText(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Text";
	cdef->free = Text_free;
	cdef->reftrace = Text_reftrace;
}

DEFAPI(void) constText(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
