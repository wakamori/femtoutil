#include <konoha1.h>
#include <QtGui>
#ifdef K_USING_BOX2D
#include <Box2D.h>
#endif
#include <iostream>
using namespace std;

class Connector : public QObject {
	Q_OBJECT;
public:
	knh_context_t *ctx;
	knh_sfp_t *sfp;
	knh_Func_t *fo;
	Connector();
	void signalConnect(QTimeLine *t, int signal, knh_Func_t *fo, knh_context_t *ctx, knh_sfp_t *sfp);
public slots:
	void slot_func(qreal val);
};

class KGraphicsRectItem : public QObject, public QGraphicsRectItem {
	Q_OBJECT;
public:
	KGraphicsRectItem() {}
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
signals:
	void emitMousePressEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void emitDragEnterEvent(QGraphicsSceneDragDropEvent *event);
};

/*
class KGraphicsEllipseItem : public QObject, public QGraphicsEllipseItem {
	Q_OBJECT;
public:
	KGraphicsEllipseItem() {}
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
signals:
	void emitMousePressEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void emitDragEnterEvent(QGraphicsSceneDragDropEvent *event);
};
*/

#ifdef K_USING_BOX2D
class KRect;
class KEllipse;
class KWorld : public QObject {
public:
	int timer_id;
	int iteration;
	qreal center_x;
	qreal center_y;
	float timestep;
	QGraphicsScene *scene;
	QList<KRect *> *rect_list;
	QList<KEllipse*> *ellipse_list;
	b2Body *root;
	//JointObjectManagerList *joml;
	b2World *world;
	
	KWorld(int width, int height);
	//void addJointObjectManager(JointObjectManager *jom);
	void addObj(void *obj);
    void start(void);
	void joint(void);
	qreal centerX(void);
	qreal centerY(void);
	void timerEvent(QTimerEvent *event);
};
#endif

class KRect : public QObject {
	Q_OBJECT;
public:
	QRect *r;
	KGraphicsRectItem *gr;
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	qreal prev_x;
	qreal prev_y;
	qreal dx_sum;
	qreal dy_sum;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
	KRect(int x, int y, int width, int height);
#ifdef K_USING_BOX2D
	void setRotation(qreal rotation_);
	void setDensity(qreal density_);
	void setFriction(qreal friction_);
	void setRestitution(qreal restitution_);
	void addToWorld(KWorld *w);
	void adjust(void);
#endif
public slots:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
};

class KEllipse : public QObject {
	Q_OBJECT;
public:
	QGraphicsEllipseItem *ge;
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
	
	KEllipse();
	void setRect(KRect *r);
#ifdef K_USING_BOX2D
	void setRotation(qreal rotation_);
	void setDensity(qreal density_);
	void setFriction(qreal friction_);
	void setRestitution(qreal restitution_);
	void addToWorld(KWorld *w);
	void adjust(void);
#endif
};

class KGroup : public QObject {
	Q_OBJECT;
public:
	QGraphicsItemGroup *g;
	
	KGroup() {
		g = new QGraphicsItemGroup();
		setObjectName("KGroup");
	}
	
	void addToGroup(QGraphicsItem *i) {
		g->addToGroup(i);
	}
};

static inline QGraphicsItem *KITEM_to(knh_RawPtr_t *p)
{
	QObject *o = (QObject *)p->rawptr;
	QString name = o->objectName();
	if (name == "KRect") {
		return (QGraphicsItem *)((KRect *)o)->gr;
	} else if ("KGroup") {
		return (QGraphicsItem *)((KGroup *)o)->g;
	} else if ("KEllipse") {
		return (QGraphicsItem *)((KEllipse *)o)->ge;
	}
}

static inline int match(const char *base, const char *target)
{
	int ret = 0;
	if (strlen(base) == strlen(target) &&
		!strncmp(base, target, strlen(target))) {
		ret = 1;
	}
	return ret;
}

class KTimer : public QObject {
public:
	int interval;
	knh_Func_t *fo;
	knh_context_t *ctx;
	knh_sfp_t *sfp;
	knh_Method_t *mtd;
	
	KTimer(int interval_, knh_Func_t *fo_) {
		interval = interval_;
		fo = fo_;
	}
	
	int start(CTX ctx_, knh_sfp_t *sfp_) {
		ctx = (knh_context_t *)ctx_;
		sfp = sfp_;
		int timer_id = startTimer(interval);
		return timer_id;
	}

	void timerEvent(QTimerEvent *event) {
		knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, event);
		KNH_SETv(ctx, sfp[5].o, UPCAST(p));
		if (fo->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, (fo)->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, fo->mtd, 2);
	}
};

#define KMETHOD  void  CC_FASTCALL_
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;
