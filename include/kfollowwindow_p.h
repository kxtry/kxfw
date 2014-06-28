#pragma once;


#include "kwindow_p.h"

class KXFW_API KFollowWindowPrivate : public KWindowPrivate
{
	Q_DECLARE_PUBLIC(KFollowWindow)
public:
	QPointer<KWidget> target;
	KFollowWindow::Direction direction;
	QPointF offsetPt;
	QPointF targetPt;
	int hideFlags;
	QSize winSize;
public:
	KFollowWindowPrivate();
	~KFollowWindowPrivate();
	void init();
};