#include "stdafx.h"
#include "kcommonlistwidget.h"
#include "kcommonlistmodel.h"
#include "klistwidget_p.h"

#include "kxmlui.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KCommonListWidget)

class KCommonListWidgetPrivate : public KListWidgetPrivate
{
public:
	KCommonListModel *m_model;
public:
	KCommonListWidgetPrivate()
	{
		m_model = new KCommonListModel();
	}

	~KCommonListWidgetPrivate()
	{
		KX_SAFE_DELETE(m_model);
	}
};


KCommonListWidget::KCommonListWidget( QGraphicsItem *parent/*=NULL*/ )
 : KListWidget(*new KCommonListWidgetPrivate(),parent)
{
	Q_D(KCommonListWidget);
	setModel(d->m_model);
}

KCommonListWidget::~KCommonListWidget()
{
}

void KCommonListWidget::addItem( const QString& text )
{
	Q_D(KCommonListWidget);

	d->m_model->addItem(text);
}

void KCommonListWidget::addItem( const QString& text, const QVariant& other )
{
	Q_D(KCommonListWidget);
	d->m_model->addItem(text, other);
}

void KCommonListWidget::addItem( const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KCommonListWidget);
	d->m_model->addItem(icon, text, other);
}

void KCommonListWidget::insertItem( int index, const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KCommonListWidget);
	d->m_model->insertItem(index, icon, text, other);
}

void KCommonListWidget::removeItem( const QString& text )
{
	Q_D(KCommonListWidget);
	d->m_model->removeItem(text);
}

void KCommonListWidget::removeItem( int index )
{
	Q_D(KCommonListWidget);
	d->m_model->removeItem(index);
}

void KCommonListWidget::removeAll()
{
	Q_D(KCommonListWidget);
	d->m_model->removeAll();
}

QString KCommonListWidget::itemsText() const
{
	Q_D(const KCommonListWidget);
	return d->m_model->itemsText();
}

void KCommonListWidget::setItemsText( const QString& items )
{
	Q_D(KCommonListWidget);
	d->m_model->setItemsText(items);
}

