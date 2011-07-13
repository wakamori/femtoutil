#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

//=========================Presented by Takuma Wakamori====================//
#define Array_size(a) (sizeof(a) / sizeof(a[0]))

typedef struct {
	CvPoint v[3];
} Triangle;

typedef struct {
	Triangle *t;
	int size;
} Triarray;

#define Triarray_size(a) ((a).size)
static CvSubdiv2D *init_delaunay(CvMemStorage *storage, CvRect rect)
{
	CvSubdiv2D* subdiv;
	subdiv = cvCreateSubdiv2D(CV_SEQ_KIND_SUBDIV2D, sizeof(*subdiv),
			sizeof(CvSubdiv2DPoint),
			sizeof(CvQuadEdge2D),
			storage);
	cvInitSubdivDelaunay2D(subdiv, rect);
	return subdiv;
}

static Triangle draw_subdiv_facet(CvSubdiv2DEdge edge, CvRect rect)
{
	CvSubdiv2DEdge t = edge;
	int i, count = 0;
	Triangle buf = {{{0, 0}}};
	do {
		count++;
		t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
	} while (t != edge);
	t = edge;
	for (i = 0; i < count; i++) {
		CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg(t);
		if (!pt) break;
		if (pt->pt.x < rect.x || pt->pt.y < rect.y || pt->pt.x > rect.width || pt->pt.y > rect.height) break;
		buf.v[i] = cvPoint(cvRound(pt->pt.x), cvRound(pt->pt.y));
		t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
	}
	if(i == count) {
		return buf;
	}
	buf.v[0].x = -1;
	return buf;
}

static int Triangle_equals(Triangle t1, Triangle t2)
{
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (t1.v[i].x == t2.v[j].x && t1.v[i].y == t2.v[j].y) {
				break;
			}
		}
		if (j == 3) return 0;
	}
	return 1;
}

static int Array_contains(Triarray tris, Triangle tri)
{
	for (int i = 0; i < Triarray_size(tris); i++) {
		if (Triangle_equals(tris.t[i], tri)) {
			return 1;
		}
	}
	return 0;
}

static void Array_add(Triarray *tris, Triangle tri)
{
	(*tris).t[Triarray_size(*tris)] = tri;
	Triarray_size(*tris) += 1;
}

static Triarray draw_subdiv(CvSubdiv2D *subdiv, int vnum, CvRect rect)
{
	CvSeqReader reader;
	int i, j, total = subdiv->edges->total;
	int elem_size = subdiv->edges->elem_size;
	Triangle buf;
	Triarray tris;
	tris.t = (Triangle *)malloc((total - vnum + 2) * sizeof(Triangle));
	tris.size = 0;
	cvStartReadSeq((CvSeq *)(subdiv->edges), &reader, 0);
	for(i = 0; i < total; i++) {
		CvQuadEdge2D *edge = (CvQuadEdge2D *)(reader.ptr);
		if(CV_IS_SET_ELEM(edge)) {
			CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
			for (j = 0; j < 3; j += 2) {
				buf = draw_subdiv_facet(cvSubdiv2DRotateEdge(e, 0), rect);
				if (buf.v[0].x > 0 && !Array_contains(tris, buf)) {
					Array_add(&tris, buf);
				}
			}
		}
		CV_NEXT_SEQ_ELEM(elem_size, reader);
	}
	return tris;
}

static int getTriangles(CvPoint ***tris, KPoint **pts, int vnum, CvRect rect)
{
	int i;
	CvSubdiv2D *subdiv;
	CvMemStorage *storage = cvCreateMemStorage(0);
	subdiv = init_delaunay(storage, rect);
	for (i = 0; i < vnum; i++) {
		CvPoint2D32f fp = cvPoint2D32f((float)pts[i]->x, (float)pts[i]->y);
		fprintf(stderr, "fp = (%f, %f)\n", fp.x, fp.y);
		if (fp.x < rect.width && fp.y < rect.height) {
			cvSubdivDelaunay2DInsert(subdiv, fp);
		}
	}

	Triarray a = draw_subdiv(subdiv, vnum, rect);
	cvReleaseMemStorage(&storage);
	int retsize = Triarray_size(a);
	(*tris) = (CvPoint **)malloc(Triarray_size(a) * sizeof(CvPoint *));
	for (i = 0; i < Triarray_size(a); i++) {
		(*tris)[i] = (CvPoint *)malloc(3 * sizeof(CvPoint));
		int j;
		for (j = 0; j < 3; j++) {
			(*tris)[i][j] = a.t[i].v[j];
		}
	}
	free(a.t);
	return retsize;
}

KComplexItem::KComplexItem(knh_Array_t *a)
{
	CvPoint **tris = NULL;
	x = 0;
	y = 0;
	CvRect rect = {0, 0, 1300, 800};
	int asize = knh_Array_size(a);
	KPoint *pts[asize];
	for (int i = 0; i < asize; i++) {
		knh_RawPtr_t *o = (knh_RawPtr_t *)a->list[i];
		KPoint *p = (KPoint *)o->rawptr;
		pts[i] = p;
		//fprintf(stderr, "(x, y) = (%d, %d)\n", p->x, p->y);
	}
	int size = getTriangles(&tris, pts, asize, rect);
	fprintf(stderr, "size = [%d]\n", size);
	gp_list = new QList<QGraphicsPolygonItem *>();
	for (int i = 0; i < size; i++) {
		CvPoint *triangle = tris[i];
		QPolygonF p;
		for (int j = 0; j < 3; j++) {
			//fprintf(stderr, "(x, y) = (%d, %d)\n", triangle[j].x, triangle[j].y);
			p << QPoint(triangle[j].x, -triangle[j].y);
		}
		QGraphicsPolygonItem *gp = new QGraphicsPolygonItem();
		gp->setPolygon(p);
		//gp->scale(-1, 1); // mirroring for x-axis
		gp->setBrush(QColor(triangle[0].x, triangle[1].x, triangle[2].x));
		gp_list->append(gp);
		fprintf(stderr, "(%d, %d), (%d, %d), (%d, %d)\n",
			triangle[0].x, triangle[0].y,
			triangle[1].x, triangle[1].y,
			triangle[2].x, triangle[2].y);
		free(tris[i]);
	}
	free(tris);
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
		b2PolygonShape shape;
		const QGraphicsPolygonItem *gp = gp_list->at(i);
		QPolygonF poly = gp->polygon();
		b2Vec2 vers[3];
		const QPointF p0 = poly.at(0);
		float x0 = p0.x(), y0 = -p0.y();
		const QPointF p1 = poly.at(1);
		float x1 = p1.x(), y1 = -p1.y();
		const QPointF p2 = poly.at(2);
		float x2 = p2.x(), y2 = -p2.y();
		if (((x0 * y1 - x1 * y0) + (x1 * y2 - x2 * y1)) > 0) {
			vers[0].Set(x0, y0);
			vers[1].Set(x1, y1);
			vers[2].Set(x2, y2);
		} else {
			vers[0].Set(x0, y0);
			vers[1].Set(x2, y2);
			vers[2].Set(x1, y1);
		}
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
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, c);
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
	fprintf(stderr, "ComplexItem:free\n");
	KComplexItem *t = (KComplexItem *)p->rawptr;
	delete t;
}

static void ComplexItem_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "ComplexItem:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
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
