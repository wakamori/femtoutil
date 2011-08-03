#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef K_USING_BOX2D
KContact::KContact()
{
	begin = NULL;
	end = NULL;
}

void KContact::BeginContact(b2Contact *contact)
{
	//fprintf(stderr, "CONTACT BEGIN\n");
	b2Fixture *a = contact->GetFixtureA();
	b2Fixture *b = contact->GetFixtureB();
	b2Body *abody = a->GetBody();
	b2Body *bbody = b->GetBody();
	knh_GraphicsUserData_t *data1 = (knh_GraphicsUserData_t *)abody->GetUserData();
	knh_GraphicsUserData_t *data2 = (knh_GraphicsUserData_t *)bbody->GetUserData();
	knh_class_t cid1 = data1->cid;
	knh_class_t cid2 = data2->cid;
	QObject *o1 = data1->o;
	QObject *o2 = data2->o;
	if (o1 != NULL && o2 != NULL && begin != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid1);
		const knh_ClassTBL_t *ct2 = ClassTBL(cid2);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = o1;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = o2;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (begin->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, begin->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, begin->mtd, 3);
	}
}

void KContact::EndContact(b2Contact *contact)
{
	//fprintf(stderr, "CONTACT END\n");
	b2Fixture *a = contact->GetFixtureA();
	b2Fixture *b = contact->GetFixtureB();
	b2Body *abody = a->GetBody();
	b2Body *bbody = b->GetBody();
	knh_GraphicsUserData_t *data1 = (knh_GraphicsUserData_t *)abody->GetUserData();
	knh_GraphicsUserData_t *data2 = (knh_GraphicsUserData_t *)bbody->GetUserData();
	knh_class_t cid1 = data1->cid;
	knh_class_t cid2 = data2->cid;
	QObject *o1 = data1->o;
	QObject *o2 = data2->o;
	if (o1 != NULL && o2 != NULL && end != NULL) {
		const knh_ClassTBL_t *ct1 = ClassTBL(cid1);
		const knh_ClassTBL_t *ct2 = ClassTBL(cid2);
		if (ct1 == NULL || ct2 == NULL) return;
		knh_RawPtr_t *p1 = (knh_RawPtr_t*)new_hObject_(ctx, ct1);
		p1->rawptr = o1;
		KNH_SETv(ctx, sfp[5].o, UPCAST(p1));
		knh_RawPtr_t *p2 = (knh_RawPtr_t*)new_hObject_(ctx, ct2);
		p2->rawptr = o2;
		KNH_SETv(ctx, sfp[6].o, UPCAST(p2));
		if (begin->baseNULL != NULL) {
			KNH_SETv(ctx, sfp[K_CALLDELTA].o, end->baseNULL);
		}
		KNH_SCALL(ctx, sfp, 0, end->mtd, 3);
	}
}

void KContact::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
	//fprintf(stderr, "CONTACT PRE SOLVE\n");
	(void)contact;
	(void)oldManifold;
}

void KContact::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
	//fprintf(stderr, "CONTACT POST SOLVE\n");
	(void)contact;
	(void)impulse;
}

static void Contact_free(CTX ctx, knh_RawPtr_t *p)
{
	(void)ctx;
	fprintf(stderr, "Contact:free\n");
	KContact *c = (KContact *)p->rawptr;
	delete c;
}

static void Contact_reftrace(CTX ctx, knh_RawPtr_t *p FTRARG)
{
	(void)ctx;
	(void)p;
	(void)tail_;
	fprintf(stderr, "Contact:reftrace\n");
	//QApplication *app = (QApplication *)p->rawptr;
}

DEFAPI(void) defContact(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Contact";
	cdef->free = Contact_free;
	cdef->reftrace = Contact_reftrace;
}

DEFAPI(void) constContact(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)cid;
	(void)kapi;
	//kapi->loadIntClassConst(ctx, cid, TimeLineConstInt);
}

#endif

#ifdef __cplusplus
}
#endif
