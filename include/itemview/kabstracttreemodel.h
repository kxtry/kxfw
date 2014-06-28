#pragma once;

class ItemCall
{
public:
	enum NextOP//NextOperation
	{
		eWalkChild,
		eWalkSibling
	};
	QList<qint64> m_itemsResult;
public:
	virtual NextOP walkAt(qint64 nodeid, int nchild)
	{
		m_itemsResult.push_back(nodeid);
		return eWalkChild;
	}
};

class KXFW_API KAbstractTreeModel : public QObject
{
	Q_OBJECT
public:
	enum ItemUserRole
	{
		UserRoleOther = Qt::UserRole + 1,
	};
public:
	KAbstractTreeModel(QObject *parent = 0) ;
	virtual ~KAbstractTreeModel();
	virtual qint64 nodeRoot() const = 0;
	/*当前节点离顶点的深度*/
	virtual int nodeDepth(qint64 nodeid) const = 0;
	/*当前模型中的数据条目*/
	inline bool hasChildren(qint64 nodeid)
	{
		return childCount(nodeid) > 0;
	}
	virtual int childCount(qint64 nodeid) const = 0;
	inline bool isEmpty() const
	{
		return count() == 0;
	}
	virtual int count() const = 0;
	/*获取某条数据信息*/
	virtual QMap<Qt::ItemDataRole, QVariant> data(qint64 nodeid) const = 0;
	virtual qint64 parent(qint64 nodeid) = 0;
	virtual QList<qint64> childs(qint64 nodeid) = 0;

	virtual void enumTree(qint64 nodeid, ItemCall* cb) = 0;
	virtual qint64 firstChild(qint64 nodeid) = 0;
	virtual bool hasNextSibling(qint64 nodeid) = 0;
	virtual bool hasPreviousSibling(qint64 nodeid) = 0;
	virtual qint64 nextSibling(qint64 nodeid) = 0;
	virtual qint64 previousSibling(qint64 nodeid) = 0;
	/*修改某条数据内存*/
	virtual bool setData(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant> &val);
	virtual bool setAction(int id, const QVariant& val, QVariant& result);
signals:
	void itemsInserted(const QList<qint64>& ids);
	void itemsRemoved(const QList<qint64>& ids);
	void itemsChanged(const QList<qint64>& ids, const QList<Qt::ItemDataRole> &roles);
	void modelReset();
};