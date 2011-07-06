#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD Scene_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsScene *s = new QGraphicsScene();
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, s);
	RETURN_(p);
}

KMETHOD Scene_setItemIndexMethod(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsScene *s = RawPtr_to(QGraphicsScene *, sfp[0]);
	QGraphicsScene::ItemIndexMethod idx = Int_to(QGraphicsScene::ItemIndexMethod, sfp[1]);
	s->setItemIndexMethod(idx);
	RETURNvoid_();
}

KMETHOD Scene_setBackgroundColor(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsScene *s = RawPtr_to(QGraphicsScene *, sfp[0]);
	QColor *c = RawPtr_to(QColor *, sfp[1]);
	s->setBackgroundBrush(*c);
	RETURNvoid_();
}

KMETHOD Scene_setSceneRect(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsScene *s = RawPtr_to(QGraphicsScene *, sfp[0]);
	int x = Int_to(int, sfp[1]);
	int y = Int_to(int, sfp[2]);
	int width = Int_to(int, sfp[3]);
	int height = Int_to(int, sfp[4]);
	s->setSceneRect(x, y, width, height);
	RETURNvoid_();
}

KMETHOD Scene_addItem(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QGraphicsScene *s = RawPtr_to(QGraphicsScene *, sfp[0]);
	QGraphicsItem *i = KITEM_to(sfp[1].p);
	s->addItem(i);
	RETURNvoid_();
}

DEFAPI(void) defScene(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Scene";
}

static knh_IntData_t SceneConstInt[] = {
	{"NoIndex", QGraphicsScene::NoIndex},
	{NULL, 0}
};

DEFAPI(void) constScene(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, SceneConstInt);
}

#ifdef __cplusplus
}
#endif
