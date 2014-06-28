#include "stdafx.h"
#include "kimageitem.h"
#include "ktheme.h"
#include <qpainterpath.h>
#include "kxmlui.h"
#include "kwidget_p.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KImageItem)

class KImageItemPrivate: public KWidgetPrivate
{
public:
	QPixmap pixmap;
	QString imageFile;
	qreal scale;
	qreal rotate;
	bool bAutoAdjust;
	bool bHandHover;
	bool bAcceptClick;
	QColor effectColor;
	QColor borderColor;
	int borderPixel;
	quint32 imageAlignment;
	QMargins imageMargin;
};

KImageItem::KImageItem( QGraphicsItem *parent /*= NULL*/ ) 
: KWidget(*new KImageItemPrivate(), parent)
{
	Q_D(KImageItem);
	d->imageAlignment = Qt::AlignHCenter|Qt::AlignVCenter;
	d->bAutoAdjust = false;
	d->scale = 1.0;
	d->rotate = 0.0;
	d->borderPixel = 0;
	d->effectColor = QColor(255, 255, 255);
}

KImageItem::~KImageItem()
{

}

QString KImageItem::imagePath() const
{
	return d_func()->imageFile;
}

void KImageItem::setImagePath( const QString& file )
{
	Q_D(KImageItem);
	if(d->imageFile == file)
		return;
	d->imageFile = file;
	d->pixmap = KTheme::getPixmap(d->imageFile, hasTheme());
	update();
}

QPixmap KImageItem::pixmap() const
{
	return d_func()->pixmap;
}

void KImageItem::setPixmap( const QPixmap& pixmap )
{
	Q_D(KImageItem);
	d->imageFile.clear();
	d->pixmap = pixmap;
	update();
}

qreal KImageItem::scale() const
{
	return d_func()->scale;
}

void KImageItem::setScale( qreal factor )
{
	d_func()->scale = (1.0) * factor;
	update();
}

bool KImageItem::autoAdjust() const
{
	return d_func()->bAutoAdjust;
}

void KImageItem::setAutoAdjust(bool bAdjust)
{
	d_func()->bAutoAdjust = bAdjust;
	update();
}

bool KImageItem::handHover() const
{
	return d_func()->bHandHover;
}

void KImageItem::setHandHover( bool hover )
{
	d_func()->bHandHover = hover;
	setAcceptHoverEvents(hover);
	if(hover)
	{
		setCursor(Qt::PointingHandCursor);
	}
	else
	{
		unsetCursor();
	}
}

bool KImageItem::acceptClick() const
{
	return d_func()->bAcceptClick;
}

void KImageItem::setAcceptClick( bool bClick )
{
	d_func()->bAcceptClick = bClick;
	if(bClick)
	{
		setAcceptedMouseButtons(Qt::LeftButton);
	}
	else
	{
		setAcceptedMouseButtons(0);
	}
}

Qt::Alignment KImageItem::imageAlignment() const
{
	return d_func()->imageAlignment;
}

void KImageItem::setImageAlignment( Qt::Alignment align )
{
	d_func()->imageAlignment = align;
	update();
}

void KImageItem::setEffectColor( const QColor& color )
{
	d_func()->effectColor = color;
}

QColor KImageItem::effectColor() const
{
	return d_func()->effectColor;
}

QColor KImageItem::borderColor() const
{
	return d_func()->borderColor;
}

void KImageItem::setBorderColor( const QColor &color )
{
	d_func()->borderColor = color;
	update();
}


void KImageItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	painter->save();
	painter->setRenderHint(QPainter::SmoothPixmapTransform);
	painter->setClipRect(marginRect(), Qt::IntersectClip);
	if(d_func()->rotate != 0.0)
	{
		QPointF centerPt = rect().center();
		painter->translate(centerPt);
		painter->rotate(d_func()->rotate);
		painter->translate(-centerPt);
	}
	if(!d_func()->pixmap.isNull())
	{
		if((d_func()->imageAlignment & (Qt::AlignHCenter| Qt::AlignVCenter)) == (Qt::AlignHCenter| Qt::AlignVCenter))
		{
			drawHVCenter(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignLeft | Qt::AlignVCenter)) == (Qt::AlignLeft | Qt::AlignVCenter))
		{
			drawVCenter(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignRight | Qt::AlignVCenter)) == (Qt::AlignRight | Qt::AlignVCenter))
		{
			drawRVCenter(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignBottom | Qt::AlignHCenter)) == (Qt::AlignBottom | Qt::AlignHCenter))
		{
			drawBHCenter(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignTop | Qt::AlignHCenter)) == (Qt::AlignTop | Qt::AlignHCenter))
		{
			drawHCenter(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignLeft | Qt::AlignTop)) == (Qt::AlignLeft | Qt::AlignTop))
		{
			drawTopLeft(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignRight | Qt::AlignTop)) == (Qt::AlignRight | Qt::AlignTop))
		{
			drawTopRight(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignLeft | Qt::AlignBottom)) == (Qt::AlignLeft | Qt::AlignBottom))
		{
			drawBotLeft(painter, option, widget);
		}
		else if((d_func()->imageAlignment & (Qt::AlignRight | Qt::AlignBottom)) == (Qt::AlignRight | Qt::AlignBottom))
		{
			drawBotRight(painter, option, widget);
		}
		else if((d_func()->imageAlignment & Qt::AlignLeft) == Qt::AlignLeft)
		{
			drawTopLeft(painter, option, widget);
		}
		else if((d_func()->imageAlignment & Qt::AlignRight) == Qt::AlignRight)
		{
			drawTopRight(painter, option, widget);
		}
		else if((d_func()->imageAlignment & Qt::AlignBottom) == Qt::AlignBottom)
		{
			drawBotLeft(painter, option, widget);
		}
		else if((d_func()->imageAlignment & Qt::AlignHCenter) == Qt::AlignHCenter)
		{
			drawHCenter(painter, option, widget);
		}
		else if((d_func()->imageAlignment & Qt::AlignVCenter) == Qt::AlignVCenter)
		{
			drawVCenter(painter, option, widget);
		}
		else
		{
			drawHVCenter(painter, option, widget);
		}
	}
	drawBorder(painter, option, widget);
	painter->restore();
}

void KImageItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	event->setAccepted(d_func()->bAcceptClick);
	if(d_func()->bAcceptClick)
	{
		emit pressed(event->pos());
	}
}

void KImageItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	event->setAccepted(d_func()->bAcceptClick);
	if(d_func()->bAcceptClick)
	{
		emit pressed(event->pos());
		emit clicked();
	}
}

void KImageItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	if(handHover())
	{
		QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
		effect->setOffset(0, 0);
		effect->setBlurRadius(12);
		effect->setColor(d_func()->effectColor);
		setGraphicsEffect(effect);
	}

}

void KImageItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	if(handHover())
	{
		setGraphicsEffect(NULL);
	}
}

void KImageItem::drawBorder( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	if(!d->borderColor.isValid() || d->borderPixel <= 0)
		return;
	QPen pen(d->borderColor);
	pen.setWidth(1);
	painter->setPen(pen);
	QRect drawRt = marginRect();
	drawRt.adjust(d->borderPixel, d->borderPixel, -d->borderPixel, -d->borderPixel);
	painter->drawRect(drawRt);
}

void KImageItem::drawTopLeft( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left() + border, d->imageMargin.top() + border, drawSize.width(), drawSize.height());
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawTopRight( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate(marginRt.width() - drawSize.width() - border, border);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawBotLeft( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate(border, marginRt.height() - drawSize.height() - border);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawBotRight( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate(marginRt.width() - drawSize.width() - border, marginRt.height() - drawSize.height() - border);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawHCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate((marginRt.width() - drawSize.width()) / 2, border);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawVCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate(border, (marginRt.height() - drawSize.height()) / 2);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawHVCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate((marginRt.width() - drawSize.width()) / 2, (marginRt.height() - drawSize.height()) / 2);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawBHCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate((marginRt.width() - drawSize.width()) / 2, marginRt.height() - drawSize.height() - border);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

void KImageItem::drawRVCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KImageItem);
	QRectF marginRt = marginRect();
	int border = borderWidth();
	QSizeF drawSize = drawScaleSize(border);
	QRectF drawRt = QRectF(d->imageMargin.left(), d->imageMargin.top(), drawSize.width(), drawSize.height());
	drawRt.translate(marginRt.width() - drawSize.width() - border, (marginRt.height() - drawSize.height()) / 2);
	painter->drawPixmap(drawRt, d->pixmap, QRectF(0, 0, d->pixmap.width(), d->pixmap.height()));
}

QSizeF KImageItem::drawScaleSize(int border)
{
	Q_D(KImageItem);
	QSizeF drawSize = d->pixmap.size() * d->scale;
	if(d->bAutoAdjust)
	{
		QRectF targetRt = marginRect();
		qreal HScale = (targetRt.width() - border * 2) / d->pixmap.width();
		qreal VScale = (targetRt.height() - border * 2) / d->pixmap.height();
		drawSize = d->pixmap.size() * qMin(HScale, VScale);
	}
	return drawSize;
}

QMargins KImageItem::imageMargins() const
{
	return d_func()->imageMargin;
}

void KImageItem::setImageMargins( const QMargins &margin )
{
	d_func()->imageMargin = margin;
	update();
}

QSize KImageItem::pixmapSize()
{
	return d_func()->pixmap.size();
}

qreal KImageItem::rotation() const
{
	return d_func()->rotate;
}

void KImageItem::setRotation( qreal val )
{
	if(d_func()->rotate == val)
		return;
	d_func()->rotate = val;
	update();
}

int KImageItem::borderWidth()
{
	Q_D(KImageItem);
	return d->borderColor.isValid() ? (d->borderPixel) : (0);
}

QRect KImageItem::marginRect()
{
	Q_D(KImageItem);
	QRect winRt = rect().toRect();
	return winRt.adjusted(d->imageMargin.left(), d->imageMargin.top(), -d->imageMargin.right(), -d->imageMargin.bottom());
}

void KImageItem::doThemeChange()
{
	Q_D(KImageItem);
	d->pixmap = KTheme::getPixmap(imagePath(), hasTheme());
	d->pixmap.save("c:\\a.jpg");
	__super::doThemeChange();
	update();
}

int KImageItem::borderPixel()
{
	Q_D(KImageItem);
	return d->borderPixel;
}

void KImageItem::setBorderPixel( int pixel )
{
	Q_D(KImageItem);
	d->borderPixel = pixel;
}
