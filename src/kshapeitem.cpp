#include "stdafx.h"
#include "kshapeitem.h"
#include "kwidget_p.h"
#include "kxmlui.h"
#include "ktheme.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KShapeItem)

class KShapeItemPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KShapeItem)
public:
	QPen pen;
	QBrush brush;
	QMargins margins;
	bool bsmooth;
	KShapeItem::Shape shape;
	int roundPixel;
	QSize fan;
public:
	KShapeItemPrivate()
		: bsmooth(false)
		, roundPixel(2)
		, fan(0, 360*16)
		, shape(KShapeItem::Rect)
		, brush(Qt::blue)
		, pen(Qt::DotLine)
	{

	}

	~KShapeItemPrivate()
	{

	}
};

KShapeItem::KShapeItem( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KShapeItemPrivate(), parent)
{

}

KShapeItem::~KShapeItem()
{

}

Qt::PenStyle KShapeItem::penStyle()
{
	Q_D(KShapeItem);
	return d->pen.style();
}

void KShapeItem::setPenStyle( Qt::PenStyle ps )
{
	Q_D(KShapeItem);
	d->pen.setStyle(ps);
}

Qt::PenCapStyle KShapeItem::capStyle()
{
	Q_D(KShapeItem);
	return d->pen.capStyle();
}

void KShapeItem::setPenCapStyle( Qt::PenCapStyle cs )
{
	Q_D(KShapeItem);
	d->pen.setCapStyle(cs);
}

Qt::PenJoinStyle KShapeItem::joinStyle()
{
	Q_D(KShapeItem);
	return d->pen.joinStyle();
}

void KShapeItem::setPenJoinStyle( Qt::PenJoinStyle js )
{
	Q_D(KShapeItem);
	d->pen.setJoinStyle(js);
}

Qt::BrushStyle KShapeItem::brushStyle()
{
	Q_D(KShapeItem);
	return d->brush.style();
}

void KShapeItem::setBrushStyle( Qt::BrushStyle bs )
{
	Q_D(KShapeItem);
	QColor clr = d->brush.color();
	d->brush = QBrush(clr, bs);
}

QColor KShapeItem::penColor() const
{
	Q_D(const KShapeItem);
	return d->pen.color();
}

void KShapeItem::setPenColor( const QColor& clr )
{
	Q_D(KShapeItem);
/*
	PenStyle ps = d->pen.style();
	Qt::PenCapStyle pcs = d->pen.capStyle();
	Qt::PenJoinStyle pjs = d->pen.joinStyle();
*/
	d->pen.setColor(clr);
}

QColor KShapeItem::brushColor() const
{
	Q_D(const KShapeItem);
	return d->brush.color();
}

void KShapeItem::setBrushColor( const QColor& clr )
{
	Q_D(KShapeItem);
	Qt::BrushStyle bs = d->brush.style();
	if(bs > Qt::DiagCrossPattern)
	{
		bs = Qt::SolidPattern;
	}
	d->brush = QBrush(clr, bs);
}

QMargins KShapeItem::shapeMargins() const
{
	Q_D(const KShapeItem);
	return d->margins;
}

void KShapeItem::setShapeMargins( QMargins& m )
{
	Q_D(KShapeItem);
	d->margins = m;
}

bool KShapeItem::smooth()
{
	Q_D(KShapeItem);
	return d->bsmooth;
}

void KShapeItem::setSmooth( bool on )
{
	Q_D(KShapeItem);
	d->bsmooth = on;
}

int KShapeItem::penWidth()
{
	Q_D(KShapeItem);
	return d->pen.width();
}

void KShapeItem::setPenWidth( int w )
{
	Q_D(KShapeItem);
	d->pen.setWidth(w);
}

KShapeItem::Shape KShapeItem::drawShape() const
{
	Q_D(const KShapeItem);
	return d->shape;
}

void KShapeItem::setDrawShape( Shape s )
{
	Q_D(KShapeItem);
	d->shape = s;
}

void KShapeItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_D(KShapeItem);
	QPen penold = painter->pen();
	QBrush brushold = painter->brush();
	bool bsmoothold = painter->testRenderHint(QPainter::Antialiasing);

	painter->setPen(d->pen);
	painter->setBrush(d->brush);
	painter->setRenderHint(QPainter::Antialiasing, d->bsmooth);
	QRectF marginRt = rect();
	marginRt.adjust(d->margins.left(), d->margins.top(), -d->margins.right(), -d->margins.bottom());
	/*¼õÈ¥»­±Ê´óÐ¡*/
	marginRt.adjust(d->pen.width(), d->pen.width(), -d->pen.width(), -d->pen.width());
	switch(d->shape)
	{
	case Rect:
		{
			painter->drawRect(marginRt);
		}
		break;
	case RoundRect:
		{
			painter->drawRoundedRect(marginRt, d->roundPixel, d->roundPixel);
		}
		break;
	case Ellipse:
		{
			painter->drawEllipse(marginRt);
		}
		break;
	case Pie:
		{
			painter->drawPie(marginRt, d->fan.width(), d->fan.height());
		}
		break;
	case Chord:
		{
			painter->drawChord(marginRt, d->fan.width(), d->fan.height());
		}
		break;
	}

	painter->setRenderHint(QPainter::Antialiasing, bsmoothold);
	painter->setPen(penold);
	painter->setBrush(brushold);
}

int KShapeItem::roundPixel()
{
	Q_D(KShapeItem);
	return d->roundPixel;
}

void KShapeItem::setRoundPixel( int pixels )
{
	Q_D(KShapeItem);
	d->roundPixel = pixels;
}

QSize KShapeItem::fanAngle() const
{
	Q_D(const KShapeItem);
	return d->fan;
}

void KShapeItem::setFanAngle( const QSize& s )
{
	Q_D(KShapeItem);
	d->fan = s;
}

QString KShapeItem::textureBrush() const
{
	Q_D(const KShapeItem);
	return "";
}

void KShapeItem::setTextureBrush( const QString& t )
{
	Q_D(KShapeItem);
	QPixmap pix = KTheme::getPixmap(t);
	if(pix.isNull())
		return;
	d->brush = QBrush(pix);
}

QtGradient KShapeItem::gradientBrush() const
{
	return QtGradient();
}

void KShapeItem::setGradientBrush( const QtGradient& gd )
{
	Q_D(KShapeItem);
	switch(gd.t)
	{
	case QGradient::LinearGradient:
		{
			QLinearGradient lg(gd.d.l.startx, gd.d.l.starty, gd.d.l.finalx, gd.d.l.finaly);
			lg.setCoordinateMode(QGradient::ObjectBoundingMode);
			lg.setSpread(gd.spread);
			lg.setStops(gd.gs);
			d->brush = QBrush(lg);
		}
		break;
	case QGradient::ConicalGradient:
		{
			QConicalGradient cg(gd.d.c.centerx, gd.d.c.centery,gd.d.c.angle);
			cg.setCoordinateMode(QGradient::ObjectBoundingMode);
			cg.setSpread(gd.spread);
			cg.setStops(gd.gs);
			d->brush = QBrush(cg);
		}
		break;
	case QGradient::RadialGradient:
		{
			QRadialGradient rg(gd.d.r.centerx, gd.d.r.centery,gd.d.r.radius);
			rg.setCoordinateMode(QGradient::ObjectBoundingMode);
			rg.setSpread(gd.spread);
			rg.setStops(gd.gs);
			d->brush = QBrush(rg);
		}
		break;
	}
}

