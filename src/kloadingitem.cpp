#include "stdafx.h"
#include "kloadingitem.h"

#include "kwidget_p.h"
#include "ktheme.h"
#include "kxmlui.h"

class KLoadingItemPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KLoadingItem);

public:
	KLoadingItemPrivate()
		: KWidgetPrivate()
		, timerId(0)
		, rotationAngle(0)
		, color(Qt::white)
		, fps(24)
		, rps(1)
		, ratio(0.3)
	{
		adjust();
	}

	void adjust()
	{
		degree = rps * 360 / fps;
	}

private:
	int timerId;
	int rotationAngle;
	qreal ratio;
	qreal fps;
	qreal rps;
	qreal degree;
	QColor color;
	QSizeF animationSize;
};

KX_WIDGET_CREATOR_GLOBAL_STATIC(KLoadingItem)

KLoadingItem::KLoadingItem( QGraphicsItem* parent )
:	KWidget(*new KLoadingItemPrivate(), parent)
{
	Q_D(KLoadingItem);
	d->timerId = startTimer(1000 / d->fps);
}

void KLoadingItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(KLoadingItem);
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->setRenderHint(QPainter::Antialiasing);

	QSizeF s = d->animationSize;
	if(s.isEmpty())
	{
		s = size();
	}
	//draw loading ring
	QPainterPath ring;
	QRectF dest = QRectF((width() - s.width()) / 2, (height() - s.height()) / 2, s.width(), s.height());
	QPointF center = dest.center();
	qreal radiusSup = qMin(dest.width(), dest.height()) / 2;
	qreal outterRadius = radiusSup;
	qreal innerRadius = radiusSup * (1 - d->ratio);
	ring.addEllipse(center, outterRadius, outterRadius);
	ring.addEllipse(center, innerRadius, innerRadius);
	ring.setFillRule(Qt::OddEvenFill);
	QConicalGradient cg(center, -d->rotationAngle);

	//simulate gamma correction by 5 stops, gamma = 2.2
	QColor c0(d->color);
	QColor c1(d->color);
	QColor c2(d->color);
	QColor c3(d->color);
	QColor c4(d->color);
	QColor c5(d->color);
	c0.setAlpha(0xff);
	c1.setAlpha(0xf7);
	c2.setAlpha(0xe3);
	c3.setAlpha(0xb0);
	c4.setAlpha(0x5a);
	c5.setAlpha(0x1f);
	cg.setColorAt(0.0, c0);
	cg.setColorAt(0.2, c1);
	cg.setColorAt(0.4, c2);
	cg.setColorAt(0.6, c3);
	cg.setColorAt(0.8, c4);
	cg.setColorAt(1.0, c5);
	painter->setBrush(cg);
	painter->setPen(QColor::fromRgba(0));
	painter->drawPath(ring);
}

void KLoadingItem::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);
	Q_D(KLoadingItem);

	d->rotationAngle += d->degree;
	d->rotationAngle %= 360;
	update();
}

qreal KLoadingItem::ratio() const
{
	Q_D(const KLoadingItem);
	return d->ratio;
}

void KLoadingItem::setRatio(const qreal &r)
{
	Q_D(KLoadingItem);
	Q_ASSERT(r > 0 && r < 1);
	d->ratio = r;
}

qreal KLoadingItem::fps() const
{
	Q_D(const KLoadingItem);
	return d->fps;
}

void KLoadingItem::setFps(const qreal &f)
{
	Q_D(KLoadingItem);
	Q_ASSERT(f > 0 && f < 600.0);
	d->fps = f;
	d->adjust();
}

QColor KLoadingItem::color() const
{
	Q_D(const KLoadingItem);
	return d->color;
}

void KLoadingItem::setColor(const QColor &color)
{
	Q_D(KLoadingItem);
	d->color = color;
}

QSizeF KLoadingItem::animationSize() const
{
	Q_D(const KLoadingItem);
	return d->animationSize;
}

void KLoadingItem::setAnimationSize( const QSizeF& s )
{
	Q_D(KLoadingItem);
	d->animationSize = s;
}



void KLoadingItem::start()
{
	Q_D(KLoadingItem);
	killTimer(d->timerId);
	d->timerId = startTimer(1000 / d->fps);
}

void KLoadingItem::stop()
{
	Q_D(KLoadingItem);
	killTimer(d->timerId);
}

void KLoadingItem::visibleEvent( bool bvis )
{
	if(bvis)
	{
		start();
	}
	else
	{
		stop();
	}
}