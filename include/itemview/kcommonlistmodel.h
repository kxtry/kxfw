#pragma once;

#include "kabstractlistmodel.h"

class KXFW_API KCommonListModel : public KAbstractListModel
{
	Q_OBJECT
public:
	explicit KCommonListModel(QObject *parent=NULL);
	virtual ~KCommonListModel();

	/*数据量大的可使用批增加。*/
	void batchInsert();
	int batchCommit();

	void addItem(const QString& text);
	void addItem(const QString& text, const QVariant& other);
	void addItem(const QString& icon, const QString& text, const QVariant& other);
	void insertItem(int index, const QString& icon, const QString& text, const QVariant& other);
	void removeItem(const QString& text);
	void removeItem(int index);
	void removeAll();

	QString itemsText() const;
	void setItemsText(const QString& items);

	int count() const;
	QMap<Qt::ItemDataRole, QVariant> data(int index) const;

protected:
	QList<QMap<Qt::ItemDataRole, QVariant>> m_items;
	int m_iBatchInsertedBegin;
	int m_nBatchInserted;
	bool m_bBatchInserted;
};