#include "stdafx.h"
#include "kbubbletip.h"

#include "kfollowwindow_p.h"
#include "kwidget.h"

#include "kxmlui.h"

class KBubbleTipPrivate : public KFollowWindowPrivate
{
	Q_DECLARE_PUBLIC(KBubbleTip)
public:
	QPainterPath bubblePath;
	QBrush brush;
	QPen pen;
	QGraphicsDropShadowEffect m_effect;

public:
	KBubbleTipPrivate()
		: pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) 
		, brush(QColor(255,255,224)) 
	{

	}

	~KBubbleTipPrivate()
	{

	}

	void init()
	{
		Q_Q(KBubbleTip);

		q->setGraphicsEffect(&m_effect);
		q->setSceneMargins(QMargins(15,15,15,15));
		q->setTranslucent(true);
		q->setAutoDeleteWhenAppQuit(true);
	}
};

KBubbleTip::KBubbleTip( QWidget *parent/*=NULL*/ )
 :KFollowWindow(*new KBubbleTipPrivate(), parent) 
{
	Q_D(KBubbleTip);
	d->init();
}

KBubbleTip::~KBubbleTip()
{

}

void KBubbleTip::setPen( const QPen& pen )
{
	Q_D(KBubbleTip);
	d->pen = pen;
}

void KBubbleTip::setBrush( const QBrush& br )
{
	Q_D(KBubbleTip);
	d->brush = br;
}

bool KBubbleTip::shadowEnabled()
{
	Q_D(KBubbleTip);
	return d->m_effect.isEnabled();
}

void KBubbleTip::setShadowEnable( bool on )
{
	Q_D(KBubbleTip);
	d->m_effect.setEnabled(on);
}

qreal KBubbleTip::shadowBlurRadius()
{
	Q_D(KBubbleTip);

	return d->m_effect.blurRadius();
}

void KBubbleTip::setShadowBlurRadius( qreal v )
{
	Q_D(KBubbleTip);
	d->m_effect.setBlurRadius(v);
}

QPointF KBubbleTip::shadowOffset()
{
	Q_D(KBubbleTip);
	return d->m_effect.offset();
}

void KBubbleTip::setShadowOffset( QPointF v )
{
	Q_D(KBubbleTip);
	d->m_effect.setOffset(v);
}

void KBubbleTip::construct()
{
	
}

QColor KBubbleTip::brushColor() const
{
	Q_D(const KBubbleTip);
	return d->brush.color();
}

void KBubbleTip::setBrushColor( const QColor& clr )
{
	Q_D(KBubbleTip);
	return d->brush.setColor(clr);
}

QColor KBubbleTip::penColor() const
{
	Q_D(const KBubbleTip);
	return d->pen.color();
}

void KBubbleTip::setPenColor( const QColor& clr )
{
	Q_D(KBubbleTip);
	return d->pen.setColor(clr);
}

void KBubbleTip::resetGeometry()
{
	Q_D(KBubbleTip);

	__super::resetGeometry();

	if(d->target == NULL)
		return;

	d->bubblePath = QPainterPath();
	//minRt和targetRt的左上角是重叠的。
	Direction tmpDirection = d->direction;
	if(tmpDirection == LeftTop || tmpDirection == LeftCenter || tmpDirection == LeftBottom)
	{
		QRectF itemRt = rect();
		QMargins m = sceneMargins();
		itemRt.adjust(m.left(), m.top(), -m.right(), -m.bottom());
		QPointF arrowPt = QPointF(itemRt.right()+m.left(), itemRt.center().y());
		QPointF arrowPt2 = QPointF(itemRt.right()-5, itemRt.center().y());

		QPainterPath arrow;
		arrow.moveTo(arrowPt);
		arrow.lineTo(arrowPt2-QPointF(0,10));
		arrow.lineTo(arrowPt2+QPointF(0,10));
		arrow.closeSubpath();		
		d->bubblePath.addRoundedRect(itemRt, 3,3);
		d->bubblePath =	d->bubblePath.united(arrow);
	}
	else if(tmpDirection == RightTop || tmpDirection == RightCenter || tmpDirection == RightBottom)
	{
		QRectF itemRt = rect();
		QMargins m = sceneMargins();
		itemRt.adjust(m.left(), m.top(), -m.right(), -m.bottom());
		QPointF arrowPt = QPointF(itemRt.left()-m.left(), itemRt.center().y());
		QPointF arrowPt2 = QPointF(itemRt.left()+5, itemRt.center().y());

		QPainterPath arrow;
		arrow.moveTo(arrowPt);
		arrow.lineTo(arrowPt2-QPointF(0,10));
		arrow.lineTo(arrowPt2+QPointF(0,10));
		arrow.closeSubpath();		
		d->bubblePath.addRoundedRect(itemRt, 3,3);
		d->bubblePath =	d->bubblePath.united(arrow);
	}
	else if(tmpDirection == TopLeft || tmpDirection == TopCenter || tmpDirection == TopRight)
	{
		QRectF itemRt = rect();
		QMargins m = sceneMargins();
		itemRt.adjust(m.left(), m.top(), -m.right(), -m.bottom());
		QPointF arrowPt = QPointF(itemRt.center().x(), itemRt.bottom()+m.top());
		QPointF arrowPt2 = QPointF(itemRt.center().x(), itemRt.bottom()-5);

		QPainterPath arrow;
		arrow.moveTo(arrowPt);
		arrow.lineTo(arrowPt2-QPointF(10,0));
		arrow.lineTo(arrowPt2+QPointF(10,0));
		arrow.closeSubpath();		
		d->bubblePath.addRoundedRect(itemRt, 3,3);
		d->bubblePath =	d->bubblePath.united(arrow);
	}
	else if(tmpDirection == BottomLeft || tmpDirection == BottomCenter || tmpDirection == BottomRight)
	{
		QRectF itemRt = rect();
		QMargins m = sceneMargins();
		itemRt.adjust(m.left(), m.top(), -m.right(), -m.bottom());
		QPointF arrowPt = QPointF(itemRt.center().x(), itemRt.top()-m.top());
		QPointF arrowPt2 = QPointF(itemRt.center().x(), itemRt.top()+5);

		QPainterPath arrow;
		arrow.moveTo(arrowPt);
		arrow.lineTo(arrowPt2-QPointF(10,0));
		arrow.lineTo(arrowPt2+QPointF(10,0));
		arrow.closeSubpath();		
		d->bubblePath.addRoundedRect(itemRt, 3,3);
		d->bubblePath =	d->bubblePath.united(arrow);
	}
	update();
}

void KBubbleTip::drawBackground( QPainter * painter, const QRectF & rect )
{
	Q_D(KBubbleTip);
	if(d->bubblePath.isEmpty())
		return;
	QPainter::RenderHints hints = painter->renderHints();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(d->pen);
	painter->setBrush(d->brush);
	painter->drawPath(d->bubblePath);
	painter->setRenderHints(hints);
}

void KBubbleTip::drawForeground( QPainter * painter, const QRectF & rect )
{

}

void KBubbleTip::setBackgroundColor( const QColor& clr )
{
	setBrushColor(clr);
}

QColor KBubbleTip::backgroundColor() const
{
	return brushColor();
}

void KBubbleTip::setTarget( KWidget *w )
{
	Q_D(KBubbleTip);
	if(d->target == w)
		return;
	if(d->target)
	{
		listenEvent(d->target, abortAll);
	}
	listenEvent(w, dockFlag);
	d->target = w;
}