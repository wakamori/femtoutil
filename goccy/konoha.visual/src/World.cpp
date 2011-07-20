#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_BOX2D

KWorld::KWorld(int width, int height)
{
	//b2AABB worldAABB;
	//worldAABB.lowerBound.Set(-width, -height);
	//worldAABB.upperBound.Set(width, height);
	//world = new b2World(worldAABB, b2Vec2(0.0f, -10.0f), true);
	rect_list = new QList<KRect*>();
	ellipse_list = new QList<KEllipse*>();
	texture_list = new QList<KTexture*>();
	text_list = new QList<KText*>();
	line_list = new QList<KLine*>();
	complex_list = new QList<KComplexItem*>();
	world = new b2World(b2Vec2(0.0f, -10.0f), true);
	//scene = new QGraphicsScene();
	//scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	//scene->setBackgroundBrush(Qt::white);
	//scene->setSceneRect(0, 0, width, height);
	iteration = 10;
	timestep = 1.0f / 30.0f;
	timer_id = 0;
	//joml = new JointObjectManagerList();
	center_x = width / 2;
	center_y = height / 2;
	step_count = 0;
	contact = new KContact();
	world->SetContactListener(contact);
}
	
void KWorld::addObj(void *obj)
{
	QObject *o = (QObject *)obj;
	QString name = o->objectName();
	if (name == "KRect") {
		//fprintf(stderr, "addAdjustFuncFromObj: this = [%p]\n", ((KRect *)o));
		rect_list->append((KRect *)o);
	} else if (name == "KEllipse") {
		ellipse_list->append((KEllipse *)o);
	} else if (name == "KTexture") {
		texture_list->append((KTexture *)o);
	} else if (name == "KText") {
		text_list->append((KText *)o);
	} else if (name == "KLine") {
		line_list->append((KLine *)o);
	} else if (name == "KGroup") {
		
	} else if (name == "KComplexItem") {
		complex_list->append((KComplexItem *)o);
	} else {
		fprintf(stderr, "STILL NOT SUPPORTED\n");
	}
}

/*
void KWorld::addJointObjectManager(JointObjectManager *jom)
{
	joml->addObj(jom);
}
*/
void KWorld::start()
{
	//joint object by using JointObjectManager
	//if (joml != NULL) {
	//joint();
	//}
	if (!timer_id) {
		timer_id = startTimer(1000 / 60.0);
	}
}
/*
void KWorld::joint()
{
	JointObjectManagerList *l = joml->_root;
	while (l->_next != NULL) {
		l = l->_next;
		JointObjectManager *jom = l->jom;
		jom->addBodyList(bodys);
		jom->joint(world);
	}
}
*/
qreal KWorld::centerX(void)
{
	return center_x;
}

qreal KWorld::centerY(void) {
	return center_y;
}

void KWorld::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timer_id) {
		world->Step(timestep, iteration, iteration);
		int rect_list_size = rect_list->size();
		int ellipse_list_size = ellipse_list->size();
		int texture_list_size = texture_list->size();
		int text_list_size = text_list->size();
		int line_list_size = line_list->size();
		int complex_list_size = complex_list->size();
		for (int i = 0; i < rect_list_size; i++) {
			KRect *r = rect_list->at(i);
			r->adjust();
		}
		for (int i = 0; i < ellipse_list_size; i++) {
			KEllipse *e = ellipse_list->at(i);
			e->adjust();
		}
		for (int i = 0; i < texture_list_size; i++) {
			KTexture *t = texture_list->at(i);
			t->adjust();
		}
		for (int i = 0; i < text_list_size; i++) {
			KText *t = text_list->at(i);
			t->adjust();
		}
		for (int i = 0; i < line_list_size; i++) {
			KLine *l = line_list->at(i);
			l->adjust();
		}
		for (int i = 0; i < complex_list_size; i++) {
			KComplexItem *c = complex_list->at(i);
			c->adjust();
		}
	}
	step_count++;
	QObject::timerEvent(event);
}

KMETHOD World_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	qsrand(time(0));
	int width = Int_to(int, sfp[1]);
	int height = Int_to(int, sfp[2]);
	KWorld *w = new KWorld(width, height);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[3].p, w);
	RETURN_(p);
}

KMETHOD World_add(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KWorld *world = RawPtr_to(KWorld *, sfp[0]);
	QObject *o = RawPtr_to(QObject *, sfp[1]);
	QString name = o->objectName();
	if (name == "KRect") {
		KRect *r = RawPtr_to(KRect *, sfp[1]);
		r->addToWorld(world);
		world->addObj(r);
	} else if (name == "KEllipse") {
		KEllipse *e = RawPtr_to(KEllipse *, sfp[1]);
		e->addToWorld(world);
		world->addObj(e);
	} else if (name == "KTexture") {
		KTexture *t = RawPtr_to(KTexture *, sfp[1]);
		t->addToWorld(world);
		world->addObj(t);
	} else if (name == "KText") {
		KText *t = RawPtr_to(KText *, sfp[1]);
		t->addToWorld(world);
		world->addObj(t);
	} else if (name == "KLine") {
		KLine *l = RawPtr_to(KLine *, sfp[1]);
		l->addToWorld(world);
		world->addObj(l);
	} else if (name == "KComplexItem") {
		KComplexItem *c = RawPtr_to(KComplexItem *, sfp[1]);
		c->addToWorld(world);
		world->addObj(c);
	} else {
		fprintf(stderr, "World: [WARNING] UNNOWN OBJECT\n");
	}
	RETURNvoid_();
}

KMETHOD World_remove(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KWorld *w = RawPtr_to(KWorld *, sfp[0]);
	QObject *o = RawPtr_to(QObject *, sfp[1]);
	QString name = o->objectName();
	b2World *world = w->world;
	if (name == "KRect") {
		KRect *r = RawPtr_to(KRect *, sfp[1]);
		QList<KRect *> *rect_list = w->rect_list;
		b2Body *body = r->body;
		rect_list->removeOne(r);
		world->DestroyBody(body);
	} else if (name == "KEllipse") {
		KEllipse *e = RawPtr_to(KEllipse *, sfp[1]);
		QList<KEllipse *> *ellipse_list = w->ellipse_list;
		b2Body *body = e->body;
		ellipse_list->removeOne(e);
		world->DestroyBody(body);
	} else if (name == "KTexture") {
		KTexture *t = RawPtr_to(KTexture *, sfp[1]);
		QList<KTexture *> *texture_list = w->texture_list;
		b2Body *body = t->body;
		texture_list->removeOne(t);
		world->DestroyBody(body);
	} else if (name == "KText") {
		KText *t = RawPtr_to(KText *, sfp[1]);
		QList<KText *> *text_list = w->text_list;
		b2Body *body = t->body;
		text_list->removeOne(t);
		world->DestroyBody(body);
	} else if (name == "KLine") {
		KLine *l = RawPtr_to(KLine *, sfp[1]);
		QList<KLine *> *line_list = w->line_list;
		b2Body *body = l->body;
		line_list->removeOne(l);
		world->DestroyBody(body);
	} else if (name == "KComplexItem") {
		KComplexItem *c = RawPtr_to(KComplexItem *, sfp[1]);
		QList<KComplexItem *> *complex_list = w->complex_list;
		b2Body *body = c->body;
		complex_list->removeOne(c);
		world->DestroyBody(body);
	} else {
		fprintf(stderr, "World: [WARNING] UNNOWN OBJECT\n");
	}
	RETURNvoid_();
}

KMETHOD World_start(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KWorld *w = RawPtr_to(KWorld *, sfp[0]);
	w->start();
	RETURNvoid_();
}

KMETHOD World_setGravity(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KWorld *w = RawPtr_to(KWorld *, sfp[0]);
	float x = Float_to(float, sfp[1]);
	float y = Float_to(float, sfp[2]);
	if (w->world != NULL) {
		w->world->SetGravity(b2Vec2(x, y));
	}
	RETURNvoid_();
}

KMETHOD World_setBeginContactEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KWorld *w = RawPtr_to(KWorld *, sfp[0]);
	knh_Func_t *f = sfp[1].fo;
	w->contact->begin = f;
	w->contact->ctx = (knh_context_t *)ctx;
	w->contact->sfp = sfp;
	RETURNvoid_();
}

KMETHOD World_setEndContactEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KWorld *w = RawPtr_to(KWorld *, sfp[0]);
	knh_Func_t *f = sfp[1].fo;
	w->contact->end = f;
	w->contact->ctx = (knh_context_t *)ctx;
	w->contact->sfp = sfp;
	RETURNvoid_();
}

#endif

static void World_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "World:free\n");
#endif
		KWorld *w = (KWorld *)p->rawptr;
		delete w;
	}
}

static void World_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "World:reftrace\n");
#endif
	}
}

DEFAPI(void) defWorld(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "World";
	cdef->free = World_free;
	cdef->reftrace = World_reftrace;
}

DEFAPI(void) constWorld(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#ifdef __cplusplus
}
#endif
