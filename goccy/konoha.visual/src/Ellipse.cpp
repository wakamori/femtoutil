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

#ifdef K_USING_BOX2D
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
	knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	memset(data, 0, sizeof(knh_GraphicsUserData_t));
	data->cid = cid;
	data->o = (QObject *)this;
	body->SetUserData((void *)data);
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
#endif

void KEllipse::setRect(KRect *r)
{
	ge->setRect(*r->r);
	x = r->x;
	y = r->y;
	width = r->width;
	height = r->height;
}

void KEllipse::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Ellipse", S_tochar(cts[i]->sname), sizeof("Ellipse"))) {
			ct = (knh_ClassTBL_t *)cts[i];
			break;
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Ellipse\n");
	cid = ct->cid;
}

KMETHOD Ellipse_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KEllipse *e = new KEllipse();
	e->setClassID(ctx);
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

static void Ellipse_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Ellipse:free\n");
#endif
		KEllipse *e = (KEllipse *)p->rawptr;
		(void)e;
		//delete e;
	}
}

static void Ellipse_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Ellipse:reftrace\n");
#endif
	}
}

DEFAPI(void) defEllipse(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Ellipse";
	cdef->free = Ellipse_free;
	cdef->reftrace = Ellipse_reftrace;
}

DEFAPI(void) constEllipse(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
