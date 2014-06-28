#pragma once;

#include "kwidget_p.h"

class KAbstractTreeModel;
class KTreeItem;
class KScrollBar;
class KListItemCreator;

class KXFW_API KTreeWidgetPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KTreeWidget)
public:
	QBasicTimer m_layoutTimer;
	QBasicTimer m_areaResizeTimer;//当控件大小或Items的条数增加或减少时可用。
	QBasicTimer m_makeListTimer;//从树模型中建立列表。
	qint64 m_nodeIdSelect;//在列表被选中的。
	int m_heightItem;
	QVariant m_variantUnselect;
	QVariant m_variantSelected;
	QVariant m_variantHovered;
	QMargins m_itemMargins;
	KTreeItemCreator *m_creator;
	int m_indexWidgetItemBegin;//表示m_widgetItems可见列表中的第一项。
	QHash<qint64,QPointer<KTreeItem>> m_widgetItems;
	QRectF m_contentArea;//列表显示的区域
	QRectF m_vscrollArea;//滚动条显示的区域

	int m_valueCurrentScroll;//当前列表滚动的位置。
	int m_heightContentTopHidden;
	int m_heightContentBottomHidden;
	QPointer<KScrollBar> m_scrollbar;
	int m_thinknessScrollbar;

	QVariant m_cfgCustom;

	QPointer<KAbstractTreeModel> m_model;

	QSet<qint64> m_itemsExpanded;//展开的状态。
	QList<qint64> m_itemsTree;//将树转化成列表，从而形成的列表s
	int m_styleTree;//树的风格
	int m_depthIndentation;//每一次树的深度
	QColor m_colorBranch; // 树线的颜色

	bool m_bDbClickedAsCollapsedOrExpanded;
public:
	KTreeWidgetPrivate();
	~KTreeWidgetPrivate();
	void init( );
	KAbstractTreeModel* setModel(KAbstractTreeModel *model);
	void itemsInserted(const QList<qint64>& ids);
	void itemsRemoved(const QList<qint64>& ids);
	void itemsChanged(const QList<qint64>& ids, const QList<Qt::ItemDataRole> &roles);
	void modelReset();

	bool isNodeVisible(qint64 nodeid);


	void updateLayout();
	void updateArea();
	void updateList();

	void makeList();
	void resetAreas();
	void doLayout();

	void doItemExpanded(qint64 nodeid);
	void doItemCollapsed(qint64 nodeid);

	void doMousePress(QGraphicsSceneMouseEvent *event);
	void doMouseRelease(QGraphicsSceneMouseEvent *event);
	void doMouseWheel(QGraphicsSceneWheelEvent *event);
	void doMouseDbClick(QGraphicsSceneMouseEvent *event);
	
	qreal itemsHeight();
	qreal maxScrollValue();

	void calcLayoutFirstItem(int &itemIdxFirst, qreal &itemPosVisibleFirst, int iFrom, int iTo);

	/*当该相关项在界面时，才会生效，否则不生效。*/
	bool tryToSetAction(qint64 nodeid, int id, const QVariant& val, QVariant& result);
};

