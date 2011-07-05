#include <konoha1.h>
#include <QtGui>
#include <visual.hpp>
#define KMETHOD  void  CC_FASTCALL_

#ifdef __cplusplus
extern "C" {
#endif
#define NO_WARNING() (void)ctx; (void)sfp; (void)_rix;
#define NO_WARNING2() (void)ctx; (void)cid;

KMETHOD Color_new(CTX ctx, knh_sfp_t *sfp _RIX)
{
	NO_WARNING();
	QString s = String_to(QString, sfp[1]);
	QColor *c = new QColor(s);
	knh_RawPtr_t *p = new_RawPtr(ctx, sfp[1].p, c);
	RETURN_(p);
}

#ifdef __cplusplus
}
#endif
