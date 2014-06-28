#include "stdafx.h"
#include "kslider.h"

#include "kwidget_p.h"
#include "kimageitem.h"
#include "ktheme.h"
#include "kframeitem.h"
#include "kxmlui.h"


class KSliderPrivate: public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KSlider);
public:
	KSliderPrivate();
	~KSliderPrivate();

	KFrameItem*		m_select;
	KFrameItem*		m_thumb;
	KFrameItem*		m_frame;
	qreal				m_value;
	bool				m_isDragging;
	QPointF				m_pointInThumb;
	QSizeF				m_thumbSize;

	qreal				m_minimum;
	qreal				m_maximum;

	Qt::Orientation		m_orientation;

	QBasicTimer			m_layoutTimer;
};

KSliderPrivate::KSliderPrivate()
: m_select(NULL)
, m_thumb(NULL)
, m_value(0.0)
, m_frame(NULL)
, m_isDragging(false)
, m_minimum(0.0)
, m_maximum(1.0)
, m_orientation(Qt::Horizontal)
{
}

KSliderPrivate::~KSliderPrivate()
{
}

KX_WIDGET_CREATOR_GLOBAL_STATIC(KSlider)

KSlider::KSlider( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KSliderPrivate, parent)
{
	init();
}

KSlider::KSlider( KWidgetPrivate &dd, QGraphicsItem *parent /*= NULL*/ )
: KWidget(dd, parent)
{
	init();
}

KSlider::~KSlider()
{

}

void KSlider::init()
{
	Q_D(KSlider);
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
	setDragPolicy(Nothing);

	d->m_frame = new KFrameItem(":/image/theme/hslider_bkg.png", KFrameItem::GridFour, this);
	d->m_select = new KFrameItem(":/image/theme/slider_select.png", KFrameItem::GridFour, this);
	d->m_thumb = new KFrameItem(":/image/theme/hsliderthumb_normal.png|:/image/theme/hsliderthumb_hover.png|:/image/theme/hsliderthumb_down.png", KFrameItem::GridFour, this);
	relayout();
}

void KSlider::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	Q_D(KSlider);
	QRectF thumbRect = d->m_thumb->geometry();
	if ( thumbRect.contains(event->pos()) )
	{
		d->m_isDragging = true;
		d->m_pointInThumb = event->pos() - thumbRect.topLeft();
		d->m_thumb->setDown();
	}
	else
	{
		d->m_thumb->setNormal();
		if(d->m_orientation == Qt::Vertical)
		{
			qreal val = event->pos().y() - d->m_thumbSize.height() / 2;
			qreal ratio = val / (height() - d->m_thumbSize.height());
			_setValue(ratio);
		}
		else
		{
			qreal val = event->pos().x() - d->m_thumbSize.width() / 2;
			qreal ratio = val / (width() - d->m_thumbSize.width());
			_setValue(ratio);
		}
	}
}

void KSlider::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D(KSlider);
	d_func()->m_isDragging = false;
	d->m_thumb->setNormal();
	emit mouseRelease();
}

void KSlider::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	Q_D(KSlider);
	if (d->m_isDragging)
	{
		if(d->m_orientation == Qt::Vertical)
		{
			qreal yValue = event->pos().y() - d->m_pointInThumb.y();
			_setValue( yValue / (height() - d->m_thumbSize.height()));
		}
		else
		{
			qreal xValue = event->pos().x() - d->m_pointInThumb.x();
			_setValue( xValue / (width() - d->m_thumbSize.width()));
		}
	}
}

void KSlider::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	Q_D(KSlider);
	
}

void KSlider::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_D(KSlider);
	QRectF thumbRect = d->m_thumb->geometry();
	if (thumbRect.contains(event->pos()))
	{
		d->m_thumb->setHover();
	}
	else
	{
		d->m_thumb->setNormal();
	}
}

void KSlider::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	Q_UNUSED(event);
	Q_D(KSlider);
	d->m_thumb->setNormal();
}

qreal KSlider::value() const
{
	Q_D(const KSlider);
	qreal val = _value();
	val = val * (d->m_maximum - d->m_minimum) + d->m_minimum;
	return val;
}

bool KSlider::setValue(qreal value)
{
	Q_D(KSlider);
	if(value > d->m_maximum)
		value = d->m_maximum;
	if(value < d->m_minimum)
		value = d->m_minimum;
	qreal val = (value - d->m_minimum) / (d->m_maximum - d->m_minimum);
	return _setValue(val);
}

QString KSlider::thumbImage() const
{
	Q_D(const KSlider);
	return d->m_thumb->imagePath();
}

void KSlider::setThumbImage( const QString &imgs )
{
	Q_D(KSlider);
	
	d->m_thumb->setImagePath( imgs );
}

QString KSlider::frameImage() const
{
	Q_D(const KSlider);
	return d->m_frame->imagePath();
}

void KSlider::setFrameImage( const QString &backImage )
{
	Q_D(KSlider);
	d->m_frame->setImagePath(backImage);
}

QString KSlider::selectImage() const
{
	Q_D(const KSlider);
	return d->m_select->imagePath();
}

void KSlider::setSelectImage( const QString &img )
{
	Q_D(KSlider);
	d->m_select->setImagePath( img );
}

void KSlider::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	relayout();
}

void KSlider::relayout()
{
	Q_D(KSlider);
	if(!d->m_layoutTimer.isActive())
		d->m_layoutTimer.start(0,this);
}

void KSlider::_relayout()
{
	Q_D(KSlider);

	d->m_layoutTimer.stop();

	if(size().isEmpty())
		return;
	if(d->m_thumbSize.isEmpty())
		d->m_thumbSize = d->m_thumb->pixmapSize();
	if(d->m_orientation == Qt::Vertical)
	{
		QRectF frameRt, selectRt, thumbRt;
		QSizeF bkSize = d->m_frame->pixmapSize();
		QSizeF selSize = d->m_select->pixmapSize();
		frameRt.setRect((width() - bkSize.width()) / 2, 0, bkSize.width(), height());
		qreal val = d->m_value * (height() - d->m_thumbSize.height()) + d->m_thumbSize.height() / 2;
		thumbRt.setRect((width() - d->m_thumbSize.width()) / 2, val - d->m_thumbSize.height() / 2, d->m_thumbSize.width(), d->m_thumbSize.height());
		selectRt.setRect((width() - selSize.width()) / 2, val, selSize.width(), height() - val);

		d->m_frame->setGeometry(frameRt);
		d->m_select->setGeometry(selectRt);
		d->m_thumb->setGeometry(thumbRt);
	}
	else
	{
		QRectF frameRt, selectRt, thumbRt;
		QSizeF bkSize = d->m_frame->pixmapSize();
		QSizeF selSize = d->m_select->pixmapSize();
		frameRt.setRect(0, (height() - bkSize.height()) / 2, width(), bkSize.height());
		qreal val = d->m_value * (width() - d->m_thumbSize.width()) + d->m_thumbSize.width() / 2;
		thumbRt.setRect(val - d->m_thumbSize.width() / 2, (height() - d->m_thumbSize.height()) / 2, d->m_thumbSize.width(), d->m_thumbSize.height());
		selectRt.setRect(0, (height() - selSize.height()) / 2, val, selSize.height());

		d->m_frame->setGeometry(frameRt);
		d->m_select->setGeometry(selectRt);
		d->m_thumb->setGeometry(thumbRt);
	}
}

QSizeF KSlider::range() const
{
	Q_D(const KSlider);
	return QSizeF(d->m_minimum,d->m_maximum);
}

void KSlider::setRange( const QSizeF& s )
{
	setRange(s.width(), s.height());
}

void KSlider::setRange( qreal minval, qreal maxval )
{
	Q_D(KSlider);
	d->m_minimum = minval;
	d->m_maximum = maxval;
}

qreal KSlider::_value() const
{
	Q_D(const KSlider);
	qreal val = d->m_value;
	return val;
}

bool KSlider::_setValue( qreal val )
{
	Q_D(KSlider);

	if (val < 0)
	{
		val = 0;
	}
	else if (val > 1.0)
	{
		val = 1.0;
	}

	if( qFuzzyCompare( d->m_value, val) == false )
	{
		d->m_value = val;

		relayout();

		val = val * (d->m_maximum - d->m_minimum) + d->m_minimum;

		emit valueChanged(val);

		return true;
	}

	return false;
}

qreal KSlider::minimum()
{
	Q_D(KSlider);
	return d->m_minimum;
}

void KSlider::setMinimum( qreal v )
{
	Q_D(KSlider);
	d->m_minimum = v;
}

qreal KSlider::maximum()
{
	Q_D(KSlider);
	return d->m_minimum;
}

void KSlider::setMaximum( qreal v )
{
	Q_D(KSlider);
	d->m_maximum = v;
}

Qt::Orientation KSlider::orientation() const
{
	Q_D(const KSlider);
	return d->m_orientation;
}

void KSlider::setOrientation( Qt::Orientation o )
{
	Q_D(KSlider);
	if(d->m_orientation == o)
		return;
	d->m_orientation = o;
	if(o == Qt::Horizontal)
	{
		d->m_frame->setImagePath(":/image/theme/hslider_bkg.png");
		d->m_select->setImagePath(":/image/theme/slider_select.png");
		d->m_thumb->setImagePath(":/image/theme/hsliderthumb_normal.png|:/image/theme/hsliderthumb_hover.png|:/image/theme/hsliderthumb_down.png");
	}
	else
	{
		d->m_frame->setImagePath(":/image/theme/vslider_bkg.png");
		d->m_select->setImagePath(":/image/theme/slider_select.png");
		d->m_thumb->setImagePath(":/image/theme/vsliderthumb_normal.png|:/image/theme/vsliderthumb_hover.png|:/image/theme/vsliderthumb_down.png");
	}
}

QSizeF KSlider::thumbSize() const
{
	Q_D(const KSlider);
	return d->m_thumbSize;
}

void KSlider::setThumbSize( const QSizeF& s )
{
	Q_D(KSlider);
	d->m_thumbSize = s;
}

void KSlider::timerEvent( QTimerEvent * event )
{
	Q_D(KSlider);
	if (event->type() == QEvent::Timer)
	{
		if(static_cast<QTimerEvent*>(event)->timerId() == d->m_layoutTimer.timerId())
		{
			_relayout();
		}
	}
	__super::timerEvent(event);
}
