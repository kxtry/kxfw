#pragma once;

class KXFW_API KAbstractListModel : public QObject
{
	Q_OBJECT
public:
	enum ItemUserRole
	{
		UserRoleOther = Qt::UserRole + 1,
	};
public:
	KAbstractListModel(QObject *parent = 0);
	virtual ~KAbstractListModel();

public:
	//以下函数是列表调用模型。

	inline bool isEmpty()
	{
		return count() == 0;
	}
	/*当前模型中的数据条目*/
	virtual int count() const = 0;
	/*获取某条数据信息*/
	virtual QMap<Qt::ItemDataRole, QVariant> data(int index) const = 0;
	/*修改某条数据内存*/
	virtual bool setData(int index, const QMap<Qt::ItemDataRole, QVariant> &val);
	virtual bool setAction(int id, const QVariant& val, QVariant& result);
signals:
	//以下接口是模型通知列表。
	void itemsInserted(int index, int n);
	void itemsRemoved(int index, int n);
	void itemsMoved(int from, int to, int n);
	void itemsChanged(int index, int n, const QList<Qt::ItemDataRole> &roles);
	void modelReset();
};