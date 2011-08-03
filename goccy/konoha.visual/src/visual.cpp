#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

DEFAPI(void) defVisual(CTX ctx, knh_class_t cid, knh_ClassDef_t *cdef)
{
	NO_WARNING2();
	cdef->name = "Visual";
}

static knh_IntData_t VisualConstInt[] = {
	{"YAxis", Qt::YAxis},
	{NULL, 0}
};

DEFAPI(void) constVisual(CTX ctx, knh_class_t cid, const knh_PackageLoaderAPI_t *kapi)
{
	kapi->loadIntClassConst(ctx, cid, VisualConstInt);
}

DEFAPI(const knh_PackageDef_t*) init(CTX ctx, const knh_PackageLoaderAPI_t *kapi)
{
	(void)ctx;
	(void)kapi;
	RETURN_PKGINFO("konoha.visual");
}

#ifdef __cplusplus
}
#endif
