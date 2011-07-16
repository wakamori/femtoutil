#include <QtGui>
#ifdef K_USING_BOX2D
#include <Box2D.h>
#endif
#ifdef K_USING_OPENCV
#include <cv.h>
#include <highgui.h>
#endif
#include <iostream>
#define K_INTERNAL
#include <konoha1.h>

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

class KPoint {
public:
	int x;
	int y;
	KPoint(int x_, int y_) {
		x = x_;
		y = y_;
	}
};

class KGraphicsLineItem : public QObject, public QGraphicsLineItem {
	Q_OBJECT;
public:
	KGraphicsLineItem() {}
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	//void wheelEvent(QGraphicsSceneWheelEvent *event);
signals:
	void emitMousePressEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void emitDragEnterEvent(QGraphicsSceneDragDropEvent *event);
	//void emitWheelEvent(QGraphicsSceneWheelEvent *event);
};

class KGraphicsScene : public QGraphicsScene {
	Q_OBJECT;
public:
	int n;
	KGraphicsScene() {}
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	//void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	//void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
signals:
	void emitMousePressEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseMoveEvent(QGraphicsSceneMouseEvent *event);
	//void emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	//void emitDragEnterEvent(QGraphicsSceneDragDropEvent *event);
};

class KGraphicsRectItem : public QObject, public QGraphicsRectItem {
	Q_OBJECT;
public:
	KGraphicsRectItem() {}
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
signals:
	void emitMousePressEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void emitDragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void emitWheelEvent(QGraphicsSceneWheelEvent *event);
};

class KGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem {
	Q_OBJECT;
public:
	KGraphicsPixmapItem() {}
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
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
signals:
	void emitMousePressEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void emitMouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void emitDragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void emitWheelEvent(QGraphicsSceneWheelEvent *event);
};
*/

#ifdef K_USING_BOX2D
class KRect;
class KLine;
class KEllipse;
class KTexture;
class KText;
class KComplexItem;

typedef struct _knh_GraphicsUserData_t {
	QObject *o;
	knh_class_t cid;
	size_t *step_count;
} knh_GraphicsUserData_t;

class KContact : public b2ContactListener {
public:
	knh_Func_t *begin;
	knh_Func_t *end;
	knh_context_t *ctx;
	knh_sfp_t *sfp;

	KContact();
	knh_ClassTBL_t *getClassTBL(QString class_name);
	void BeginContact(b2Contact *contact);
	void EndContact(b2Contact *contact);
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
};

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
	QList<KTexture*> *texture_list;
	QList<KText*> *text_list;
	QList<KLine*> *line_list;
	QList<KComplexItem*> *complex_list;
	b2Body *root;
	//JointObjectManagerList *joml;
	b2World *world;
	KContact *contact;
	size_t step_count;
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

class KGraphicsScene;

class KScene : public QObject {
	Q_OBJECT;
public:
	knh_Func_t *mouse_press_func;
	knh_Func_t *mouse_move_func;
	knh_Func_t *mouse_release_func;
	//knh_Func_t *mouse_dragEnter_func;
	knh_class_t cid;
	knh_class_t mouse_event_cid;
	knh_context_t *ctx;
	knh_sfp_t *sfp;
	knh_Method_t *mtd;
	KGraphicsScene *gs;

	KScene() {
		gs = new KGraphicsScene();
		ctx = NULL;
		sfp = NULL;
		mouse_press_func = NULL;
		mouse_move_func = NULL;
		mouse_release_func = NULL;
	}

	void setClassID(CTX ctx);

public slots:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	//void mouseDragEnterEvent(QGraphicsSceneEvent *event);
};

class KLine : public QObject {
	Q_OBJECT;
public:
	QLine *l;
	KGraphicsLineItem *gl;
	QPen *pen;
	bool isDrag;
	int x1;
	int y1;
	int x2;
	int y2;
	int width;
	int height;
	qreal prev_x;
	qreal prev_y;
	qreal dx_sum;
	qreal dy_sum;
	knh_class_t cid;
	QGraphicsDropShadowEffect *se;

#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
	KLine(int x1, int y1, int x2, int y2);
	void setClassID(CTX ctx);
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
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
};

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
	knh_class_t cid;
	knh_class_t mouse_event_cid;
	QGraphicsDropShadowEffect *se;
	knh_Func_t *mouse_press_func;
	knh_Func_t *mouse_move_func;
	knh_Func_t *mouse_release_func;
	knh_context_t *ctx;
	knh_sfp_t *sfp;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
	size_t base_step;
#endif
	KRect(int x, int y, int width, int height);
	void setClassID(CTX ctx);
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
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
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
	knh_class_t cid;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
	
	KEllipse();
	void setRect(KRect *r);
	void setClassID(CTX ctx);
#ifdef K_USING_BOX2D
	void setRotation(qreal rotation_);
	void setDensity(qreal density_);
	void setFriction(qreal friction_);
	void setRestitution(qreal restitution_);
	void addToWorld(KWorld *w);
	void adjust(void);
#endif
};

typedef QList<QList<KPoint*>*> ObjPointList;

class KTexture : public QObject {
	Q_OBJECT;
public:
	QPixmap *p;
	KGraphicsPixmapItem *gp;
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	knh_class_t cid;
	knh_class_t mouse_event_cid;
	QGraphicsColorizeEffect *ce;
	knh_Func_t *mouse_press_func;
	knh_Func_t *mouse_move_func;
	knh_Func_t *mouse_release_func;
	knh_context_t *ctx;
	knh_sfp_t *sfp;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
#ifdef K_USING_OPENCV
	IplImage *ipl;
#endif
	KTexture(const char *filepath);
	KTexture(QImage *image);
	KTexture(QPixmap *image);
	void setTrackData(const char *filepath);
	void setConnect(void);
	QList<KTexture*> *split(CTX ctx, int row, int col);
	void setRect(KRect *r);
	void setColor(QColor *c);
	void setClassID(CTX ctx);
	~KTexture(void);
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

class KComplexItem : public QObject {
	Q_OBJECT;
public:
	QList<QGraphicsPolygonItem*> *gp_list;
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	knh_class_t cid;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
	KComplexItem(knh_Array_t *a);
	void setClassID(CTX ctx);
	void setPosition(int x, int y);
	void setColor(QColor *c);
	~KComplexItem(void);
#ifdef K_USING_BOX2D
	//void setRotation(qreal rotation_);
	void setDensity(qreal density_);
	//void setFriction(qreal friction_);
	//void setRestitution(qreal restitution_);
	void addToWorld(KWorld *w);
	void adjust(void);
#endif
};

class KText : public QObject {
	Q_OBJECT;
public:
	QGraphicsTextItem *gt;
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	knh_class_t cid;
#ifdef K_USING_BOX2D
	bool isStatic;
	qreal rotation;
	b2FixtureDef *shapeDef;
	b2BodyDef *bodyDef;
	b2Body *body;
#endif
	
	KText(QString text);
	void setClassID(CTX ctx);
	void setPosition(int x, int y);
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
	knh_class_t cid;
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
	} else if (name == "KGroup") {
		return (QGraphicsItem *)((KGroup *)o)->g;
	} else if (name == "KEllipse") {
		return (QGraphicsItem *)((KEllipse *)o)->ge;
	} else if (name == "KTexture") {
		return (QGraphicsItem *)((KTexture *)o)->gp;
	} else if (name == "KText") {
		return (QGraphicsItem *)((KText *)o)->gt;
	} else if (name == "KLine") {
		return (QGraphicsItem *)((KLine *)o)->gl;
	} else if (name == "KComplexItem") {
		return (QGraphicsItem *)((KComplexItem *)o)->gp_list;

	} else {
		fprintf(stderr, "CANNNOT CONVERT TO QGraphicsItem\n");
	}
	return NULL;
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

class Vec2f {
public:
	float x;
	float y;
	Vec2f() {
		this->x = 0;
		this->y = 0;
	};
	Vec2f(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

typedef struct {
	Vec2f a;
	Vec2f b;
	Vec2f c;
} Triangle;

extern "C" std::vector<Triangle> triangulate(const std::vector<Vec2f> & points, float resolution);

#define KMETHOD  void  CC_FASTCALL_
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;
