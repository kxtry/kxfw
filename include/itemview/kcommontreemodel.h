#pragma once;

#include "kabstracttreemodel.h"

class KXFW_API KCommonTreeModel : public KAbstractTreeModel
{
	Q_OBJECT
public:
	struct Node
	{
		qint64 nodeid;//nodeid==-1值，代表无效值。
		QMap<Qt::ItemDataRole, QVariant> data;
		Node *parent;
		QList<Node*> childs;

		Node()
		{
			parent = NULL;
			nodeid = 0;
		}
		~Node()
		{
			for(int i = 0; i < childs.count(); i++)
			{
				Node *n = childs[i];
				delete n;
			}
		}
	};
public:
	explicit KCommonTreeModel(QObject *parent=NULL);
	~KCommonTreeModel();

	/*数据量大的可使用批增加。*/
	void batchInsert();
	QList<qint64> batchCommit();

	qint64 addChild(qint64 nodeid, const QString& text);
	qint64 addChild(qint64 nodeid, const QString& text, const QVariant& other);
	qint64 addChild(qint64 nodeid, const QString& icon, const QString& text, const QVariant& other);
	qint64 addChild(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals);

	qint64 insertAfter(qint64 nodeid, const QString& text);
	qint64 insertAfter(qint64 nodeid, const QString& text, const QVariant& other);
	qint64 insertAfter(qint64 nodeid, const QString& icon, const QString& text, const QVariant& other);
	qint64 insertAfter(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals);

	qint64 insertBefore(qint64 nodeid, const QString& text);
	qint64 insertBefore(qint64 nodeid, const QString& text, const QVariant& other);
	qint64 insertBefore(qint64 nodeid, const QString& icon, const QString& text, const QVariant& other);
	qint64 insertBefore(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals);


	void removeNode(qint64 nodeid);
	void removeChilds(qint64 nodeid);
	void removeAll();

	qint64 nodeRoot() const;
	int nodeDepth(qint64 nodeid) const;
	int childCount(qint64 nodeid) const;
	int count() const;
	QMap<Qt::ItemDataRole, QVariant> data(qint64 nodeid) const;
	qint64 parent(qint64 nodeid);
	QList<qint64> childs(qint64 nodeid);
	void enumTree(qint64 nodeid, ItemCall* cb);
	qint64 firstChild(qint64 nodeid);
	qint64 nextSibling(qint64 nodeid);
	qint64 previousSibling(qint64 nodeid);

	bool hasNextSibling(qint64 nodeid);
	bool hasPreviousSibling(qint64 nodeid);

	bool moveToLast(qint64 nodeid);
	bool moveToNext(qint64 nodeid);
	bool moveToFront(qint64 nodeid);
	bool moveToBack(qint64 nodeid);
	bool moveToParent(qint64 nodeid);
	bool moveToChild(qint64 nodeid);
	bool moveToChild(qint64 nodeid, qint64 npid);

protected:
	qint64 nodeId();

protected:
	QList<Node*> m_roots;
	QHash<qint64, Node*> m_nodes;
	QList<qint64> m_batchInserted;
	bool m_bBatchInserted;
};