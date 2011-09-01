#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KComplexItem::KComplexItem(knh_Array_t *a)
{
	x = 0;
	y = 0;
	int asize = knh_Array_size(a);
	std::vector<Vec2f> pts;
	for (int i = 0; i < asize; i++) {
		knh_RawPtr_t *o = (knh_RawPtr_t *)a->list[i];
		KPoint *p = (KPoint *)o->rawptr;
		pts.push_back(Vec2f(p->x, p->y));
		delete p;
		//fprintf(stderr, "(x, y) = (%d, %d)\n", p->x, p->y);
	}
	std::vector<Triangle> tris = triangulate(pts, asize);
	gp_list = new QList<QGraphicsPolygonItem *>();
	for (std::vector<Triangle>::iterator triIt = tris.begin(); triIt != tris.end(); ++triIt) {
		QPolygonF p;
		p << QPoint(triIt->a.x, triIt->a.y);
		p << QPoint(triIt->b.x, triIt->b.y);
		p << QPoint(triIt->c.x, triIt->c.y);
		QGraphicsPolygonItem *gp = new QGraphicsPolygonItem();
		gp->setPolygon(p);
		gp->setPen(Qt::NoPen);
		//gp->setBrush(QColor((int)triIt->a.x, (int)triIt->b.x, (int)triIt->c.x));
		gp_list->append(gp);
		//fprintf(stderr, "(%f, %f), (%f, %f), (%f, %f)\n", triIt->a.x, triIt->a.y, triIt->b.x, triIt->b.y, triIt->c.x, triIt->c.y);
	}
	isDrag = false;
	setObjectName("KComplexItem");
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
#ifdef K_USING_OPENCV
	//setTrackData(filepath_);
#endif
}

KComplexItem::~KComplexItem(void)
{
	int gp_length = gp_list->size();
	for (int i = 0; i < gp_length; i++) {
		QGraphicsPolygonItem *gp = gp_list->at(i);
		delete gp;
	}
}

void KComplexItem::addToWorld(KWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x, -y);
	//bodyDef.angle = -(gp_list->at(0)->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	int gp_length = gp_list->size();
	//fprintf(stderr, "length ==%d\n", gp_length);
	for (int i = 0; i < gp_length; i++) {
		QPolygonF poly = gp_list->at(i)->polygon();
		b2Vec2 vers[3];
		const QPointF p0 = poly.at(0);
		const QPointF p1 = poly.at(1);
		const QPointF p2 = poly.at(2);
		vers[2].Set(p0.x(), -p0.y());
		vers[1].Set(p1.x(), -p1.y());
		vers[0].Set(p2.x(), -p2.y());
		b2PolygonShape shape;
		shape.Set(vers, 3);
		shapeDef->shape = &shape;
		body->CreateFixture(shapeDef);
		//fprintf(stderr, "mass == %f\n", body->GetMass());
	}
	knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	memset(data, 0, sizeof(knh_GraphicsUserData_t));
	data->cid = cid;
	data->o = (QObject *)this;
	body->SetUserData((void *)data);
}

void KComplexItem::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("ComplexItem", S_tochar(cts[i]->sname), sizeof("ComplexItem"))) {
			ct = (knh_ClassTBL_t *)cts[i];
			break;
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: ComplexItem\n");
	cid = ct->cid;
}

void KComplexItem::setColor(QColor *c)
{
	int gp_length = gp_list->size();
	for (int i = 0; i < gp_length; i++) {
		QGraphicsPolygonItem *gp = gp_list->at(i);
		gp->setPen(QPen(*c));
		gp->setBrush(*c);
	}
}

void KComplexItem::setDensity(qreal density_)
{
	shapeDef->density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}

void KComplexItem::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
}

void KComplexItem::adjust(void)
{
	b2Vec2 position = body->GetPosition();
	float32 angle = body->GetAngle();
	int gp_length = gp_list->length();
	for (int i = 0; i < gp_length; i++) {
		//fprintf(stderr, "px[%f], py[%f]\n", position.x, position.y);
		QGraphicsPolygonItem *gp = gp_list->at(i);
		gp->setPos(position.x, -position.y);
		gp->setRotation(-(angle * 360.0) / (2 * M_PI));
	}
}


KMETHOD ComplexItem_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	knh_Array_t *a = sfp[1].a;
	KComplexItem *c = new KComplexItem(a);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[2].p, c);
	RETURN_(p);
}

KMETHOD ComplexItem_setPosition(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KComplexItem *c = RawPtr_to(KComplexItem *, sfp[0]);
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	c->setPosition(x, y);
	RETURNvoid_();
}

KMETHOD ComplexItem_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KComplexItem *c = RawPtr_to(KComplexItem *, sfp[0]);
	QColor *clr = RawPtr_to(QColor *, sfp[1]);
	c->setColor(clr);
	RETURNvoid_();
}

#ifdef K_USING_BOX2D

KMETHOD ComplexItem_setDensity(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KComplexItem *c = RawPtr_to(KComplexItem *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	c->setDensity(density);
	RETURNvoid_();
}

#endif

static void ComplexItem_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "ComplexItem:free\n");
#endif
		KComplexItem *t = (KComplexItem *)p->rawptr;
		delete t;
	}
}

static void ComplexItem_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "ComplexItem:reftrace\n");
#endif
	}
}

DEFAPI(void) defComplexItem(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "ComplexItem";
	cdef->free = ComplexItem_free;
	cdef->reftrace = ComplexItem_reftrace;
}

DEFAPI(void) constComplexItem(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
