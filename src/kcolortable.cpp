#include "stdafx.h"
#include "kcolortable.h"
#include "kwidget_p.h"
#include "kxmlui.h"
#include "colorspace.h"
#include "ktheme.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KColorTable)

class KColorTablePrivate : public KWidgetPrivate
{
public:
	KColorTablePrivate()
	{
		hue = 180.;
		light = 50.;
		sat = 50.;
		colorselect = KTheme::getImage(":/image/theme/colorselect.png");
	}
	QImage huelight;
	QImage satuation;
	float hue, light, sat;
	QImage colorselect;
};

#define DISTANCE_HLS	(5)
#define HEIGHT_SATUATION	(15)

KColorTable::KColorTable( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KColorTablePrivate(), parent)
{
	setDragPolicy(Nothing);
	setAcceptedMouseButtons(Qt::LeftButton);
}

KColorTable::~KColorTable()
{

}

void KColorTable::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_D(KColorTable);
	painter->save();
	QRectF drawRt = hueLightRect();
	painter->setClipRect(drawRt, Qt::ReplaceClip);
	painter->drawImage(drawRt, d->huelight);
	qreal centerx = d->hue / 360.0 * width();
	qreal centery = d->light / 100.0 * (height() - HEIGHT_SATUATION - DISTANCE_HLS) ;
	painter->drawImage(centerx - d->colorselect.width() / 2, centery - d->colorselect.height() / 2, d->colorselect);

	drawRt = satuationRect();
	painter->setClipRect(drawRt, Qt::ReplaceClip);
	painter->drawImage(drawRt, d->satuation);
	centerx = d->sat / 100.0 * width();
	centery = height() - HEIGHT_SATUATION / 2;
	painter->drawImage(centerx - d->colorselect.width() / 2, centery - d->colorselect.height() / 2, d->colorselect);
	painter->restore();
}

void KColorTable::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(KColorTable);
	if(d->huelight.width() != width() 
		|| (d->huelight.height() + HEIGHT_SATUATION + DISTANCE_HLS) != height())
	{
		d->huelight = KHSL::CreateColorTable(width(), height() - HEIGHT_SATUATION - DISTANCE_HLS, d->sat);
	}
	if(d->satuation.width() != width()
		|| d->satuation.height() != HEIGHT_SATUATION)
	{
		d->satuation = KHSL::CreateColorBar(width(), HEIGHT_SATUATION, d->hue, d->light);
	}
}

QColor KColorTable::color() const
{
	Q_D(const KColorTable);

	QRgb rgb;
	KHSL::Hsl2Rgb(d->hue, d->sat, d->light, 0xFF, &rgb);
	return rgb;
}

void KColorTable::setColor( const QColor& clr )
{
	Q_D(KColorTable);

	QRgb rgb = clr.rgb();
	float hue, sat, light;
	KHSL::Rgb2Hsl(rgb, &hue, &sat, &light);
	if(d->huelight.isNull() || d->satuation.isNull())
	{
		d->hue = hue;
		d->light = light;
		d->sat = sat;
		return;
	}
	if(qFuzzyCompare(hue, d->hue)
		|| qFuzzyCompare(light, d->light))
	{
		d->hue = hue;
		d->light = light;
		KHSL::UpdateColorBar(d->satuation, hue, light);
	}
	if(qFuzzyCompare(sat, d->sat))
	{
		d->sat = sat;
		KHSL::UpdateColorTable(d->huelight, sat);
	}
	update();
}

void KColorTable::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D(KColorTable);
	QPointF pt = event->pos();
	QRectF hitRt = hueLightRect();
	if(hitRt.contains(pt))
	{
		d->hue = pt.x() / hitRt.width() * 360.0;
		d->light = pt.y() / hitRt.height() * 100.0;
		update(hitRt);

		QRgb clr;
		KHSL::Hsl2Rgb(d->hue, d->sat, d->light, 0, &clr);
		KHSL::UpdateColorBar(d->satuation, d->hue, d->light);
		emit colorChanged(clr);
	}
	else
	{
		hitRt = satuationRect();
		if(hitRt.contains(pt))
		{
			d->sat = pt.x() / hitRt.width() * 100.0;
			update(hitRt);

			QRgb clr;
			KHSL::Hsl2Rgb(d->hue, d->sat, d->light, 0, &clr);
			KHSL::UpdateColorTable(d->huelight, d->sat);
			emit colorChanged(clr);
		}
	}
}

void KColorTable::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	mousePressEvent(event);
}

QRectF KColorTable::hueLightRect()
{
	QRectF drawRt = rect();
	drawRt.setRect(0, 0, width(), height() - HEIGHT_SATUATION - DISTANCE_HLS);
	return drawRt;
}

QRectF KColorTable::satuationRect()
{
	QRectF drawRt = rect();
	drawRt.setRect(0, height() - HEIGHT_SATUATION, width(), HEIGHT_SATUATION);
	return drawRt;
}

void KColorTable::on_colorChanged( const QColor& clr )
{
	KTheme::setThemeColor(clr);
}

void KColorTable::connectToTheme()
{
	QObject::connect(this, SIGNAL(colorChanged(QColor)), this, SLOT(on_colorChanged(QColor)));
}
