#include "stdafx.h"
#include "ktabwidget.h"
#include "kxmlui.h"
#include "kwidget_p.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KTabWidget)
KX_WIDGET_CREATOR_GLOBAL_STATIC(KTabItem)

class KTabWidgetPrivate : public KWidgetPrivate
{
public:
	KTabWidgetPrivate()
	{
		index = -1;
	}
	QList<KTabItem*> tabs;
	int index;
};

KTabWidget::KTabWidget( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KTabWidgetPrivate(), parent)
{

}

KTabWidget::~KTabWidget()
{

}

void KTabWidget::construct()
{
	Q_D(KTabWidget);
	__super::construct();
	/*枚举出子树中的Radio，直接加入到该树了。*/
	QList<QGraphicsItem *> childs = childItems();
	for(int i = 0; i < childs.count(); i++)
	{
		QGraphicsItem *item = childs[i];
		KTabItem *tab = qobject_cast<KTabItem*>(item->toGraphicsObject());
		if(tab)
		{
			addTabItem(tab);
		}
	}
	if(d->index >= 0 && d->index < d->tabs.count())
	{
		KTabItem *tab = d->tabs[d->index];
		tab->setChecked(true);
	}
}

void KTabWidget::addItem( KWidget *item )
{
	__super::addItem(item);
	KTabItem *tab = qobject_cast<KTabItem*>(item);
	if(tab)
	{
		addTabItem(tab);
	}
}

void KTabWidget::removeItem( KWidget *item )
{
	__super::removeItem(item);
}

void KTabWidget::childEvent( QChildEvent * event )
{
	Q_D(KTabWidget);

	__super::childEvent(event);

	if ( event->removed() )
	{
		QObject *tab = event->child();
		bool bok = d->tabs.removeOne(reinterpret_cast<KTabItem*>(tab));
		int count = d->tabs.count();
	}
}

void KTabWidget::on_tab_state_changed()
{
	Q_D(KTabWidget);

	KTabItem *hitTab = (KTabItem*)sender();
	doTabSelected(hitTab);

	emit tabSelected(hitTab);
	emit indexSelected(d->index);
}

void KTabWidget::addTabItem( KTabItem* tab )
{
	Q_D(KTabWidget);

	if(tab == NULL)
		return;
	if(d->tabs.contains(tab))
		return;
	connect(tab, SIGNAL(clicked()), this, SLOT(on_tab_state_changed()));
	d->tabs.push_back(tab);
}

int KTabWidget::selectIndex()
{
	Q_D(KTabWidget);
	return d->index;
}

void KTabWidget::setSelectIndex( int index )
{
	Q_D(KTabWidget);
	if(d->index == index)
		return;
	d->index = index;
	if(d->index < 0)
	{
		for(int i = 0; i < d->tabs.count(); i++)
		{
			KTabItem *tab = d->tabs[i];
			tab->setChecked(false);
		}
	}
	else if(d->index < d->tabs.count())
	{
		KTabItem *tab = d->tabs[d->index];
		//tab->setChecked(true);
		doTabSelected(tab);
		emit tabSelected(tab);
		emit indexSelected(index);
	}
}

void KTabWidget::doTabSelected( KTabItem* hitTab )
{
	Q_D(KTabWidget);
	hitTab->setChecked(true);
	for(int i = 0; i < d->tabs.count(); i++)
	{
		KTabItem *tab = d->tabs[i];
		if(tab == hitTab)
		{
			d->index = i;
			continue;
		}
		tab->setChecked(false);
	}
}

int KTabWidget::indexOfItem( KTabItem *hitTab )
{
	Q_D(KTabWidget);
	for(int i = 0; i < d->tabs.count(); i++)
	{
		KTabItem *tab = d->tabs[i];
		if(tab == hitTab)
		{
			return i;
		}
	}
	return -1;
}

KTabItem * KTabWidget::itemOfIndex( int idx )
{
	Q_D(KTabWidget);
	if(idx < 0 || idx >= d->tabs.count())
		return NULL;
	return d->tabs[idx];
}

int KTabWidget::itemCount()
{
	Q_D(KTabWidget);
	return d->tabs.count();
}


/*                   KTabItem                                      */


KTabItem::KTabItem( QGraphicsItem *parent/*=NULL*/ )
:KFrameItem(parent)
{
	setImagePath(":/image/theme/tabframe_unselect.png|:/image/theme/tabframe_hover.png|:/image/theme/tabframe_select.png|:/image/theme/tabframe_disable.png|"
		":/image/theme/tabframe_select.png|:/image/theme/tabframe_hover.png|:/image/theme/tabframe_unselect.png|:/image/theme/tabframe_disable.png|");
	setFrameType(GridFour);
	setAcceptHover(true);
	setAcceptClick(true);
}

KTabItem::~KTabItem()
{

}

void KTabItem::setImagePath( const QString& path )
{
	QString pathnew;
	QStringList imgs = path.split("|");
	switch(imgs.count())
	{
	case 2:
		{
			/*2态图，扩展成8态*/
			pathnew = QString("%1|%1|%1|%1|%2|%2|%2|%2").arg(imgs.at(0)).arg(imgs.at(1));
		}
		break;
	case 3:
		{
			/*3态图，扩展成8态*/
			pathnew = QString("%1|%2|%2|%1|%3|%3|%3|%3").arg(imgs.at(0)).arg(imgs.at(1)).arg(imgs.at(2));
		}
		break;
	case 6:
		{
			/*6态图，扩展成8态*/
			pathnew = QString("%1|%2|%3|%3|%4|%5|%6|%6").arg(imgs.at(0)).arg(imgs.at(1)).arg(imgs.at(2)).arg(imgs.at(3)).arg(imgs.at(4)).arg(imgs.at(5));
		}
		break;
	default:
		{
			pathnew = path;
		}
		break;
	}

	__super::setImagePath(pathnew);
}
