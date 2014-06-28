#include "stdafx.h"
#include "kcommontreemodel.h"


KCommonTreeModel::KCommonTreeModel( QObject *parent/*=NULL*/ )
: KAbstractTreeModel(parent)
, m_bBatchInserted(false)
{

}

KCommonTreeModel::~KCommonTreeModel()
{

}

int KCommonTreeModel::childCount( qint64 nodeid ) const
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		return n->childs.count();
	}
	return m_roots.count();
}

QMap<Qt::ItemDataRole, QVariant> KCommonTreeModel::data( qint64 nodeid ) const
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		return n->data;
	}
	return QMap<Qt::ItemDataRole, QVariant>();
}

qint64 KCommonTreeModel::parent( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		Node *np = n->parent;
		if(np)
		{
			return np->nodeid;
		}
		return 0;
	}	
	return -1;
}

QList<qint64> KCommonTreeModel::childs( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	QList<qint64> ids;
	if(n)
	{
		for(int i = 0; i < n->childs.size(); i++)
		{
			const Node *c = n->childs.at(i);
			ids.push_back(c->nodeid);
		}
	}
	return ids;
}

qint64 KCommonTreeModel::firstChild( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		if(n->childs.isEmpty())
			return -1;
		Node *c = n->childs.first();
		return c->nodeid;
	}	
	return -1;
}

qint64 KCommonTreeModel::nextSibling( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		Node *np = n->parent;
		if(np == NULL)
		{
			int i = m_roots.indexOf(n);
			if(i < m_roots.size()-1)
			{
				Node *ns = m_roots.at(i+1);
				return ns->nodeid;
			}
			return -1;
		}
		int i = np->childs.indexOf(n);
		if(i < np->childs.size()-1)
		{
			Node *ns = np->childs.at(i+1);
			return ns->nodeid;
		}
		return -1;
	}	
	return -1;
}

qint64 KCommonTreeModel::previousSibling( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		Node *np = n->parent;
		if(np == NULL)
		{
			if(m_roots.first() == n)
				return -1;
			int i = m_roots.indexOf(n);
			Node *ns = m_roots.at(i-1);
			return ns->nodeid;
		}
		if(np->childs.front() == n)
			return -1;
		int i = np->childs.indexOf(n);
		Node *ns = np->childs.at(i-1);
		return ns->nodeid;
	}	
	return -1;
}

qint64 KCommonTreeModel::nodeId()
{
	//1395038338484
	static int icount = 0;
	qint64 msec = QDateTime::currentMSecsSinceEpoch();
	msec <<= 16;
	msec += icount++;
	return msec;
}

qint64 KCommonTreeModel::nodeRoot() const
{
	return 0;
}

qint64 KCommonTreeModel::addChild( qint64 nodeid, const QString& text )
{
	QMap<Qt::ItemDataRole, QVariant> vals;
	vals.insert(Qt::DisplayRole, text);
	return addChild(nodeid, vals);
}

qint64 KCommonTreeModel::addChild( qint64 nodeid, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> vals;
	vals.insert(Qt::DisplayRole, text);
	vals.insert((Qt::ItemDataRole)UserRoleOther, other);
	return addChild(nodeid, vals);
}

qint64 KCommonTreeModel::addChild( qint64 nodeid, const QString& icon, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> vals;
	vals.insert(Qt::DecorationRole, icon);
	vals.insert(Qt::DisplayRole, text);
	vals.insert((Qt::ItemDataRole)UserRoleOther, other);
	return addChild(nodeid, vals);
}

qint64 KCommonTreeModel::addChild( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals )
{
	if(nodeid == 0)
	{
		Node *n = new Node;
		n->data = vals;
		n->nodeid = nodeId();
		m_roots.push_back(n);
		m_nodes.insert(n->nodeid, n);

		QList<qint64> ids;
		ids.push_back(n->nodeid);
		if(m_bBatchInserted)
		{
			m_batchInserted.push_back(n->nodeid);
		}
		else
		{
			emit itemsInserted(ids);
		}

		return n->nodeid;
	}
	if(m_nodes.contains(nodeid))
	{
		Node *nodeParent = m_nodes[nodeid];
		Node *n = new Node;
		n->data = vals;
		n->nodeid = nodeId();
		n->parent = nodeParent;
		nodeParent->childs.push_back(n);
		m_nodes.insert(n->nodeid, n);

		QList<qint64> ids;
		ids.push_back(n->nodeid);
		if(m_bBatchInserted)
		{
			m_batchInserted.push_back(n->nodeid);
		}
		else
		{
			emit itemsInserted(ids);
		}
		return n->nodeid;
	}
	return 0;
}

qint64 KCommonTreeModel::insertAfter( qint64 nodeid, const QString& text )
{
	QMap<Qt::ItemDataRole, QVariant> vals;
	vals.insert(Qt::DisplayRole, text);
	return insertAfter(nodeid, vals);
}

qint64 KCommonTreeModel::insertAfter( qint64 nodeid, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> vals;
	
	vals.insert(Qt::DisplayRole, text);
	vals.insert((Qt::ItemDataRole)UserRoleOther, other);

	return insertAfter(nodeid, vals);
}

qint64 KCommonTreeModel::insertAfter( qint64 nodeid, const QString& icon, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> vals;

	vals.insert(Qt::DecorationRole, icon);
	vals.insert(Qt::DisplayRole, text);
	vals.insert((Qt::ItemDataRole)UserRoleOther, other);

	return insertAfter(nodeid, vals);
}

qint64 KCommonTreeModel::insertAfter( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals )
{
	if(m_nodes.contains(nodeid))
	{
		Node *noderef = m_nodes[nodeid];
		Node *nodeParent = noderef->parent;
		Node *n = new Node;
		n->data = vals;
		n->nodeid = nodeId();
		n->parent = nodeParent;
		if(nodeParent == NULL)
		{
			int i = m_roots.indexOf(noderef);
			m_roots.insert(i+1, n);
		}
		else
		{
			int i = nodeParent->childs.indexOf(noderef);
			nodeParent->childs.insert(i+1, n);
		}
		m_nodes.insert(n->nodeid, n);

		QList<qint64> ids;
		ids.push_back(n->nodeid);
		if(m_bBatchInserted)
		{
			m_batchInserted.push_back(n->nodeid);
		}
		else
		{
			emit itemsInserted(ids);
		}
		return n->nodeid;
	}
	return 0;
}

qint64 KCommonTreeModel::insertBefore( qint64 nodeid, const QString& text )
{
	QMap<Qt::ItemDataRole, QVariant> vals;

	vals.insert(Qt::DisplayRole, text);

	return insertBefore(nodeid, vals);
}

qint64 KCommonTreeModel::insertBefore( qint64 nodeid, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> vals;

	vals.insert(Qt::DisplayRole, text);
	vals.insert((Qt::ItemDataRole)UserRoleOther, other);

	return insertBefore(nodeid, vals);
}

qint64 KCommonTreeModel::insertBefore( qint64 nodeid, const QString& icon, const QString& text, const QVariant& other )
{
	QMap<Qt::ItemDataRole, QVariant> vals;

	vals.insert(Qt::DecorationRole, icon);
	vals.insert(Qt::DisplayRole, text);
	vals.insert((Qt::ItemDataRole)UserRoleOther, other);

	return insertBefore(nodeid, vals);
}

qint64 KCommonTreeModel::insertBefore( qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals )
{
	if(m_nodes.contains(nodeid))
	{
		Node *noderef = m_nodes[nodeid];
		Node *nodeParent = noderef->parent;
		Node *n = new Node;
		n->data = vals;
		n->nodeid = nodeId();
		n->parent = nodeParent;
		if(nodeParent == NULL)
		{
			int i = m_roots.indexOf(noderef);
			m_roots.insert(i, n);
		}
		else
		{
			int i = nodeParent->childs.indexOf(noderef);
			nodeParent->childs.insert(i, n);
		}
		m_nodes.insert(n->nodeid, n);

		QList<qint64> ids;
		ids.push_back(n->nodeid);
		if(m_bBatchInserted)
		{
			m_batchInserted.push_back(n->nodeid);
		}
		else
		{
			emit itemsInserted(ids);
		}
		return n->nodeid;
	}
	return 0;
}

void KCommonTreeModel::removeNode( qint64 nodeid )
{
	if(m_nodes.contains(nodeid))
	{
		ItemCall tic;
		enumTree(nodeid, &tic);

		Node *n = m_nodes[nodeid];
		Node *np = n->parent;
		if(np == NULL)
		{
			m_roots.removeOne(n);
		}
		else
		{
			np->childs.removeOne(n);
		}
		delete n;

		QList<qint64> ids = tic.m_itemsResult;
		for(int i = 0; i < ids.size(); i++)
		{
			m_nodes.remove(ids.at(i));
		}
		emit itemsRemoved(ids);
	}
}

void KCommonTreeModel::removeChilds( qint64 nodeid )
{
	if(m_nodes.contains(nodeid))
	{
		ItemCall tic;
		enumTree(nodeid, &tic);

		Node *n = m_nodes[nodeid];
		for(int i = 0; i < n->childs.count(); i++)
		{
			Node *c = n->childs[i];
			m_nodes.remove(c->nodeid);
			delete c;
		}
		n->childs.clear();

		QList<qint64> ids = tic.m_itemsResult;
		ids.removeOne(nodeid);
		for(int i = 0; i < ids.size(); i++)
		{
			m_nodes.remove(ids.at(i));
		}
		emit itemsRemoved(ids);
	}
}

void KCommonTreeModel::removeAll()
{
	for(int i = 0; i < m_roots.count(); i++)
	{
		Node *r = m_roots[i];
		delete r;
	}
	m_roots.clear();
	m_nodes.clear();

	emit modelReset();
}

int KCommonTreeModel::count() const
{
	return m_nodes.size();
}

void KCommonTreeModel::enumTree( qint64 nodeid, ItemCall* cb )
{
	if(nodeid == 0)
	{
		for(int i = 0; i < m_roots.count(); i++)
		{
			Node *c = m_roots[i];
			enumTree(c->nodeid, cb);
		}
		return;
	}
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		ItemCall::NextOP op = cb->walkAt(nodeid, n->childs.size());
		if(op == ItemCall::eWalkChild)
		{
			for(int i = 0; i < n->childs.size(); i++)
			{
				Node *c = n->childs[i];
				enumTree(c->nodeid, cb);
			}
		}
	}
}

int KCommonTreeModel::nodeDepth( qint64 nodeid ) const
{
	if(nodeid == 0)
	{
		return 0;
	}
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
	{
		return 0;
	}
	int ndepth = 1;
	while(n && n->parent)
	{
		ndepth++;
		n = n->parent;
	}
	return ndepth;
}

bool KCommonTreeModel::hasNextSibling( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		Node *np = n->parent;
		if(np == NULL)
		{
			return n != m_roots.back();
		}
		return n != np->childs.back();
	}
	return false;
}

bool KCommonTreeModel::hasPreviousSibling( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n)
	{
		Node *np = n->parent;
		if(np == NULL)
		{
			return n != m_roots.front();
		}
		return np->childs.front() != n;
	}
	return false;
}

bool KCommonTreeModel::moveToLast( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = n->parent;
	if(np == NULL)
	{
		int i = m_roots.indexOf(n);
		m_roots.removeOne(n);
		m_roots.insert(i-1, n);
		emit modelReset();
		return true;
	}
	int i = np->childs.indexOf(n);
	np->childs.removeOne(n);
	np->childs.insert(i-1, n);
	emit modelReset();
	return true;
}

bool KCommonTreeModel::moveToNext( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = n->parent;
	if(np == NULL)
	{
		int i = m_roots.indexOf(n);
		m_roots.removeOne(n);
		m_roots.insert(i+1, n);
		emit modelReset();
		return true;
	}
	int i = np->childs.indexOf(n);
	np->childs.removeOne(n);
	np->childs.insert(i+1, n);
	emit modelReset();
	return true;
}

bool KCommonTreeModel::moveToFront( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = n->parent;
	if(np == NULL)
	{
		m_roots.removeOne(n);
		m_roots.push_front(n);
		emit modelReset();
		return true;
	}
	np->childs.removeOne(n);
	np->childs.push_front(n);
	emit modelReset();
	return true;
}

bool KCommonTreeModel::moveToBack( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = n->parent;
	if(np == NULL)
	{
		m_roots.removeOne(n);
		m_roots.push_back(n);
		emit modelReset();
		return true;
	}
	np->childs.removeOne(n);
	np->childs.push_back(n);
	emit modelReset();
	return true;
}

bool KCommonTreeModel::moveToParent( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = n->parent;
	if(np)
	{
		Node *npp = np->parent;
		np->childs.removeOne(n);
		if(npp == NULL)
		{
			int i = m_roots.indexOf(np);
			m_roots.insert(i+1, n);
			n->parent = NULL;
			emit modelReset();
			return true;
		}
		int i = npp->childs.indexOf(np);
		npp->childs.insert(i+1, n);
		n->parent = npp;
		emit modelReset();
		return true;
	}
	return false;
}

bool KCommonTreeModel::moveToChild( qint64 nodeid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = n->parent;
	if(np == NULL)
	{
		int i = m_roots.indexOf(n);
		i -= 1;
		if(i >= 0)
		{
			m_roots.removeOne(n);
			Node *np = m_roots.at(i);
			np->childs.push_front(n);
			n->parent = np;
			emit modelReset();
			return true;
		}
		return false;
	}
	int i = np->childs.indexOf(n);
	i -= 1;
	if(i >= 0)
	{
		np->childs.removeOne(n);
		np = np->childs.at(i);
		np->childs.push_front(n);
		n->parent = np;
		emit modelReset();
		return true;
	}
	return false;
}

bool KCommonTreeModel::moveToChild( qint64 nodeid, qint64 npid )
{
	Node *n = m_nodes.value(nodeid);
	if(n == NULL)
		return false;
	Node *np = m_nodes.value(npid);
	if(np)
	{
		np->childs.push_front(n);
		n->parent = np;
		emit modelReset();
		return true;
	}
	return false;
}

void KCommonTreeModel::batchInsert()
{
	m_bBatchInserted = true;
	m_batchInserted.clear();
}

QList<qint64> KCommonTreeModel::batchCommit()
{
	m_bBatchInserted = false;
	if(!m_batchInserted.isEmpty())
	{
		emit itemsInserted(m_batchInserted);
	}
	return m_batchInserted;
}
