#include "stdafx.h"
#include "kbuttongroup.h"
#include "kradiobutton.h"
#include "kwidget_p.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KButtonGroup)

class KButtonGroupPrivate : public KWidgetPrivate
{
public:
	QList<KRadioButton*> lstRadio;
};

KButtonGroup::KButtonGroup( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KButtonGroupPrivate(), parent)
{
}

KButtonGroup::~KButtonGroup()
{

}

void KButtonGroup::childEvent(QChildEvent * event)
{
	__super::childEvent(event);
	if ( event->removed() )
	{
		QObject *button = event->child();
		if ( button )
		{
			d_func()->lstRadio.removeOne(reinterpret_cast<KRadioButton*>(button));
		}
	}
}

void KButtonGroup::on_button_state_changed()
{
	KRadioButton *radio = (KRadioButton*)sender();
	radio->setChecked(true);
	for(int i = 0; i < d_func()->lstRadio.count(); i++)
	{
		KRadioButton *button = d_func()->lstRadio[i];
		if(radio == button)
		{
			continue;
		}
		button->setChecked(false);
	}
	emit radioSelected(radio);
}

void KButtonGroup::addItem( KWidget *item )
{
	__super::addItem(item);
	KRadioButton *radio = qobject_cast<KRadioButton*>(item);
	if(radio)
	{
		addRadioButton(radio);
	}
}

void KButtonGroup::removeItem( KWidget *item )
{
	__super::removeItem(item);
}

void KButtonGroup::addRadioButton( KWidget* widget )
{
	KRadioButton *radio = qobject_cast<KRadioButton*>(widget);
	if(radio)
	{
		/*加入到Group中*/
		if(d_func()->lstRadio.contains(radio))
			return;
		connect(radio, SIGNAL(clicked()), this, SLOT(on_button_state_changed()));
		d_func()->lstRadio.push_back(radio);
	}
	else
	{
		QList<QGraphicsItem *> childs = widget->childItems();
		for(int i = 0; i < childs.count(); i++)
		{
			QGraphicsItem *item = childs[i];
			KWidget *widget = qobject_cast<KWidget*>(item->toGraphicsObject());
			if(widget)
			{
				addRadioButton(widget);
			}
		}
	}
}

void KButtonGroup::construct()
{
	__super::construct();
	/*枚举出子树中的Radio，直接加入到该树了。*/
	QList<QGraphicsItem *> childs = childItems();
	for(int i = 0; i < childs.count(); i++)
	{
		QGraphicsItem *item = childs[i];
		KWidget *widget = qobject_cast<KWidget*>(item->toGraphicsObject());
		if(widget)
		{
			addRadioButton(widget);
		}
	}
}
