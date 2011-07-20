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
	//fprintf(stderr, "KTexture::mousePressEvent\n");
	isDrag = true;
	if (ctx != NULL && sfp != NULL && mouse_press_func != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid);
		const knh_ClassTBL_t *ct2 = ClassTBL(mouse_event_cid);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = this;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = event;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (mouse_press_func->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, mouse_press_func->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, mouse_press_func->mtd, 3);
	}
}

void KTexture::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//fprintf(stderr, "KTexture::mouseMoveEvent\n");
	isDrag = true;
	if (ctx != NULL && sfp != NULL && mouse_move_func != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid);
		const knh_ClassTBL_t *ct2 = ClassTBL(mouse_event_cid);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = this;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = event;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (mouse_move_func->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, mouse_move_func->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, mouse_move_func->mtd, 3);
	}
}

void KTexture::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	//fprintf(stderr, "KTexture::mouseReleaseEvent\n");
	isDrag = false;
	if (ctx != NULL && sfp != NULL && mouse_release_func != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid);
		const knh_ClassTBL_t *ct2 = ClassTBL(mouse_event_cid);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = this;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = event;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (mouse_release_func->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, mouse_release_func->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, mouse_release_func->mtd, 3);
	}
}

void KTexture::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	fprintf(stderr, "KTexture::dragEnterEvent\n");
	(void)event;
	//event->setAccepted(event->mimeData()->hasFormat("text/plain"));
}

#ifdef K_USING_OPENCV
#include <legacy/legacy.hpp>
typedef struct parameter Parameter;
struct parameter {
	float alpha;
	float beta;
	float gamma;
};
void KTexture::setTrackData(const char *filepath)
{
	Parameter snake_param = {0.45, 0.35, 0.2};
	IplImage *src = cvLoadImage(filepath, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	//cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.7, 0.7);
	CvPoint center;
	center.x = src->width / 2;
	center.y = src->height / 2;
	int length = 50;
	CvPoint *contour = (CvPoint *)cvAlloc(sizeof(CvPoint) * length);
	for (int i = 0; i < length; i++) {
		contour[i].x = (int)(center.x * cos(2 * CV_PI * i / length) + center.x);
		contour[i].y = (int)(center.y * sin(2 * CV_PI * i / length) + center.y);
	}
	cvCvtColor(src, dst, CV_GRAY2RGB);
	for (int i = 0; i < length - 1; i++) {
		cvLine(dst, contour[i], contour[i + 1], CV_RGB(255, 0, 0), 2, 8, 0);
	}
	for (int i = 0; i < 50; i++) {
		cvSnakeImage(src, contour, length, &snake_param.alpha, &snake_param.beta, &snake_param.gamma,
					 CV_VALUE, cvSize(15, 15), cvTermCriteria (CV_TERMCRIT_ITER, 1, 0.0), 1);
		cvCvtColor(src, dst, CV_GRAY2RGB);
		for (int j = 0; j < length - 1; j++) {
			cvLine(dst, contour[j], contour[j + 1], CV_RGB(255, 0, 0), 2);
		}
		cvLine(dst, contour[length - 1], contour[0], CV_RGB(255, 0, 0), 2);
		//cvPutText (dst_img, iter, cvPoint (15, 30), &font, CV_RGB (0, 0, 255));
	}
	for (int i = 0; i < length; i++) {
		fprintf(stderr, "(x, y) = (%d, %d)\n", contour[i].x, contour[i].y);
	}
	//cvNamedWindow ("Snakes", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Snakes", dst);
	//int c = cvWaitKey (0);
}
#endif

KTexture::KTexture(const char *filepath_)
{
	QString filepath(filepath_);
	p = new QPixmap(filepath);
	gp = new KGraphicsPixmapItem();
	gp->setPixmap(*p);
	isDrag = false;
	setObjectName("KTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
	mouse_event_cid = 0;
	mouse_press_func = NULL;
	mouse_release_func = NULL;
	mouse_move_func = NULL;
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
#ifdef K_USING_OPENCV
	//setTrackData(filepath_);
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
	mouse_event_cid = 0;
	mouse_press_func = NULL;
	mouse_release_func = NULL;
	mouse_move_func = NULL;
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
#ifdef K_USING_OPENCV
	//setTrackData();
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
	mouse_event_cid = 0;
	mouse_press_func = NULL;
	mouse_release_func = NULL;
	mouse_move_func = NULL;
#ifdef K_USING_BOX2D
	isStatic = true;
	shapeDef = new b2FixtureDef();
#endif
#ifdef K_USING_OPENCV
	//setTrackData();
#endif
}

KTexture::~KTexture(void)
{
	//delete p;
	//delete gp;
	//delete ce;
#ifdef K_USING_BOX2D
	//delete shapeDef;
	if (bodyDef) {
		//delete bodyDef;
	}
#endif
}

QList<KTexture*> *KTexture::split(CTX ctx, int row, int col)
{
	QList<KTexture*> *list = new QList<KTexture*>();
	QPixmap p = gp->pixmap();
	int width = p.width();
	int height = p.height();
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			QPixmap pixmap = p.copy(width / row * i, height / col * j, width / row, height / col);
			KTexture *t = new KTexture(&pixmap);
			t->setClassID(ctx);
			list->append(t);
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
	knh_ClassTBL_t *mouse_event_ct = NULL;
	const knh_ClassTBL_t **cts = ctx->share->ClassTBL;
	size_t size = ctx->share->sizeClassTBL;
	for (size_t i = 0; i < size; i++) {
		if (!strncmp("Texture", S_tochar(cts[i]->sname), sizeof("Texture"))) {
			ct = (knh_ClassTBL_t *)cts[i];
		} else if (!strncmp("MouseEvent", S_tochar(cts[i]->sname), sizeof("MouseEvent"))) {
			mouse_event_ct = (knh_ClassTBL_t *)cts[i];
		}
	}
	if (ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: Texture\n");
	if (mouse_event_ct == NULL) fprintf(stderr, "ERROR: UNKNOWN CLASS: MouseEvent\n");
	cid = ct->cid;
	mouse_event_cid = mouse_event_ct->cid;
}

void KTexture::setColor(QColor *c)
{
	ce->setColor(*c);
	gp->setGraphicsEffect(ce);
}

KMETHOD Texture_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	const char *filepath = String_to(const char *, sfp[1]);
	KTexture *t = new KTexture(filepath);
	t->setClassID(ctx);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[2].p, t);
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
	QList<KTexture*> *panels = t->split(ctx, row, col);
	int size = row * col;
	knh_Array_t *a = new_Array0(ctx, size);
	for (int i = 0; i < size; i++) {
		knh_Array_add(ctx, a, (Object *)new_RawPtr(ctx, sfp[0].p, panels->at(i)));
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

#ifdef K_USING_OPENCV
static ObjPointList *getDetectObjectPointList(IplImage *src, IplImage *background)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* find_contour = NULL;
	const int w = src->width;
    const int h = src->height;
	IplImage *imgResult = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *hsvImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);
	IplImage *hueImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *saturationImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *valueImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *thresholdImage1 = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *thresholdImage2 = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *thresholdImage3 = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *grayImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
	IplImage *differenceImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
    IplImage *backgroundImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
	//IplImage *prevImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
	//cvCvtColor(src, prevImage,CV_BGR2GRAY);
	cvCvtColor(background, backgroundImage, CV_BGR2GRAY);
	cvCvtColor(src, grayImage, CV_BGR2GRAY);
	//cvAbsDiff(grayImage, prevImage, differenceImage);
	cvAbsDiff(grayImage, backgroundImage, differenceImage);
	cvCvtColor(src, hsvImage, CV_BGR2HSV);
	cvSplit(hsvImage, hueImage, saturationImage, valueImage, NULL);
	cvThreshold(hueImage, thresholdImage1, 0, 255, CV_THRESH_BINARY);
	cvThreshold(hueImage, thresholdImage2, 96, 255, CV_THRESH_BINARY_INV);
	cvAnd(thresholdImage1, thresholdImage2, thresholdImage3, NULL);
	cvAnd(differenceImage, thresholdImage3, imgResult, NULL);
	cvErode(imgResult, imgResult, NULL, 1);
	cvDilate(imgResult, imgResult, NULL, 1);
	//=====================================================================//
	//IplImage *dst = cvCloneImage(imgResult);
	IplImage *tmp_img = cvCreateImage(cvGetSize(imgResult), IPL_DEPTH_8U, 1);
	cvThreshold(imgResult, tmp_img, 40, 255, CV_THRESH_BINARY);
	cvDilate(tmp_img, tmp_img, NULL, 20);
	cvFindContours(tmp_img, storage, &find_contour, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	//fprintf(stderr, "num = [%d]\n", num);
    cvDrawContours(src, find_contour, CV_RGB(255, 0, 0), CV_RGB(255, 0, 0), 1, 2, CV_AA, cvPoint(0, 0));
    //cvShowImage("hoge", src);
	//=====================================================================//
	ObjPointList *objs = new ObjPointList();
	while (1) {
		if (find_contour == NULL) break;
		//fprintf(stderr, "=============START=============\n");
		QList<KPoint *> *obj = new QList<KPoint *>();
		for (int i = 0; i < find_contour->total; i++) {
			CvPoint *point = CV_GET_SEQ_ELEM(CvPoint, find_contour, i);
			KPoint *p = new KPoint(point->x, point->y);
			obj->append(p);
			//fprintf(stderr, "lib:(x, y) = (%d, %d)\n", point->x, point->y);
		}
		//fprintf(stderr, "=============END=============\n");
		objs->append(obj);
		if (find_contour->h_next == NULL) break;
		find_contour = find_contour->h_next;
	}
	cvReleaseImage(&imgResult);
	cvReleaseImage(&hsvImage);
	cvReleaseImage(&hueImage);
	cvReleaseImage(&saturationImage);
	cvReleaseImage(&valueImage);
	cvReleaseImage(&thresholdImage1);
	cvReleaseImage(&thresholdImage2);
	cvReleaseImage(&thresholdImage3);
	cvReleaseImage(&grayImage);
	cvReleaseImage(&differenceImage);
    cvReleaseImage(&backgroundImage);
	cvReleaseImage(&tmp_img);
	cvReleaseMemStorage(&storage);
	return objs;
	//cvShowImage(winNameCapture, img);
	//cvShowImage(winNameResult, dst);
	//cvShowImage(winNameResult, imgResult);
}

KMETHOD Texture_detectHuman(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	KTexture *background = RawPtr_to(KTexture *, sfp[1]);
	ObjPointList *objs = getDetectObjectPointList(t->ipl, background->ipl);
	int objs_size = objs->size();
//fprintf(stderr, "objs_size = [%d]\n", objs_size);
	knh_Array_t *a = new_Array0(ctx, objs_size);
	for (int i = 0; i < objs_size; i++) {
		QList<KPoint *> *obj = objs->at(i);
		int obj_size = obj->size();
		//fprintf(stderr, "obj_size = [%d]\n", obj_size);
		knh_Array_t *elem = new_Array0(ctx, obj_size);
		for (int j = 0; j < obj_size; j++) {
			KPoint *p = obj->at(j);
			//fprintf(stderr, "detectHuman: (x, y) = (%d, %d)\n", p->x, p->y);
			knh_Array_add(ctx, elem, (Object *)new_RawPtr(ctx, sfp[2].p, new KPoint(p->x, p->y)));
			delete p;
		}
		knh_Array_add(ctx, a, (Object *)elem);
        delete obj;
	}
    delete objs;
	RETURN_(a);
}

KMETHOD Texture_setMousePressEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	t->mouse_press_func = fo;
	t->ctx = (knh_context_t *)ctx;
	t->sfp = sfp;
	RETURNvoid_();
}

KMETHOD Texture_setMouseReleaseEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	t->mouse_release_func = fo;
	t->ctx = (knh_context_t *)ctx;
	t->sfp = sfp;
	RETURNvoid_();
}

KMETHOD Texture_setMouseMoveEvent(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	KTexture *t = RawPtr_to(KTexture *, sfp[0]);
	knh_Func_t *fo = sfp[1].fo;
	t->mouse_move_func = fo;
	t->ctx = (knh_context_t *)ctx;
	t->sfp = sfp;
	RETURNvoid_();
}

#endif

static void Texture_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	if (p->rawptr != NULL && O_cTBL(p)->total < 4) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Texture:free\n");
#endif
		KTexture *t = (KTexture *)p->rawptr;
		delete t;
	}
}

static void Texture_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	if (p->rawptr != NULL) {
#ifdef DEBUG_MODE
		fprintf(stderr, "Texture:reftrace\n");
#endif
		//KTexture *t = (KTexture *)p->rawptr;
		//KNH_ADDREF(ctx, t->mouse_press_func);
		//KNH_ADDREF(ctx, t->mouse_move_func);
		//KNH_ADDREF(ctx, t->mouse_release_func);
	}
}

DEFAPI(void) defTexture(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Texture";
	cdef->free = Texture_free;
	cdef->reftrace = Texture_reftrace;
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
