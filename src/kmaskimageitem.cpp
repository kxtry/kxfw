#include "stdafx.h"

#include "ktheme.h"
#include "kmaskimageitem.h"
#include "kwidget_p.h"
#include "kxmlui.h"

//////////////////////////////////////////////////////////////////////////

KX_WIDGET_CREATOR_GLOBAL_STATIC(KMaskImageItem)


class KMaskImageItemPrivate : public KWidgetPrivate
{
public:
	KMaskImageItemPrivate()
	{
		m_bMaskMose = false;
	}
	QPixmap m_pixmap;
    QPixmap m_maskPixmap;
	QString m_pixmapPath;
	QRegion m_maskRegion;
	bool m_bMaskMose;
};

KMaskImageItem::KMaskImageItem( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KMaskImageItemPrivate(), parent)
{
}

KMaskImageItem::~KMaskImageItem()
{

}

void KMaskImageItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_D(KMaskImageItem);
	if(d->m_pixmap.isNull())
		return;
	if(d->m_maskRegion.isEmpty())
	{
		painter->save();
		QRect drawRt(0, 0, width(), height());
		painter->setRenderHint(QPainter::HighQualityAntialiasing,true); //抗锯齿,可能有效率问题，先测试看看
		painter->drawPixmap(drawRt, d->m_pixmap);
		painter->restore();
	}
	else
	{
		painter->save();

        QImage dst = d->m_pixmap.scaled(d->m_maskPixmap.width(), d->m_maskPixmap.height()).toImage();

        if(dst.format() != QImage::Format_ARGB32) {
            dst = dst.convertToFormat(QImage::Format_ARGB32);
        }

        QPainter p(&dst);
        p.setCompositionMode(QPainter::CompositionMode_DestinationAtop);

        p.drawPixmap(dst.rect(), d->m_maskPixmap);

		painter->setRenderHint(QPainter::HighQualityAntialiasing,true);
        
        QRect drawRt(0, 0, width(), height());

        painter->drawImage(drawRt, dst);
		painter->restore();
	}
}

QString KMaskImageItem::maskImage() const
{
	return "";
}

void KMaskImageItem::setMaskImage( const QString& path )
{
	Q_D(KMaskImageItem);
	d->m_maskPixmap = KTheme::getPixmap(path);
	d->m_maskRegion = QRegion(d->m_maskPixmap.mask());
	QVector<QRect> rts = d->m_maskRegion.rects();
	int count =  rts.count();
}

QRect KMaskImageItem::maskRect() const
{
	return QRect();
}

void KMaskImageItem::setMaskRect( const QRect& rt )
{
	Q_D(KMaskImageItem);
	d->m_maskRegion = QRegion(rt, QRegion::Rectangle);
}

QRect KMaskImageItem::maskEllipse() const
{
	return QRect();
}

void KMaskImageItem::setMaskEllipse( const QRect& rt )
{
	Q_D(KMaskImageItem);
	d->m_maskRegion = QRegion(rt, QRegion::Ellipse);
}

QString KMaskImageItem::imagePath() const
{
	Q_D(const KMaskImageItem);
	return d->m_pixmapPath;
}

void KMaskImageItem::setImagePath( const QString& path )
{
	Q_D(KMaskImageItem);
	if(d->m_pixmapPath == path)
		return;
	d->m_pixmapPath = path;
	d->m_pixmap = KTheme::getPixmap(path, false);
}

QPixmap KMaskImageItem::pixmap() const
{
	Q_D(const KMaskImageItem);
	return d->m_pixmap;
}

void KMaskImageItem::setPixmap( const QPixmap& pixmap )
{
	Q_D(KMaskImageItem);
	d->m_pixmap = pixmap;
	d->m_pixmapPath = "";
	update();
}

bool KMaskImageItem::maskMouse() const
{
	Q_D(const KMaskImageItem);
	return d->m_bMaskMose;
}

void KMaskImageItem::setMaskMouse( bool bMask )
{
	Q_D(KMaskImageItem);
	d->m_bMaskMose = bMask;
	setAcceptHoverEvents(d->m_bMaskMose);
	setAcceptedMouseButtons(bMask ? (Qt::LeftButton|Qt::RightButton) : 0);
}

void KMaskImageItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	bool bFilter = filterMessage(event->pos());
	event->setAccepted(bFilter);
	if(bFilter)
	{
		emit mouseEvent(event);
	}
}

void KMaskImageItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	bool bFilter = filterMessage(event->pos());
	event->setAccepted(bFilter);
	if(bFilter)
	{
		emit mouseEvent(event);
	}
}

void KMaskImageItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	bool bFilter = filterMessage(event->pos());
	event->setAccepted(bFilter);
	if(bFilter)
	{
		emit mouseEvent(event);
	}
}

void KMaskImageItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	bool bFilter = filterMessage(event->pos());
	event->setAccepted(bFilter);
	if(bFilter)
	{
		emit mouseEvent(event);
	}
}

void KMaskImageItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	bool bFilter = filterMessage(event->pos());
	event->setAccepted(bFilter);
	if(bFilter)
	{
		emit mouseEvent(event);
	}
}

void KMaskImageItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	bool bFilter = filterMessage(event->pos());
	event->setAccepted(bFilter);
	if(bFilter)
	{
		emit mouseEvent(event);
	}
}

bool KMaskImageItem::filterMessage( QPointF pt )
{
	Q_D(KMaskImageItem);
	if(!d->m_bMaskMose)
		return false;
	return d->m_maskRegion.contains(pt.toPoint());
}

bool KMaskImageItem::getBatchRandomPoint( QList<QPoint>& pts, int count, QGraphicsItem *toItem )
{
	Q_D(KMaskImageItem);

	int seed = GetTickCount();
	qsrand(seed);
	pts.clear();
	if(d->m_maskRegion.isEmpty())
	{
		QRect br = rect().toRect();
		if(br.isEmpty())
			return false;
		int i = 0;
		while(i < count)
		{
			int x = qrand() % br.width();
			int y = qrand() % br.height();
			pts.push_back(QPoint(x,y));
			i++;
		}		
	}
	else
	{
		QRect br = d->m_maskRegion.boundingRect();
		if(br.isEmpty())
			return false;
		int i = 0;
		while(i < count)
		{
			int x = qrand() % br.width();
			int y = qrand() % br.height();
			if(d->m_maskRegion.contains(QPoint(x,y)))
			{
				pts.push_back(QPoint(x,y));
				i++;
			}
		}
	}

	if(toItem)
	{
		for(int i = 0; i < count; i++)
		{
			QPoint pt = pts[i];
			pts[i] = mapToItem(toItem, pt).toPoint();
		}
	}
	return true;
}

