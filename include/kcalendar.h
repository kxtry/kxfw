#pragma once;

#include "kwidget.h"

/*
参考：C:\Qt\4.7.1\examples\widgets\calendarwidget，直接把该日历，proxy进来即可。
*/


class KCalendarPrivate;

class KXFW_API KCalendar : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KCalendar)

public:
	explicit KCalendar(QGraphicsItem *parent=NULL);
	~KCalendar();
protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent * event);
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
	void updateLayout();
};