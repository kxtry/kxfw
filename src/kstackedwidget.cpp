#include "stdafx.h"
#include "kstackedwidget.h"
#include "kwidget_p.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KStackedWidget)

class KStackedWidgetPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KStackedWidget)
public:
	KStackedWidgetPrivate()
		: index( -1 )
		, interval(1000)
		, effectType(KStackedWidget::NoEffect)
		, bAutoSwitch(false)
	{

	}

	~KStackedWidgetPrivate()
	{


	}

	bool bAutoSwitch;
	int interval;
	int index;
	QList<KWidget *> widgetList;
	QPointer<QParallelAnimationGroup> animationGroup;

	KStackedWidget::EffectType effectType;
};

KStackedWidget::KStackedWidget( QGraphicsItem *parent /* = NULL*/ )
: KWidget(*(new KStackedWidgetPrivate), parent)
{
	setFlag(QGraphicsItem::ItemHasNoContents);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape);
}

KStackedWidget::~KStackedWidget()
{
}

int KStackedWidget::insertWidget(int index, KWidget *widget)
{
	Q_D(KStackedWidget);

	if(index < 0)
	{
		index = d->widgetList.size();
	}
	else if( index > d->widgetList.size() ) 
	{
		index = d->widgetList.size();
	}

	widget->setParentItem(this);
	widget->setParent(this);
	d->widgetList.insert(index, widget);
	return index;
}

int KStackedWidget::currentIndex() const
{
	return d_func()->index;
}

void KStackedWidget::setCurrentIndex( int index )
{
	Q_D(KStackedWidget);

	if( d->index == index )
		return;

	d->index = index;

	if(index < 0 || index >= d->widgetList.size())
	{
		return;
	}

	update();

	if(d->effectType == NoEffect || d->index < 0)
	{
		d->index = index;
		_resetLayout();
		doAutoSwitch();
		return;
	}

	if(d->widgetList.size() <= 1)
	{
		return;
	}

	if (d->animationGroup && d->animationGroup->state() == QAbstractAnimation::Running )
	{
		d->animationGroup->stop();
	}

	if(d->effectType == SlideInOut)
	{
		QPointF leftPt(-width(), 0);
		QPointF midPt(0, 0);
		QPointF rightPt(width(), 0);

		d->animationGroup = new QParallelAnimationGroup(this);
		connect(d->animationGroup, SIGNAL(finished()), SLOT(onAnimationFinished()));
		connect(d->animationGroup, SIGNAL(stateChanged(QAbstractAnimation::State, QAbstractAnimation::State)), SLOT(onAnimationStateChanged(QAbstractAnimation::State, QAbstractAnimation::State)));

		QPropertyAnimation *slideIn = new QPropertyAnimation(widgetAt(index), "pos", d->animationGroup);
		slideIn->setDuration(300);
		d->animationGroup->addAnimation(slideIn);

		QPropertyAnimation *slideOut = new QPropertyAnimation(widgetAt(d->index), "pos", d->animationGroup);
		slideOut->setDuration(300);
		d->animationGroup->addAnimation(slideOut);

		if(index > d->index)
		{
			//move from right to left
			slideIn->setStartValue(rightPt);
			slideIn->setEndValue(midPt);

			slideOut->setStartValue(midPt);
			slideOut->setEndValue(leftPt);
		}
		else
		{
			//move from left to right
			slideIn->setStartValue(leftPt);
			slideIn->setEndValue(midPt);

			slideOut->setStartValue(midPt);
			slideOut->setEndValue(rightPt);
		}

		d->animationGroup->start(QAbstractAnimation::DeleteWhenStopped);

		d->index = index;
		widgetAt(d->index)->show();
	}
}

KWidget *KStackedWidget::widgetAt(int index) const
{
	return d_func()->widgetList.at(index);
}

KWidget * KStackedWidget::widgetAtByName( const QString& objectName ) const
{
	Q_D(const KStackedWidget);

	foreach( KWidget* item, d->widgetList )
	{
		QString xx = item->objectName();
		if ( item->objectName() == objectName )
		{
			return item;
		}
			
	}
	return NULL;

}

void KStackedWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	Q_D(KStackedWidget);
	KWidget::resizeEvent(event);

	foreach(KWidget *widget, d->widgetList)
	{
		widget->setFixSize(event->newSize());
		widget->setGeometry(QRectF(QPointF(0, 0), event->newSize()));
	}
}

void KStackedWidget::addItem(KWidget* item)
{
	insertWidget( d_func()->widgetList.size(), item );
}

void KStackedWidget::setLayout(QGraphicsLayout *layout)
{
	Q_UNUSED(layout);
	Q_ASSERT_X(false, __FUNCTION__, "stackedwidget does not need layout.");
}

QGraphicsLayout *KStackedWidget::layout() const
{
	return NULL;
}

int KStackedWidget::count() const
{
	return d_func()->widgetList.count();
}

void KStackedWidget::construct()
{
	KWidget::construct();

	Q_D(KStackedWidget);
	int index = d->index;
	index = qMax(d->index, 0);
	index = qMin(d->widgetList.size(), index);
	d->index = -1;
	setCurrentIndex(index);
}

void KStackedWidget::removeItem( KWidget* item )
{
	Q_D(KStackedWidget);
	d->widgetList.removeOne( item );
}

void KStackedWidget::removeItem( int index )
{
	d_func()->widgetList.removeAt( index );
}

int KStackedWidget::widgetIndex( const KWidget* widget )
{
	Q_D(KStackedWidget);

	for ( int i = 0 ; i < d->widgetList.size(); ++i )
	{
		if ( d->widgetList[i] == widget )
		{
			return i;
		}
	}
	return -1;
}

void KStackedWidget::setCurrentWidget( const KWidget *widget )
{
	int index = widgetIndex( widget );
	if ( index != -1 )
	{
		setCurrentIndex( index );
	}
}

void KStackedWidget::onAnimationFinished()
{
	_resetLayout();
}

void KStackedWidget::onAnimationStateChanged( QAbstractAnimation::State newState, QAbstractAnimation::State oldState )
{
	Q_D(KStackedWidget);
	if(newState == QAbstractAnimation::Stopped && oldState == QAbstractAnimation::Running)
	{
		if(d->bAutoSwitch)
		{
			doAutoSwitch();
			return;
		}
		_resetLayout();
	}
}

void KStackedWidget::_resetLayout()
{
	Q_D(KStackedWidget);
	
	for(int i = 0; i < d->widgetList.size(); i++)
	{
		KWidget *widget = d->widgetList[i];
		widget->setPos(0,0);
		widget->setVisible(d->index == i);
	}
}

KStackedWidget::EffectType KStackedWidget::effectType() const
{
	return d_func()->effectType;
}

void KStackedWidget::setEffectType(EffectType effect)
{
	d_func()->effectType = effect;
}

int KStackedWidget::interval()
{
	return d_func()->interval;
}

void KStackedWidget::setInterval( int v )
{
	Q_D(KStackedWidget);
	if(d->interval == v)
		return;
	d->interval = v;
}

bool KStackedWidget::autoSwitch()
{
	Q_D(KStackedWidget);
	return d->bAutoSwitch;
}

void KStackedWidget::setAutoSwitch( bool on )
{
	Q_D(KStackedWidget);
	d->bAutoSwitch = on;

	if(on)
	{
		doAutoSwitch();
	}
}

void KStackedWidget::on_autoswitch_page()
{
	Q_D(KStackedWidget);
	int index = (d->index + 1) % d->widgetList.size();
	setCurrentIndex(index);
}

void KStackedWidget::doAutoSwitch()
{
	Q_D(KStackedWidget);
	if(!d->bAutoSwitch)
		return;
	if(d->widgetList.size() < 2)
		return;
	if(!isVisible())
		return;
	QTimer::singleShot(d->interval, this, SLOT(on_autoswitch_page()));
}

void KStackedWidget::visibleEvent( bool bvis )
{
	Q_D(KStackedWidget);
	if(bvis)
	{
		if(d->bAutoSwitch)
		{
			doAutoSwitch();
		}
	}
}
