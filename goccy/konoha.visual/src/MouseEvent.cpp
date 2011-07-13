#include <visual.hpp>

#ifdef __cplusplus
extern "C" {
#endif

KMETHOD MouseEvent_lastScreenPos(CTX ctx, knh_sfp_t *sfp _RIX)
{
	QGraphicsSceneMouseEvent *event = RawPtr_to(QGraphicsSceneMouseEvent *, sfp[0]);
	QPointF spos = event->lastScreenPos();
	KPoint *p = new KPoint(spos.x(), spos.y());
	RETURN_(new_RawPtr(ctx, sfp[1].p, p));
}

#ifdef __cplusplus
}
#endif
