#include "stdafx.h"
#include "kcalendar.h"
#include "kwidget_p.h"
#include "kxmlui.h"
#include "kframeitem.h"

#include <QCalendarWidget>

KX_WIDGET_CREATOR_GLOBAL_STATIC(KCalendar)

class KCalendarPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KCalendar)
public:
	QCalendarWidget *calendar;
	QGraphicsProxyWidget *calendarProxy;
public:
	KCalendarPrivate()
		: calendar(NULL)
		, calendarProxy(NULL)
	{

	}

	~KCalendarPrivate()
	{

	}
};

KCalendar::KCalendar( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KCalendarPrivate(), parent)
{
	Q_D(KCalendar);
	setDragPolicy(Nothing);
	d->calendar = new QCalendarWidget();
	d->calendar->setStyleSheet("QCalendarWidget { border-style: none; background: transparent}");
	d->calendarProxy = new QGraphicsProxyWidget(this);
	d->calendarProxy->setWidget(d->calendar);
	d->calendarProxy->setFocusPolicy(Qt::StrongFocus);
	d->calendarProxy->setZValue(1.0);
}

KCalendar::~KCalendar()
{

}

void KCalendar::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	__super::resizeEvent(event);
	updateLayout();
}

void KCalendar::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	//d_func()->lineEdit->contextMenuEvent(event);
	event->ignore();
}

void KCalendar::updateLayout()
{
	Q_D(KCalendar);
	QRectF rt = rect();
	d->calendarProxy->setGeometry(rt.toRect());
	d->calendar->setGeometry(rt.toRect());
}