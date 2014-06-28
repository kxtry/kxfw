#include "stdafx.h"
#include "kabstracttreemodel.h"

KAbstractTreeModel::KAbstractTreeModel( QObject *parent /*= 0*/ )
: QObject(parent)
{

}

KAbstractTreeModel::~KAbstractTreeModel()
{

}

bool KAbstractTreeModel::setData( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant> &val )
{
	return false;
}

bool KAbstractTreeModel::setAction( int id, const QVariant& val, QVariant& result )
{
	return false;
}
