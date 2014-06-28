#include "stdafx.h"
#include "kabstractlistmodel.h"

KAbstractListModel::KAbstractListModel( QObject *parent /*= 0*/ )
 : QObject(parent)
{

}

KAbstractListModel::~KAbstractListModel()
{

}

bool KAbstractListModel::setData( int index, const QMap<Qt::ItemDataRole, QVariant> &val )
{
	return false;
}

bool KAbstractListModel::setAction( int id, const QVariant& val, QVariant& result )
{
	return false;
}
