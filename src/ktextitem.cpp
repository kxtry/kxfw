#include "stdafx.h"
#include "kwidget_p.h"
#include "ktextitem.h"
#include "kxmlui.h"
#include "kresource.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KTextItem)

class KTextItemPrivate : public KWidgetPrivate
{
public:
	QGraphicsTextItem * edit;
	bool bidealSizeAsMinimum;
};

KTextItem::KTextItem( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KTextItemPrivate(), parent)
{
	Q_D(KTextItem);
	d->edit = new QGraphicsTextItem(this);
	d->edit->setParent(this);
	init();
}

KTextItem::KTextItem( const QString& text, QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KTextItemPrivate(), parent)
{
	Q_D(KTextItem);
	d->edit = new QGraphicsTextItem(text, this);
	init();
}

KTextItem::~KTextItem()
{

}

void KTextItem::init()
{
	Q_D(KTextItem);

	setFlag(QGraphicsItem::ItemHasNoContents);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	d->bidealSizeAsMinimum = false;
	d->edit->setOpenExternalLinks(false);
	d->edit->setTextInteractionFlags(Qt::NoTextInteraction);
	d->edit->setAcceptedMouseButtons(0);
	d->edit->setAcceptHoverEvents(false);
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
}

void KTextItem::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	__super::resizeEvent(event);

	Q_D(KTextItem);
	d->edit->setPos(0, 0);
	d->edit->setTextWidth(width());
}

QString KTextItem::toHtml() const
{
	Q_D(const KTextItem);
	return d->edit->toHtml();
}


QString KTextItem::toText() const
{
	Q_D(const KTextItem);
	return d->edit->toPlainText();
}

void KTextItem::setText( const QString& txt )
{
	Q_D(KTextItem);
	if(Qt::mightBeRichText(txt))
	{
		d->edit->setHtml(txt);
	}
	else
	{
		d->edit->setPlainText(txt);
	}
}

QSizeF KTextItem::idealSize() const
{
	Q_D(const KTextItem);
	d->edit->adjustSize();
	QSizeF hitSize = d->edit->boundingRect().size();
	return hitSize;
}

QSizeF KTextItem::hitTextSize( int width )
{
	Q_D(KTextItem);

	d->edit->setTextWidth(width);
	QRectF hitRt = d->edit->boundingRect();
	return hitRt.size();
}

int KTextItem::_positionFromPoint( const QPoint &point )
{
	QPoint pt = point;
	QAbstractTextDocumentLayout *layout = document()->documentLayout();
	int index = layout->hitTest(pt, Qt::ExactHit);
	if (index != -1)
	{
		index++;
	}

	return index;
}

QTextCursor KTextItem::cursorFromPoint( const QPoint &point )
{
	int pos = _positionFromPoint(point);

	if ( pos == -1 )
	{
		return QTextCursor();
	}

	QTextCursor tcExact = textCursor();
	QTextDocument *doc = document();

	if ( pos >= doc->characterCount() )
	{
		pos = doc->characterCount() - 1;
	}

	tcExact.setPosition( pos );

	return tcExact;
}

bool KTextItem::isLinkObject(const QPoint &point)
{
	QString href = urlFromPoint(point);
	return !href.isEmpty();
}

QTextDocument * KTextItem::document()
{
	Q_D(KTextItem);
	return d->edit->document();
}

QTextCursor KTextItem::textCursor()
{
	Q_D(KTextItem);
	return d->edit->textCursor();
}

void KTextItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QPointF pt = event->pos();
	if(isLinkObject(pt.toPoint()))
	{
		setCursor(QCursor(Qt::PointingHandCursor));
	}
	else
	{
		setCursor(QCursor(Qt::ArrowCursor));
	}
}

void KTextItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	
}

void KTextItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QPointF pt = event->pos();
	QString url = urlFromPoint(pt.toPoint());
	if(!url.isEmpty())
	{
		QDesktopServices::openUrl(QUrl(url));
	}
}

void KTextItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{

}

void KTextItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
	QPointF pt = event->pos();
	if(isLinkObject(pt.toPoint()))
	{
		setCursor(QCursor(Qt::PointingHandCursor));
	}
	else
	{
		setCursor(QCursor(Qt::ArrowCursor));
	}
}

void KTextItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	setCursor(QCursor(Qt::ArrowCursor));
}

QString KTextItem::urlFromPoint( const QPoint& pt )
{
	int pos = _positionFromPoint(pt);
	if ( pos == -1 )
	{
		return false;
	}

	QTextCursor tcExact = textCursor();
	QTextDocument *doc = document();

	if ( pos >= doc->characterCount() )
	{
		pos = doc->characterCount() - 1;
	}

	tcExact.setPosition( pos );
	QTextCharFormat charFormat = tcExact.charFormat();
#if 0
	tcExact.select(QTextCursor::WordUnderCursor);
	QString name = charFormat.anchorName();ÓÀÔ¶Îª¿Õ
		QString txt = tcExact.selectedText();
	if(txt == "ËÑË÷ÉèÖÃ")
	{
		int i = 0;
	}	
#endif
	QString href = charFormat.anchorHref();
	return href;
}

KWidget::DragPolicy KTextItem::hitTest( const QPointF& point )
{
	QPointF pt = mapFromScene(point);
	if(isLinkObject(pt.toPoint()))
	{
		return KWidget::Nothing;
	}
	return KWidget::AskParent;
}

QColor KTextItem::textColor() const
{
	Q_D(const KTextItem);
	if(d->edit == NULL)
		return QColor();
	return d->edit->defaultTextColor();
}

void KTextItem::setTextColor( const QColor& txt )
{
	Q_D(KTextItem);
	d->edit->setDefaultTextColor(txt);
}

int KTextItem::fontSize()
{
	Q_D(KTextItem);
	return d->edit->font().pixelSize();
}

void KTextItem::setFontSize( int s )
{
	Q_D(KTextItem);
	QFont f = d->edit->font();
	f.setPixelSize(s);
	d->edit->setFont(f);
}

QString KTextItem::filePath() const
{
	return "";
}

void KTextItem::setFilePath( const QString& file )
{
	QByteArray data = KResource::loadRawData(file);
	setText(data);
}

QSizeF KTextItem::sizeHint( Qt::SizeHint which, const QSizeF & constraint /*= QSizeF() */ ) const
{
	Q_D(const KTextItem);
	QSizeF sz = __super::sizeHint(which, constraint);
	if(d->bidealSizeAsMinimum)
	{
		if(which == Qt::MinimumSize)
		{
			return idealSize();
		}
	}
	return sz;
}

bool KTextItem::idealSizeAsMinimun()
{
	Q_D(KTextItem);
	return d->bidealSizeAsMinimum;
}

void KTextItem::setIdealSizeAsMinimum( bool on )
{
	Q_D(KTextItem);
	d->bidealSizeAsMinimum = on;
}

QObject * KTextItem::graphicsText()
{
	Q_D(KTextItem);
	return d->edit;
}
