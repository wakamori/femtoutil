#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

void KGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMousePressEvent(event);
}

void KGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseReleaseEvent(event);
}

void KGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit emitMouseMoveEvent(event);
}

void KGraphicsPixmapItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	emit emitDragEnterEvent(event);
}

void KTexture::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "KTexture::mousePressEvent\n");
	(void)event;
	
}

void KTexture::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "KTexture::mouseMoveEvent\n");
	(void)event;
}

void KTexture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	fprintf(stderr, "KTexture::mouseReleaseEvent\n");
	(void)event;
	isDrag = false;
}

void KTexture::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	fprintf(stderr, "KTexture::dragEnterEvent\n");
	(void)event;
	//event->setAccepted(event->mimeData()->hasFormat("text/plain"));
}

KTexture::KTexture(QString filepath)
{
	p = new QPixmap(filepath);
	gp = new KGraphicsPixmapItem();
	gp->setPixmap(*p);
	isDrag = false;
	setObjectName("KTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}

void KTexture::setConnect(void)
{
	connect(gp, SIGNAL(emitMousePressEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*)));
	connect(gp, SIGNAL(emitMouseMoveEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseMoveEvent(QGraphicsSceneMouseEvent*)));
	connect(gp, SIGNAL(emitMouseReleaseEvent(QGraphicsSceneMouseEvent*)),
			this, SLOT(mouseReleaseEvent(QGraphicsSceneMouseEvent*)));
	connect(gp, SIGNAL(emitDragEnterEvent(QGraphicsSceneDragDropEvent*)),
			this, SLOT(dragEnterEvent(QGraphicsSceneDragDropEvent*)));
}

KTexture::KTexture(QImage *image)
{
	p = new QPixmap();
	QPixmap pixmap = p->fromImage(*image);
	gp = new KGraphicsPixmapItem();
	gp->setPixmap(pixmap);
	//delete p;
	isDrag = false;
	setObjectName("KTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}

KTexture::KTexture(QPixmap *image)
{
	p = NULL;
	gp = new KGraphicsPixmapItem();
	gp->setPixmap(*image);
	isDrag = false;
	setObjectName("KTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
}

KTexture::~KTexture(void)
{
	delete p;
	delete gp;
	delete ce;
#ifdef K_USING_BOX2D
	delete shapeDef;
	if (bodyDef) {
		delete bodyDef;
	}
#endif
}

QList<KTexture*> *KTexture::split(int row, int col)
{
	QList<KTexture*> *list = new QList<KTexture*>();
	QPixmap p = gp->pixmap();
	int width = p.width();
	int height = p.height();
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			QPixmap pixmap = p.copy(width / row * i, height / col * j, width / row, height / col);
			list->append(new KTexture(&pixmap));
		}
	}
	return list;
}

void KTexture::setRect(KRect *r)
{
	x = r->x;
	y = r->y;
	width = r->width;
	height = r->height;
	QPixmap p_;
	if (p == NULL) {
		QPixmap p = gp->pixmap();
		p_ = p.scaled(width, height);
	} else {
		p_ = p->scaled(width, height);
	}
	gp->setPixmap(p_);
	gp->setPos(x, y);
}

#ifdef K_USING_BOX2D
void KTexture::setRotation(qreal rotation_)
{
	rotation = rotation_;
}

void KTexture::setDensity(qreal density_)
{
	shapeDef->density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}

void KTexture::setFriction(qreal friction_)
{
	shapeDef->friction = friction_;
}

void KTexture::setRestitution(qreal restitution_)
{
	shapeDef->restitution = restitution_;
}

void KTexture::addToWorld(KWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x, -y);
	bodyDef.angle = -(gp->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	b2PolygonShape *shape = new b2PolygonShape();
	shape->SetAsBox(width / 2, height / 2,
					*(new b2Vec2(width / 2, -height / 2)), 0.0);
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

void KTexture::adjust(void)
{
	b2Vec2 position = body->GetPosition();
	qreal angle = body->GetAngle();
	if (isDrag) {
	} else {
		x = position.x;
		y = -position.y;
		//fprintf(stderr, "x = [%d], y = [%d]\n", x, y);
		gp->setPos(x, y);
		gp->setRotation(-(angle * 360.0) / (2 * M_PI));
	}
}
#endif

void KTexture::setClassID(CTX ctx)
{
	knh_ClassTBL_t *ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Texture", S_tochar(cts[i]->sname), sizeof("Texture"))) {
			ct = (knh_ClassTBL_t *)cts[i];
			break;
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Texture\n");
	cid = ct->cid;
}

void KTexture::setColor(QColor *c)
{
	ce->setColor(*c);
	gp->setGraphicsEffect(ce);
}

KMETHOD Texture_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString filepath = String_to(QString, sfp[1]);
	KTexture *t = new KTexture(filepath);
	t->setClassID(ctx);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, t);
	RETURN_(p);
}

KMETHOD Texture_setRect(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	KRect *r = RawPtr_to(KRect *, sfp[1]);
	t->setRect(r);
	RETURNvoid_();
}

KMETHOD Texture_setColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	t->setColor(c);
	RETURNvoid_();
}

KMETHOD Texture_split(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	int row = Int_to(int, sfp[1]);
	int col = Int_to(int, sfp[2]);
	QList<KTexture*> *panels = t->split(row, col);
	int size = row * col;
	knh_Array_t *a = new_Array0(ctx, size);
	for (int i = 0; i < size; i++) {
		//row*j + i;
		knh_Array_add(ctx, a, (Object *)new_RawPtr(ctx, sfp[1].p, panels->at(i)));
	}
	RETURN_(a);
}

/*
KMETHOD Texture_setPosition(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	qreal x = Float_to(qreal, sfp[1]);
	qreal y = Float_to(qreal, sfp[2]);
	image->setPosition(x, y);
	RETURNvoid_();
}
*/

#ifdef K_USING_BOX2D
KMETHOD Texture_setDensity(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	qreal density = Float_to(qreal, sfp[1]);
	t->setDensity(density);
	RETURNvoid_();
}

KMETHOD Texture_setFriction(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	qreal friction = Float_to(qreal, sfp[1]);
	t->setFriction(friction);
	RETURNvoid_();
}

KMETHOD Texture_setRotation(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	qreal rotation = Float_to(qreal, sfp[1]);
	t->setRotation(rotation);
	RETURNvoid_();
}

KMETHOD Texture_setRestitution(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	qreal restitution = Float_to(qreal, sfp[1]);
	t->setRestitution(restitution);
	RETURNvoid_();
}
#endif

static void Texture_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Texture:free\n");
	KTexture *t = (KTexture *)p->rawptr;
	delete t;
}

DEFAPI(void) defTexture(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Texture";
	cdef->free = Texture_free;
}

DEFAPI(void) constTexture(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

/*
KMETHOD QPixmap_setSize(Ctx *ctx, knh_sfp_t *sfp, long rix)
{
	KPixmap *image = RawPtr_to(KPixmap *, sfp[0]);
	int width = Int_to(int, sfp[1]);
	int height = Int_to(int, sfp[2]);
	image->setSize(width, height);
	RETURNvoid_();
}
*/
#ifdef __cplusplus
}
#endif
