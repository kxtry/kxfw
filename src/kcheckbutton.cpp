#include "stdafx.h"
#include "kcheckbutton.h"
#include "kframeitem.h"
#include "klabel.h"
#include "kabstractbutton_p.h"
#include "kxmlui.h"

class KCheckButtonPrivate : public KAbstractButtonPrivate
{
public:
	KCheckButtonPrivate(KAbstractButton::ButtonType type)
		: KAbstractButtonPrivate(type)
	{

	}
	KFrameItem *check;
	KLabel *text;
};

KX_WIDGET_CREATOR_GLOBAL_STATIC(KCheckButton)

KCheckButton::KCheckButton( QGraphicsItem *parent ) 
: KAbstractButton(*new KCheckButtonPrivate(CheckButton), parent)
{
	Q_D(KCheckButton);
	setFlag(QGraphicsItem::ItemHasNoContents);

	setMargins(QMargins(2, 0, 0, 0));
	d->check = new KFrameItem(this);
	d->check->setParent(this);
	d->check->setImagePath(":/image/theme/checkbox_default_normal.png|"
		":/image/theme/checkbox_default_hover.png|"
		":/image/theme/checkbox_default_pressdown.png|"
		":/image/theme/checkbox_default_disable.png|"
		":/image/theme/checkbox_selected_normal.png|"
		":/image/theme/checkbox_selected_hover.png|"
		":/image/theme/checkbox_selected_pressdown.png|"
		":/image/theme/checkbox_selected_disable.png");
	d->check->setFrameType(KFrameItem::OneHVCenter);
	QSizeF s = d->check->pixmapSize();
	d->check->setFixWidth(s.width());
	d->text = NULL;
	setLayoutType(KWidget::HBox);
	addItem(d->check);
}

KCheckButton::~KCheckButton()
{

}

void KCheckButton::updateLayout()
{

}

void KCheckButton::updateState( ButtonStates states )
{
	Q_D(KCheckButton);
	
	d->check->setChecked(states.testFlag(KAbstractButton::StateChecked));
	if(states.testFlag(KAbstractButton::StateNormal))
	{
		d->check->setNormal();
	}
	else if(states.testFlag(KAbstractButton::StateHover))
	{
		d->check->setHover();
	}
	else if(states.testFlag(KAbstractButton::StatePressed))
	{
		d->check->setDown();
	}
	else if(states.testFlag(KAbstractButton::StateDisable))
	{
		d->check->setDisable();
	}
}

QString KCheckButton::text() const
{
	Q_D(const KCheckButton);
	if(d->text)
	{
		return d->text->text();
	}
	return QString();
}

void KCheckButton::setText( const QString& title )
{
	Q_D(KCheckButton);
	
	if(d->text)
	{
		d->text->setText(title);
		return;
	}

	d->text = new KLabel(this);
	d->text->setTextMargins(QMargins(3, 0, 0, 0));
	d->text->setText(title);
	addItem(d->text);
}

QString KCheckButton::imagePath() const
{
	return d_func()->check->imagePath();
}

void KCheckButton::setImagePath( const QString& file )
{
	Q_D(KCheckButton);
	QString pathnew;
	QStringList imgs = file.split("|");
	switch(imgs.count())
	{
	case 2:
		{
			/*2Ì¬Í¼£¬À©Õ¹³É8Ì¬*/
			pathnew = QString("%1|%1|%1|%1|%2|%2|%2|%2").arg(imgs.at(0)).arg(imgs.at(1));
		}
		break;
	case 3:
		{
			/*3Ì¬Í¼£¬À©Õ¹³É8Ì¬:unselect,unselect_hover,select*/
			pathnew = QString("%1|%2|%2|%1|%3|%3|%3|%3").arg(imgs.at(0)).arg(imgs.at(1)).arg(imgs.at(2));
		}
		break;
	case 6:
		{
			/*6Ì¬Í¼£¬À©Õ¹³É8Ì¬*/
			pathnew = QString("%1|%2|%3|%3|%4|%5|%6|%6").arg(imgs.at(0)).arg(imgs.at(1)).arg(imgs.at(2)).arg(imgs.at(3)).arg(imgs.at(4)).arg(imgs.at(5));
		}
		break;
	default:
		{
			pathnew = file;
		}
		break;
	}

	d->check->setImagePath(pathnew);
}

QMargins KCheckButton::textMargins() const
{
	Q_D(const KCheckButton);
	if(d->text)
	{
		return d->text->textMargins();
	}
	return QMargins();
}

void KCheckButton::setTextMargins( const QMargins& margin )
{
	d_func()->text->setTextMargins(margin);
}

int KCheckButton::imageWidth() const
{
	return d_func()->check->fixWidth();
}

void KCheckButton::setImageWidth(int s)
{
	d_func()->check->setFixWidth(s);
}

bool KCheckButton::autoAdjust()
{
	return d_func()->text->autoAdjust();
}

void KCheckButton::setAutoAdjust( bool bAdjust )
{
	Q_D(KCheckButton);

	QMargins m = margins();
	if(d->text)
	{
		d->text->setAutoAdjust(bAdjust);
	}
	if(d->check)
	{
		QSizeF s = d->check->pixmapSize();
		d->check->setFixWidth(s.width());
	}
	setWidthToChild(true);
}

bool KCheckButton::vertical()
{
	Q_D(KCheckButton);
	QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());
	if(boxLayout == NULL)
		return false;
	return boxLayout->orientation() == Qt::Vertical;
}

void KCheckButton::setVertical( bool on )
{
	Q_D(KCheckButton);
	QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());
	if(boxLayout == NULL)
		return;
	Qt::Orientation o = on ? Qt::Vertical : Qt::Horizontal;
	if(boxLayout->orientation() == o)
		return;
	boxLayout->setOrientation(o);
}

QObject * KCheckButton::label() const
{
	Q_D(const KCheckButton);
	return d->text;
}

QObject * KCheckButton::frame() const
{
	Q_D(const KCheckButton);
	return d->check;
}

bool KCheckButton::reverseOrder()
{
	return false;
}

void KCheckButton::setReverseOrder( bool on )
{
	QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());
	if(boxLayout == NULL)
		return;
	QGraphicsLayoutItem *item = boxLayout->itemAt(0);
	boxLayout->removeAt(0);
	boxLayout->addItem(item);
}
