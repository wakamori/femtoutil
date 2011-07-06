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
	r = new KRect(0, 0, 10, 10);
	rect_list = new QList<KRect*>();
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
}
	
void KWorld::addObj(void *obj)
{
	QObject *o = (QObject *)obj;
	QString name = o->objectName();
	if (name == "KRect") {
		//fprintf(stderr, "addAdjustFuncFromObj: this = [%p]\n", ((KRect *)o));
		rect_list->append((KRect *)o);
	} else if ("KGroup") {
		fprintf(stderr, "STILL NOT SUPPORTED\n");
	} else if ("KEllipse") {
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
		int size = rect_list->size();
		for (int i = 0; i < size; i++) {
			KRect *r = rect_list->at(i);
			r->adjust();
		}
	}
	QObject::timerEvent(event);
}

KMETHOD World_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	qsrand(time(0));
	int width = Int_to(int, sfp[1]);
	int height = Int_to(int, sfp[2]);
	KWorld *w = new KWorld(width, height);
	//center_x = width / 2;
	//center_y = height / 2;
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, w);
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

#endif

#ifdef __cplusplus
}
#endif
