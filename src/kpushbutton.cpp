#include "stdafx.h"
#include "kpushbutton.h"
#include "kframeitem.h"
#include "klabel.h"
#include "kimageitem.h"
#include "kspacer.h"
#include "kxmlui.h"
#include "kabstractbutton_p.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KPushButton)

class KPushButtonPrivate : public KAbstractButtonPrivate
{
public:
	KPushButtonPrivate(KAbstractButton::ButtonType type)
		: KAbstractButtonPrivate(type)
	{
		icon = NULL;
		text = NULL;
		textAlignment = Qt::AlignCenter;
		iconAlignment = Qt::AlignCenter;
		clickOffset = QPoint(1,1);
		iconSize = -1;
	}

	KFrameItem *frame;
	KSpacer *content;
	KImageItem *icon;
	KLabel *text;
	Qt::Alignment textAlignment;
	Qt::Alignment iconAlignment;
	int iconSize;
	QMargins iconMargins;
	QMargins textMargins;
	QPoint clickOffset;
};

KPushButton::KPushButton( QGraphicsItem *parent /*= NULL*/ ) 
: KAbstractButton(*new KPushButtonPrivate(KAbstractButton::PushButton), parent)
{
	Q_D(KPushButton);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	d->frame = new KFrameItem(this);
	d->frame->setObjectName("PushButtonBk");
	d->frame->setFrameType(KFrameItem::GridFour);
	d->frame->setZValue(1.0);
	d->frame->setImagePath(":/image/theme/pushbutton_normal.png|:/image/theme/pushbutton_hover.png|"
							":/image/theme/pushbutton_pressdown.png|:/image/theme/pushbutton_disable.png");
	d->content = new KSpacer(this);
	d->content->setLayoutType(KWidget::HBox);
	d->content->setZValue(2.0);
}

KPushButton::~KPushButton()
{

}

void KPushButton::updateLayout()
{
	Q_D(KPushButton);
	d->frame->setGeometry(0, 0, width(), height());
	d->content->setFixSize(size());
	d->content->setGeometry(0, 0, width(), height());
}

void KPushButton::updateState( ButtonStates states )
{
	Q_D(KPushButton);

	if(states.testFlag(KAbstractButton::StateNormal))
	{
		d->frame->setFrameIndex(0);
		d->content->setPos(0, 0);
	}
	else if(states.testFlag(KAbstractButton::StateHover))
	{
		d->frame->setFrameIndex(1);
	}
	else if(states.testFlag(KAbstractButton::StatePressed))
	{
		d->frame->setFrameIndex(2);
		d->content->setPos(d->clickOffset);
	}
	else if(states.testFlag(KAbstractButton::StateDisable))
	{
		d->frame->setFrameIndex(3);
	}
}

QString KPushButton::text() const
{
	Q_D(const KPushButton);

	if(d->text == NULL)
		return QString();
	return d->text->text();
}

void KPushButton::setText( const QString& text )
{
	Q_D(KPushButton);
	
	if(d->text)
	{
		d->text->setText(text);
		return;
	}

	d->text = new KLabel(d->content);
	d->text->setText(text);
	d->content->addItem(d->text);
	d->text->setTextAlignment(d->textAlignment);
}

QString KPushButton::iconPath() const
{
	Q_D(const KPushButton);

	if(d->icon == NULL)
		return QString();
	return d->icon->imagePath();
}

void KPushButton::setIconPath( const QString& file )
{
	Q_D(KPushButton);

	if(d->icon)
	{
		d->icon->setImagePath(file);
		return;
	}
	d->icon = new KImageItem(d->content);
	d->icon->setImagePath(file);
	QPixmap pixmap = d->icon->pixmap();
	if(d->iconSize < 0)
	{
		d->iconSize = pixmap.width();
	}
	if(orientation() == Qt::Horizontal)
	{
		d->icon->setFixWidth(d->iconSize);
	}
	else
	{
		d->icon->setFixHeight(d->iconSize);
	}
	d->icon->setImageMargins(d->iconMargins);
	d->content->addItem(d->icon);
	d->icon->setImageAlignment(d->iconAlignment);
}

QString KPushButton::imagePath() const
{
	Q_D(const KPushButton);

	return d->frame->imagePath();
}

void KPushButton::setImagePath( const QString& file )
{
	Q_D(KPushButton);

	if(file == d->frame->imagePath())
		return;
	d->frame->setImagePath(file);
}

Qt::Orientation KPushButton::orientation()
{
	Q_D(KPushButton);

	QGraphicsLinearLayout *linear = dynamic_cast<QGraphicsLinearLayout*>(d->content->layout());
	if(linear == NULL)
		return Qt::Horizontal;
	return linear->orientation();
}

void KPushButton::setOrientation( Qt::Orientation or )
{
	Q_D(KPushButton);

	QGraphicsLinearLayout *linear = dynamic_cast<QGraphicsLinearLayout*>(d->content->layout());
	if(linear == NULL)
		return ;
	return linear->setOrientation(or);	
}

Qt::Alignment KPushButton::textAlignment()
{
	Q_D(KPushButton);

	return d->textAlignment;
}

void KPushButton::setTextAlignment( Qt::Alignment align )
{
	Q_D(KPushButton);

	d->textAlignment = align;
	if(d->text)
	{
		d->text->setTextAlignment(d->textAlignment);
	}
}

Qt::Alignment KPushButton::iconAlignment()
{
	Q_D(KPushButton);

	return d->iconAlignment;
}

void KPushButton::setIconAlignment( Qt::Alignment align )
{
	Q_D(KPushButton);

	d->iconAlignment = align;
	if(d->icon)
	{
		d->icon->setImageAlignment(d->iconAlignment);
	}
}

int KPushButton::iconSize() const
{
	Q_D(const KPushButton);

	return d->iconSize;
}

void KPushButton::setIconSize( int s )
{
	Q_D(KPushButton);

	d->iconSize = s;
	if(d->icon)
	{
		if(orientation() == Qt::Horizontal)
		{
			d->icon->setFixWidth(s);
		}
		else
		{
			d->icon->setFixHeight(s);
		}
	}
}

QMargins KPushButton::iconMargins() const
{
	Q_D(const KPushButton);

	return d->iconMargins;
}

void KPushButton::setIconMargins( const QMargins& margin )
{
	Q_D(KPushButton);

	d->iconMargins = margin;
	if(d->icon = NULL)
		return;
	d->icon->setImageMargins(margin);
}

void KPushButton::setClickOffset( const QPoint& offset )
{
	Q_D(KPushButton);

	d->clickOffset = offset;
}

QPoint KPushButton::clickOffset() const
{
	Q_D(const KPushButton);

	return d->clickOffset;
}

void KPushButton::setSizeToFrameImage()
{
	Q_D(KPushButton);

	QGraphicsLinearLayout *linear = dynamic_cast<QGraphicsLinearLayout*>(parentLayout());
	if(linear == NULL)
		return;
	QSizeF s = d->frame->pixmapSize();
	setFixSize(s);
}

void KPushButton::setSizeToContent(const QMargins& m)
{
	Q_D(KPushButton);

	QGraphicsLinearLayout *linear = dynamic_cast<QGraphicsLinearLayout*>(parentLayout());
	if(linear == NULL)
		return;
	if(d->text)
	{
		d->text->setAutoAdjust(true);
	}
	d->content->setContentsMargins(m.left(), m.top(), m.right(), m.bottom());
	d->content->setSizeToChild(true);
	QSizeF s = d->content->minimumSize();
	setFixWidth(s.width());
}

QMargins KPushButton::textMargins() const
{
	Q_D(const KPushButton);
	return d->textMargins;
}

void KPushButton::setTextMargins( const QMargins& m )
{
	Q_D(KPushButton);
	d->textMargins = m;
	if(d->text == NULL)
		return;
	d->text->setTextMargins(m);
}

void KPushButton::addItem( KWidget *item )
{
	Q_D(KPushButton);
	d->content->addItem(item);
}

void KPushButton::removeItem( KWidget *item )
{
	Q_D(KPushButton);
	d->content->removeItem(item);
}

void KPushButton::setFont( const QFont& f )
{
	Q_D(KPushButton);
	if(d->text == NULL)
		return;
	d->text->setFont(f);
}

int KPushButton::fontSize()
{
	Q_D(KPushButton);
	if(d->text == NULL)
		return 0;
	return d->text->fontSize();
}

void KPushButton::setFontSize( int fs )
{
	Q_D(KPushButton);
	if(d->text == NULL)
		return;
	d->text->setFontSize(fs);
}

bool KPushButton::textBold()
{
	Q_D(KPushButton);
	if(d->text == NULL)
		return false;
	return d->text->bold();
}

void KPushButton::setTextBold( bool on )
{
	Q_D(KPushButton);
	if(d->text == NULL)
		return;
	d->text->setBold(on);
}

QObject * KPushButton::buttonFrame() const
{
	Q_D(const KPushButton);
	return d->frame;
}

QObject * KPushButton::buttonIcon() const
{
	Q_D(const KPushButton);
	return d->icon;
}

QObject * KPushButton::buttonText() const
{
	Q_D(const KPushButton);
	return d->text;
}

QColor KPushButton::textColor() const
{
	Q_D(const KPushButton);
	if(d->text == NULL)
		return QColor();

	return d->text->textColor();
}

void KPushButton::setTextColor( const QColor& clr )
{
	Q_D(const KPushButton);
	if(d->text)
	{
		d->text->setTextColor(clr);
	}
}
