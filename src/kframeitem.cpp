#include "stdafx.h"
#include "kframeitem.h"
#include "ktheme.h"
#include "kwidget_p.h"
#include "kxmlui.h"
#include "kutility.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KFrameItem)

class KFrameItemPrivate : public KWidgetPrivate
{
public:
	KFrameItemPrivate()
	{
		bSmooth = false;
		frameType = KFrameItem::OneTopLeft;
		index = 0;
		framePixel = 1;
		bFrameSwitch = true;
	}

	KFrameItem::FrameType frameType;
	QVector<QPixmap> vPixmap;//保存图片序列帧。
	QVector<QColor> vColor;
	int index;
	QString imagePath;
	QString frameColor;
	int framePixel;
	bool bSmooth;
	bool bFrameSwitch;
};

KFrameItem::KFrameItem( QGraphicsItem *parent /*= NULL*/ ) 
: KWidget(*new KFrameItemPrivate(), parent)
{
	init();
}

KFrameItem::KFrameItem( const QString& path, FrameType type, QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KFrameItemPrivate(), parent)
{
	init();
	setImagePath(path);
	setFrameType(type);
}

KFrameItem::~KFrameItem()
{

}

void KFrameItem::init()
{
	Q_D(KFrameItem);

	setFlag(QGraphicsItem::ItemClipsToShape);
}

QString KFrameItem::imagePath() const
{
	return d_func()->imagePath;
}

void KFrameItem::setImagePath( const QString& path )
{
	Q_D(KFrameItem);
	if(d->imagePath == path)
		return;
	d->imagePath = path;
	updateTheme();
}

void KFrameItem::appendPixmap( const QPixmap& img )
{
	Q_D(KFrameItem);
	d->vPixmap.push_back(img);
	update();
}

void KFrameItem::setFrameType( FrameType type )
{
	Q_D(KFrameItem);

	if(d->frameType == type)
		return;
	d->frameType = type;
	update();
}

KFrameItem::FrameType KFrameItem::frameType() const
{
	return d_func()->frameType;
}

QPixmap KFrameItem::currentPixmap()
{
	Q_D(KFrameItem);
	if(d->vPixmap.count())
	{
		return d->vPixmap[d->index];
	}
	return QPixmap();
}

void KFrameItem::setFrameIndex(int idx)
{
	Q_D(KFrameItem);
	idx = normalIndex(idx);
	if(idx == d->index)
		return;
	d->index = idx;
	update();
}

int KFrameItem::frameIndex() const
{
	Q_D(const KFrameItem);
    return d->index;
}

bool KFrameItem::smooth() const
{
	return d_func()->bSmooth;
}

void KFrameItem::setSmooth( bool on )
{
	if(d_func()->bSmooth == on)
		return;
	d_func()->bSmooth = on;
	update();
}

void KFrameItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	if(d->frameType < BitmapEnd)
	{
		if(d->vPixmap.size() <= 0)
			return;
		switch(d->frameType)
		{
		case OneTopLeft:			/*左上角*/
			drawOneTopLeft(painter, option, widget);
			break;
		case OneTopRight:		/*右上角*/
			drawOneTopRight(painter, option, widget);
			break;
		case OneBotLeft:			/*左下角*/
			drawOneBotLeft(painter, option, widget);
			break;
		case OneBotRight:		/*右下角*/
			drawOneBotRight(painter, option, widget);
			break;
		case OneHCenter:			/*水平中间*/
			drawOneHCenter(painter, option, widget);
			break;
		case OneVCenter:			/*竖向中间*/
			drawOneVCenter(painter, option, widget);
			break;
		case OneHVCenter:		/*正中心*/
			drawOneHVCenter(painter, option, widget);
			break;
		case TileH:			/*水平平铺一行*/
			drawTileH(painter, option, widget);
			break;
		case TileV:		/*垂直平铺一行*/
			drawTileV(painter, option, widget);
			break;
		case TileHV:		/*水平平铺多行，直致满屏*/
			drawTileHV(painter, option, widget);
			break;
		case StretchH:	/*水平拉伸，如果高度超出图片高度，则空白。*/
			drawStretchH(painter, option, widget);
			break;
		case StretchV:	/*垂直拉伸，如果宽度超出图片宽度，则空白。*/
			drawStretchV(painter, option, widget);
			break;
		case StretchHV:	/*水平垂直均拉伸，完全充满控件。*/
			drawStretchHV(painter, option, widget);
			break;
		case StretchHTwoSize:	/*水平拉伸图片两边的边线且仅拉一像素图像，中间不拉伸。*/
			drawStretchHTwo(painter, option, widget);
			break;
		case StretchHMiddle:
			drawStretchHMiddle(painter, option, widget);
			break;
		case StretchVTwoSize:
			drawStretchVTwo(painter, option, widget);
			break;
		case StretchVMiddle:
			drawStretchVMiddle(painter, option, widget);
			break;
		case GridFour:	/*4宫格*/		
			drawGridFour(painter, option, widget);
			break;
		case GridFrameFour:	/*空心4宫格*/		
			drawGridFrameFour(painter, option, widget);
			break;
		case GridNine:
			drawGridNine(painter, option, widget);
			break;
		}
	}
	else if(d->frameType < ColorFrameEnd)
	{
		if(d->vColor.size() <= 0)
			return;
		switch(d->frameType)
		{
		case Rectangle:
			drawRectangle(painter, option, widget);
			break;
		case RoundRect:
			drawRoundRect(painter, 3, option, widget);
			break;
		case RoundRect4Pixel:
			drawRoundRect(painter, 4, option, widget);
			break;
		case RoundRect5Pixel:
			drawRoundRect(painter, 5, option, widget);
			break;
		case RoundRect6Pixel:
			drawRoundRect(painter, 6, option, widget);
			break;
		case RoundRect7Pixel:
			drawRoundRect(painter, 7, option, widget);
			break;
		}
	}
}

void KFrameItem::drawOneTopLeft( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawOneTopLeft(painter, rect(), pixmap);
}

void KFrameItem::drawOneTopRight( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawOneTopRight(painter, rect(), pixmap);
}

void KFrameItem::drawOneBotLeft( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawOneBotLeft(painter, rect(), pixmap);
}

void KFrameItem::drawOneBotRight( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawOneBotRight(painter, rect(), pixmap);
}

void KFrameItem::drawOneHCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawOneHCenter(painter, rect(), pixmap);
}

void KFrameItem::drawOneVCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawOneVCenter(painter, rect(), pixmap);
}

void KFrameItem::drawOneHVCenter( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawOneHVCenter(painter, rect(), pixmap);
}

void KFrameItem::drawTileH( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawTileH(painter, rect(), pixmap);
}

void KFrameItem::drawTileV( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawTileV(painter, rect(), pixmap);
}

void KFrameItem::drawTileHV( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawTileHV(painter, rect(), pixmap);
}

void KFrameItem::drawStretchH( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawStretchH(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawStretchV( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawStretchV(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawStretchHV( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawStretchHV(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawStretchHTwo( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);

	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawStretchHTwo(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawStretchHMiddle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(KFrameItem);

	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawStretchHMiddle(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawStretchVTwo(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(KFrameItem);

	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawStretchVTwo(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawStretchVMiddle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_D(KFrameItem);

	QPixmap pixmap = d->vPixmap[d->index];

	KUtility::drawStretchVMiddle(painter, rect(), pixmap, d->bSmooth);
}

void KFrameItem::drawGridFour( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawGridFour(painter, rect(), pixmap);
}

void KFrameItem::drawGridFrameFour( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawGridFrameFour(painter, rect(), pixmap);
}

void KFrameItem::drawGridNine( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);

	QPixmap pixmap = d->vPixmap[d->index];
	KUtility::drawGridNine(painter, rect(), pixmap);
}

void KFrameItem::drawRectangle( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	painter->save();
	//painter->setRenderHint(QPainter::Antialiasing);
	QPen pen(d->vColor[d->index]);
	pen.setWidth(d->framePixel);
	//pen.setJoinStyle(Qt::RoundJoin);
	//pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);
	QRect rt = rect().toRect();
	rt.adjust(d->framePixel, d->framePixel, -d->framePixel, -d->framePixel);
	painter->drawRect(rt);
	painter->restore();
}

void KFrameItem::drawRoundRect( QPainter *painter, const int iRadio, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KFrameItem);
	painter->save();
	//painter->setRenderHint(QPainter::Antialiasing);
	QPen pen(d->vColor[d->index]);
	pen.setWidth(d->framePixel);
	//pen.setJoinStyle(Qt::RoundJoin);
	//pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);
	QRect rt = rect().toRect();
	rt.adjust(d->framePixel, d->framePixel, -d->framePixel, -d->framePixel);
	painter->drawRoundedRect(rt, iRadio, iRadio);
	painter->restore();
}

QSizeF KFrameItem::pixmapSize()
{
	Q_D(KFrameItem);
	return d->vPixmap[d->index].size();
}

int KFrameItem::frameCount()
{
	Q_D(KFrameItem);
	return d->vPixmap.count();
}

int KFrameItem::normalIndex( int idx )
{
	Q_D(KFrameItem);
	if(d->frameType < BitmapEnd)
	{
		if(idx >= d->vPixmap.count() || idx < 0)
			idx = 0;
	}
	else
	{
		if(idx >= d->vColor.count() || idx < 0)
			idx = 0;
	}
	return idx;
}

void KFrameItem::setNormal()
{
	FrameState state = hasChecked() ? CheckNormal : Normal;
	setFrameIndex((int)state);
}

void KFrameItem::setHover()
{
	FrameState state = hasChecked() ? CheckHover : Hover;
	setFrameIndex((int)state);
}

void KFrameItem::setDown()
{
	FrameState state = hasChecked() ? CheckDown : Down;
	setFrameIndex((int)state);
}

void KFrameItem::setSelected()
{
	FrameState state = hasChecked() ? CheckSelected : Selected;
	setFrameIndex((int)state);
}

void KFrameItem::setDisable()
{
	FrameState state = hasChecked() ? CheckDisable : Disable;
	setFrameIndex((int)state);
}

bool KFrameItem::hasChecked()
{
	QString name = QString("KFrameItem%1").arg((qlonglong)this);
	QVariant var = property(name.toStdString().data());
	return var.toBool();
}

void KFrameItem::setChecked( bool bCheck )
{
	QString name = QString("KFrameItem%1").arg((qlonglong)this);
	setProperty(name.toStdString().data(), bCheck);
	int idx = frameIndex();
	if(idx > Disable && !bCheck)
	{
		idx -= 4;
	}
	else if(idx <= Disable && bCheck)
	{
		idx += 4;
	}
	setFrameIndex(idx);
}

KFrameItem::FrameState KFrameItem::frameState() const
{
	int idx = frameIndex();
	if(idx > StateMax)
		idx = Normal;
	return (FrameState)idx;
}

void KFrameItem::setFrameState( FrameState state )
{
	bool bCheck = state > Disable ? true : false;
	setChecked(bCheck);
	setFrameIndex((int)state);
}

bool KFrameItem::acceptClick()
{
	Qt::MouseButtons mbs = acceptedMouseButtons();
	return mbs & Qt::LeftButton;
}

void KFrameItem::setAcceptClick( bool bClick )
{
	Qt::MouseButtons mbs = bClick ? Qt::LeftButton : Qt::NoButton;
	setAcceptedMouseButtons(mbs);
}

bool KFrameItem::acceptHover()
{
	return acceptHoverEvents();
}

void KFrameItem::setAcceptHover( bool bHover )
{
	setAcceptHoverEvents(bHover);
}

void KFrameItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	if(frameSwitch())
	{
		setDown();
	}
	emit pressed();
}

void KFrameItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	if(frameSwitch())
	{
		setNormal();
	}
	emit released();
	emit clicked();
}

void KFrameItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	if(frameSwitch())
	{
		setHover();
	}
	emit enter();
}

void KFrameItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if(frameSwitch())
	{
		setHover();
	}
}

void KFrameItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	if(frameSwitch())
	{
		setNormal();
	}
	emit leave();
}

void KFrameItem::setEnabled( bool enabled )
{
	__super::setEnabled(enabled);
	if(frameSwitch())
	{
		if(enabled)
		{
			setNormal();
		}
		else
		{
			setDisable();
		}
	}
}

QString KFrameItem::frameColor() const
{
	return d_func()->frameColor;
}

void KFrameItem::setFrameColor( const QString& clr )
{
	Q_D(KFrameItem);
	if(d->frameColor == clr)
		return;
	d->frameColor = clr;
	updateTheme();
}

int KFrameItem::framePixel()
{
	return d_func()->framePixel;
}

void KFrameItem::setFramePixel( int iPixel )
{
	d_func()->framePixel = iPixel;
}

bool KFrameItem::frameSwitch()
{
	return d_func()->bFrameSwitch;
}

void KFrameItem::setFrameSwitch( bool bSwitch )
{
	d_func()->bFrameSwitch = bSwitch;
}

void KFrameItem::doThemeChange()
{
	Q_D(KFrameItem);
	if(!hasTheme())
		return;
	updateTheme();
	__super::doThemeChange();
}

void KFrameItem::updateTheme()
{
	Q_D(KFrameItem);
	if(!d->imagePath.isEmpty())
	{
		QStringList items = d->imagePath.split('|');
		d->vPixmap.clear();
		for(int i = 0; i < items.count(); i++)
		{
			appendPixmap(KTheme::getPixmap(items[i], hasTheme()));
		}
	}
	else if(!d->frameColor.isEmpty())
	{
		QStringList lstclr = d->frameColor.split('|');
		for(int i = 0; i < lstclr.count(); i++)
		{
			const QString& val = lstclr.at(i);
			QColor color;
			QStringList s = val.split(',');
			if (s.size() >= 3)
			{
				int red = s[0].toInt();
				int green = s[1].toInt();
				int blue = s[2].toInt();
				int alpha = 255;
				if (s.size() >= 4)
				{
					alpha = s[3].toInt();
				}
				color.setRgb(red, green, blue, alpha);
				if(hasTheme())
				{
					color = KTheme::convertToThemeColor(color);
				}
				d->vColor.push_back(color);
			}
		}
	}
}

KFrameItem::DefaultFrame KFrameItem::defaultFrame() const
{
	Q_ASSERT_X(0, "KFrameItem::defaultFrame", "bad use.");
	return BadDefalut;
}

void KFrameItem::setDefaultFrame( DefaultFrame type )
{
	switch(type)
	{
	case RoundWhite:
		setImagePath(":/image/theme/roundframe_normal.png|:/image/theme/roundframe_hover.png|:/image/theme/roundframe_down.png");
		break;
	case RoundTranparent:
		setImagePath(":/image/theme/roundframe5_normal.png|:/image/theme/roundframe5_hover.png|:/image/theme/roundframe_down.png");
		break;
	case RoundFlatTranslucent:
		setImagePath(":/image/theme/roundframe2_normal.png|:/image/theme/roundframe2_hover.png|:/image/theme/roundframe_down.png");
		break;
	case RoundRaiseTranslucent:
		setImagePath(":/image/theme/roundframe3_normal.png|:/image/theme/roundframe3_hover.png|:/image/theme/roundframe_down.png");
		break;
	case RoundSunkenTranslucent:
		setImagePath(":/image/theme/roundframe4_normal.png|:/image/theme/roundframe4_hover.png|:/image/theme/roundframe_down.png");
		break;
	}
}

void KFrameItem::setFixSizeByPixmapSize()
{
	QPixmap pixmap = currentPixmap();
	setFixSize(pixmap.size());
}

void KFrameItem::setFixWidthByPixmapWidth()
{
	QPixmap pixmap = currentPixmap();
	setFixWidth(pixmap.width());
}

void KFrameItem::setFixHeightByPixmapHeight()
{
	QPixmap pixmap = currentPixmap();
	setFixHeight(pixmap.height());
}
