#include "stdafx.h"
#include "kcommonlistmodel.h"

KCommonListModel::KCommonListModel( QObject *parent/*=NULL*/ )
 : KAbstractListModel(parent)
 , m_bBatchInserted(false)
{

}

KCommonListModel::~KCommonListModel()
{

}

int KCommonListModel::count() const
{
	return m_items.size();
}

QMap<Qt::ItemDataRole, QVariant> KCommonListModel::data( int index ) const
{
	if(index < 0 || index >= m_items.size())
		return QMap<Qt::ItemDataRole, QVariant>();
	return m_items[index];
}

void KCommonListModel::addItem( const QString& text )
{
	QMap<Qt::ItemDataRole, QVariant> val;
	val.insert(Qt::DisplayRole, text);
	int index = m_items.size();
	m_items.push_back(val);
	if(m_bBatchInserted)
	{
		if(m_iBatchInsertedBegin < 0)
		{
			m_iBatchInsertedBegin = index;
		}
		m_nBatchInserted++;
	}
	else
	{
		emit itemsInserted(index, 1);
	}
}

void KCommonListModel::addItem( const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> val;
	val.insert(Qt::DisplayRole, text);
	if(other.isValid())
	{
		val.insert((Qt::ItemDataRole)UserRoleOther, other);
	}
	int index = m_items.size();
	m_items.push_back(val);
	if(m_bBatchInserted)
	{
		if(m_iBatchInsertedBegin < 0)
		{
			m_iBatchInsertedBegin = index;
		}
		m_nBatchInserted++;
	}
	else
	{
		emit itemsInserted(index, 1);
	}
}

void KCommonListModel::addItem( const QString& icon, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> val;
	val.insert(Qt::DisplayRole, text);
	if(icon.isEmpty())
	{
		val.insert(Qt::DecorationRole, icon);
	}
	if(other.isValid())
	{
		val.insert((Qt::ItemDataRole)UserRoleOther, other);
	}
	int index = m_items.size();
	m_items.push_back(val);
	if(m_bBatchInserted)
	{
		if(m_iBatchInsertedBegin < 0)
		{
			m_iBatchInsertedBegin = index;
		}
		m_nBatchInserted++;
	}
	else
	{
		emit itemsInserted(index, 1);
	}
}

void KCommonListModel::insertItem( int index, const QString& icon, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> val;
	val.insert(Qt::DisplayRole, text);
	if(icon.isEmpty())
	{
		val.insert(Qt::DecorationRole, icon);
	}
	if(other.isValid())
	{
		val.insert((Qt::ItemDataRole)UserRoleOther, other);
	}
	m_items.insert(index, val);
	if(m_bBatchInserted)
	{
		if(m_iBatchInsertedBegin < 0)
		{
			m_iBatchInsertedBegin = index;
		}
		m_nBatchInserted++;
	}
	else
	{
		emit itemsInserted(index, 1);
	}
}

void KCommonListModel::removeItem( const QString& text )
{
	for(int i = 0; i < m_items.size(); i++)
	{
		QMap<Qt::ItemDataRole, QVariant>& val = m_items[i];
		QString hitText = val.value(Qt::DisplayRole).toString();
		if(hitText.compare(text) == 0)
		{
			m_items.removeAt(i);
			emit itemsRemoved(i, 1);
			return;
		}
	}
}

void KCommonListModel::removeItem( int index )
{
	if(index < 0 || index >= m_items.size())
		return;

	m_items.removeAt(index);
	emit itemsRemoved(index, 1);
}

void KCommonListModel::removeAll()
{
	int n = m_items.size();
	m_items.clear();

	emit modelReset();
}

QString KCommonListModel::itemsText() const
{
	QString outTxt;
	for(int i = 0; i < m_items.size(); i++)
	{
		QMap<Qt::ItemDataRole, QVariant> val = m_items[i];
		QString text = val.value(Qt::DisplayRole).toString();
		if(i == m_items.size() - 1)
		{
			outTxt += text;
		}
		else
		{
			outTxt += text;
			outTxt += "|";
		}
	}
	return outTxt;
}

void KCommonListModel::setItemsText( const QString& items )
{
	m_items.clear();
	QStringList itemsTxt = items.split('|');
	for(int i = 0; i < itemsTxt.count(); i++)
	{
		QMap<Qt::ItemDataRole,QVariant> vals;
		vals.insert(Qt::DisplayRole, itemsTxt.at(i));
		m_items.push_back(vals);
	}
	emit modelReset();
}

void KCommonListModel::batchInsert()
{
	m_bBatchInserted = true;
	m_nBatchInserted = 0;
	m_iBatchInsertedBegin = -1;
}

int KCommonListModel::batchCommit()
{
	m_bBatchInserted = false;
	if(m_nBatchInserted > 0)
	{
		emit itemsInserted(m_iBatchInsertedBegin, m_nBatchInserted);
	}
	return m_nBatchInserted;
}
