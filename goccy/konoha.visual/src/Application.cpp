#include <konoha1.h>
#include <QtGui>
#define KMETHOD  void  CC_FASTCALL_

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD Application_new(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	knh_Array_t *args = (knh_Array_t *)sfp[1].o;
	int args_size = knh_Array_size(args);
#ifdef K_USING_MINGW
	char **value0 = (char **)malloc(args_size * sizeof(char*));
#else
	char **value0 = (char **)alloca(args_size * sizeof(char*));
#endif
	int i = 0;
	for (i = 0; i < args_size; i++) {
		knh_String_t *s = (knh_String_t *)args->list[i];
		value0[i] = (char *)s->str.text;
	}
	QApplication *app = new QApplication(args_size, value0);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, app);
	RETURN_(p);
}

KMETHOD Application_exec(Ctx *ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QApplication *app = RawPtr_to(QApplication *, sfp[0]);
	app->exec();
	RETURNvoid_();
}

#ifdef __cplusplus
}
#endif
