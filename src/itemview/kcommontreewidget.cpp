#include "stdafx.h"
#include "kcommontreewidget.h"
#include "kcommontreemodel.h"

#include "kxmlui.h"

#include "ktreewidget_p.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KCommonTreeWidget)

class KCommonTreeWidgetPrivate : public KTreeWidgetPrivate
{
	Q_DECLARE_PUBLIC(KCommonTreeWidget)
public:
	KCommonTreeModel *m_model;
public:
	KCommonTreeWidgetPrivate()
	{
		m_model = new KCommonTreeModel();
	}

	~KCommonTreeWidgetPrivate()
	{
		KX_SAFE_DELETE(m_model);
	}
};


KCommonTreeWidget::KCommonTreeWidget( QGraphicsItem *parent/*=NULL*/ )
 : KTreeWidget(*new KCommonTreeWidgetPrivate(),parent)
{
	Q_D(KCommonTreeWidget);
	setModel(d->m_model);
}

KCommonTreeWidget::~KCommonTreeWidget()
{
}

qint64 KCommonTreeWidget::nodeRoot()
{
	Q_D(KCommonTreeWidget);
	return d->m_model->nodeRoot();
}

qint64 KCommonTreeWidget::addChild( qint64 nodeid, const QString& text )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->addChild(nodeid, text);
}

qint64 KCommonTreeWidget::addChild( qint64 nodeid, const QString& text, const QVariant& other )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->addChild(nodeid, text, other);
}

qint64 KCommonTreeWidget::addChild( qint64 nodeid, const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->addChild(nodeid, icon, text, other);
}

qint64 KCommonTreeWidget::addChild( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->addChild(nodeid, vals);
}

qint64 KCommonTreeWidget::insertAfter( qint64 nodeid, const QString& text )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertAfter(nodeid, text);
}

qint64 KCommonTreeWidget::insertAfter( qint64 nodeid, const QString& text, const QVariant& other )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertAfter(nodeid, text, other);
}

qint64 KCommonTreeWidget::insertAfter( qint64 nodeid, const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertAfter(nodeid, icon, text, other);
}

qint64 KCommonTreeWidget::insertAfter( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertAfter(nodeid, vals);
}

qint64 KCommonTreeWidget::insertBefore( qint64 nodeid, const QString& text )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertBefore(nodeid, text);
}

qint64 KCommonTreeWidget::insertBefore( qint64 nodeid, const QString& text, const QVariant& other )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertBefore(nodeid, text, other);
}

qint64 KCommonTreeWidget::insertBefore( qint64 nodeid, const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertBefore(nodeid, icon, text, other);
}

qint64 KCommonTreeWidget::insertBefore( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->insertBefore(nodeid, vals);
}

void KCommonTreeWidget::removeNode( qint64 nodeid )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->removeNode(nodeid);
}

void KCommonTreeWidget::removeChilds( qint64 nodeid )
{
	Q_D(KCommonTreeWidget);
	return d->m_model->removeChilds(nodeid);
}

void KCommonTreeWidget::removeAll()
{
	Q_D(KCommonTreeWidget);
	return d->m_model->removeAll();
}

int KCommonTreeWidget::count() const
{
	Q_D(const KCommonTreeWidget);
	return d->m_model->count();
}

int KCommonTreeWidget::childCount( qint64 nodeid ) const
{
	Q_D(const KCommonTreeWidget);
	return d->m_model->childCount(nodeid);
}

