#pragma once;

#include "kwidget.h"


/************************************************************************/
/*                            需要派生使用或创建即初始化其MODEL         */
/************************************************************************/

class KTreeWidgetPrivate;
class KTreeWidget;
class KAbstractTreeModel;
class KLabel;
class KImageItem;

class KXFW_API KTreeItem : public KWidget
{
	Q_OBJECT
public:
	explicit KTreeItem(KTreeWidget *view);
	~KTreeItem();

	qint64 nodeId() const;
	QString itemIcon() const;
	QString itemText() const;
	QVariant itemData(Qt::ItemDataRole role) const;
	KTreeWidget *treeView() const;
	bool itemSelected() const;
	QVariant customVariant() const;
	QColor branchColor() const;
	
	bool hasChildren() const;
	bool hasExpanded() const;
	bool hasNextSibling(qint64 nodeid);
	bool hasPrevSibling(qint64 nodeid);



protected:
	virtual void itemReset();
	/*状态变化*/
	virtual void setSelected(const QVariant& bv);
	virtual void setUnselected(const QVariant& bv);
	virtual void setHovered(const QVariant& bv);
	virtual void drawBranch(QPainter *p);
	virtual bool setAction(int id, const QVariant& val, QVariant& result);
private:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void resetNodeId(const qint64 nodeid);
protected:
	friend class KTreeWidgetPrivate;
	QPointer<KTreeWidget> m_treeWidget;
	QPointer<KAbstractTreeModel> m_model;
	qint64 m_nodeid;
	QMap<Qt::ItemDataRole, QVariant> m_data;

protected:
	QPointer<KLabel> m_label;
	QPointer<KImageItem> m_icon;
};

class KTreeItemCreator
{
public:
	virtual KTreeItem *create(KTreeWidget *view) = 0;
	virtual void recycle(KTreeItem *item)
	{
		m_itemsUnused.push_back(item);
	}
protected:
	QList<QPointer<KTreeItem>> m_itemsUnused;
};

template <class T>
class KTreeItemCreatorT : public KTreeItemCreator
{
public:
	inline KTreeItem *create(KTreeWidget *view)
	{
		T*pli = NULL;
		if(m_itemsUnused.isEmpty())
		{
			return new T(view);
		}
		return m_itemsUnused.takeFirst();
	}
};

class KXFW_API KTreeWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KTreeWidget)
public:
	enum TreeStyle
	{
		None = 0,
		HasBranch = 1,
	};
	Q_ENUMS(TreeStyle)
	Q_DECLARE_FLAGS(TreeStyles, TreeStyle)
	Q_FLAGS(TreeStyles)
	Q_PROPERTY(TreeStyles style READ treeStyle WRITE setTreeStyle)
public:
	explicit KTreeWidget(QGraphicsItem *parent=NULL);
	~KTreeWidget();

	void setItemCreator(KTreeItemCreator *creator);

	KAbstractTreeModel *model() const;
	KAbstractTreeModel *setModel(KAbstractTreeModel *model);

	qint64 selectNode() const;
	void setSelectNode(qint64 id);
	void clearSelected();

	bool hasChild(qint64 nodeid);
	bool hasExpanded(qint64 nodeid);
	bool hasNextSibling(qint64 nodeid);
	bool hasPrevSibling(qint64 nodeid);

	void setExpandedNode(qint64 id);
	void setCollapsedNode(quint64 id);

	QMap<Qt::ItemDataRole,QVariant> nodeData(qint64 nodeid) const;
	int nodeDepth(qint64 nodeid) const;

	TreeStyles treeStyle() const;
	void setTreeStyle(const TreeStyles style);

	QMargins itemMargins() const;
	void setItemMargins(const QMargins m);

	void setItemHeight(int h);

	int indentation();
	void setIndentation(int w);

	void setItemBackgroundVariant(const QVariant& vUnselect, const QVariant& vSelected, const QVariant& vHovered);

	
	QColor branchColor() const;
	void setBranchColor(const QColor& clr);
	/*当在滚动列表时，会将空白位显示出来，表示列表已经到顶或到底。*/
	void setContentHiddenArea(int itop, int ibottom);
	/*让你记录一些全局信息。*/
	QVariant itemCustomVariant() const;
	void setItemCustomVariant(const QVariant& val);

	QVariant itemUnselectBackgroundVariant() const;
	QVariant itemSelectedBackgroundVariant() const;
	QVariant itemHoveredBackgroundVariant() const;

	QObject *verticalScrollBar() const;


	/*当该相关项在界面时，才会生效，否则不生效。*/
	bool tryToSetAction(qint64 nodeid, int id, const QVariant& val, QVariant& result);

	bool doubleClickedAsCollapsedOrExpanded();
	void setDoubleClickedAsCollapsedOrExpanded(bool on);
signals:
	void indexChanged(qint64 nodeid);
	void itemClicked(qint64 nodeid, Qt::MouseButton button);
	void clicked(Qt::MouseButton button);
	void itemExpanded(qint64 nodeid);
	void itemCollapsed(qint64 nodeid);
	void itemDbClicked(qint64 nodeid, Qt::MouseButton button);

protected:
	explicit KTreeWidget(KTreeWidgetPrivate &dd, QGraphicsItem *parent);
	bool event(QEvent *e); 
private slots:
	void on_scroll_valueChanged(qreal val);
	void on_model_itemsInserted(const QList<qint64>& ids);
	void on_model_itemsRemoved(const QList<qint64>& ids);
	void on_model_itemsChanged(const QList<qint64>& ids, const QList<Qt::ItemDataRole> &roles);
	void on_model_modelReset();
};