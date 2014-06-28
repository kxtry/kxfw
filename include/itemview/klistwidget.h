#pragma once;

#include "kwidget.h"

/************************************************************************/
/*                            需要派生使用或创建即初始化其MODEL         */
/************************************************************************/

class KListWidgetPrivate;
class KListWidget;
class KLabel;
class KImageItem;
class KAbstractListModel;

class KXFW_API KListItem : public KWidget
{
	Q_OBJECT
public:
	explicit KListItem(KListWidget *view);
	~KListItem();

	int index() const;
	QString itemIcon() const;
	QString itemText() const;
	QVariant itemData(Qt::ItemDataRole role) const;
	KListWidget *listView() const;
	bool itemSelected() const;
	QVariant customVariant() const;

protected:
	virtual void itemReset();
	/*状态变化*/
	virtual void setSelected(const QVariant& bv);
	virtual void setUnselected(const QVariant& bv);
	virtual void setHovered(const QVariant& bv);
	virtual bool setAction(int id, const QVariant& val, QVariant& result);
private:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void resetIndex(const int idx);
protected:
	friend class KListWidgetPrivate;
	QPointer<KListWidget> m_listWidget;
	QPointer<KAbstractListModel> m_model;
	int m_index;
	QMap<Qt::ItemDataRole, QVariant> m_data;

protected:
	QPointer<KLabel> m_label;
	QPointer<KImageItem> m_icon;
};

class KListItemCreator
{
public:
	virtual KListItem *create(KListWidget *view) = 0;
	virtual void recycle(KListItem *item)
	{
		m_itemsUnused.push_back(item);
	}
protected:
	QList<QPointer<KListItem>> m_itemsUnused;
};


template <class T>
class KListItemCreatorT : public KListItemCreator
{
public:
	inline KListItem *create(KListWidget *view)
	{
		T*pli = NULL;
		if(m_itemsUnused.isEmpty())
		{
			return new T(view);
		}
		return m_itemsUnused.takeFirst();
	}
};

class KXFW_API KListWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KListWidget)
	Q_PROPERTY(QMargins itemMargins READ itemMargins WRITE setItemMargins)
	Q_PROPERTY(int index READ selectIndex WRITE setSelectIndex)
	Q_PROPERTY(QObject* vscroll READ verticalScrollBar)
public:
	explicit KListWidget(QGraphicsItem *parent=NULL);
	~KListWidget();

	int selectIndex();
	void setSelectIndex(int i);
	void clearSelected();

	QMap<Qt::ItemDataRole,QVariant> itemData(int idx) const;

	int findOneIndexByText(const QString& txt);
	QList<int> findAllIndexByText(const QString& txt);
	inline int indexOfText(const QString& txt)
	{
		return findOneIndexByText(txt);
	}

	QMargins itemMargins() const;
	void setItemMargins(const QMargins m);

	int count();
	int itemsHeight();

	void setItemHeight(int iunselect, int iselect);
	void setItemBackgroundVariant(const QVariant& vUnselect, const QVariant& vSelected, const QVariant& vHovered);
	/*当在滚动列表时，会将空白位显示出来，表示列表已经到顶或到底。*/
	void setContentHiddenArea(int itop, int ibottom);
	/*让你记录一些全局信息。*/
	QVariant itemCustomVariant() const;
	void setItemCustomVariant(const QVariant& val);

	QVariant itemUnselectBackgroundVariant() const;
	QVariant itemSelectedBackgroundVariant() const;
	QVariant itemHoveredBackgroundVariant() const;

	void setItemCreator(KListItemCreator *creator);

	KAbstractListModel *model() const;
	KAbstractListModel *setModel(KAbstractListModel *model);

	/*当该相关项在界面时，才会生效，否则不生效。*/
	bool tryToSetAction(int index, int id, const QVariant& val, QVariant& result);

	QObject *verticalScrollBar() const;

signals:
	void indexChanged(int i);
	void itemClicked(int i, Qt::MouseButton button);
	void clicked(Qt::MouseButton button);
	void itemDbClicked(int i, Qt::MouseButton button);
	
protected:
	explicit KListWidget(KListWidgetPrivate& dd, QGraphicsItem *parent);
	bool event(QEvent *e); 

	void construct();

private slots:
	void on_scroll_valueChanged(qreal val);
	void on_model_itemsInserted(int idx, int n);
	void on_model_itemsRemoved(int idx, int n);
	void on_model_itemsMoved(int from, int to, int n);
	void on_model_itemsChanged(int idx, int n, const QList<Qt::ItemDataRole> &roles);
	void on_model_modelReset();
};