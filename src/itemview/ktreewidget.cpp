#include "stdafx.h"
#include "ktreewidget.h"
#include "kxmlui.h"
#include "kabstracttreemodel.h"
#include "ktreewidget_p.h"
#include "kscrollbar.h"
#include "kimageitem.h"
#include "klabel.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KTreeWidget)

#define ITEM_DEFAULT_HEIGHT (20)
#define DEPTH_INDENTATION	(16)
#define BRANCH_FRAME_SIZE	(9)

class TreeItemCall : public ItemCall
{
public:
	QList<qint64> m_itemsOld;
	QList<qint64> m_itemsNew;
	QSet<qint64> m_itemsExpanded;
	NextOP walkAt(qint64 nodeid, int nchild)
	{
		m_itemsResult.push_back(nodeid);
		if(!m_itemsOld.removeOne(nodeid))
		{
			/*代表这是新增的数据*/
			m_itemsNew.push_back(nodeid);
		}
		if(m_itemsExpanded.contains(nodeid))
			return eWalkChild;
		return eWalkSibling;
	}
};

KTreeWidgetPrivate::KTreeWidgetPrivate()
: m_nodeIdSelect(-1)
, m_indexWidgetItemBegin(0)
, m_valueCurrentScroll(0)
, m_thinknessScrollbar(18)
, m_heightContentBottomHidden(10)
, m_heightContentTopHidden(0)
, m_heightItem(ITEM_DEFAULT_HEIGHT)
, m_variantHovered(QColor(250,250,206))
, m_variantUnselect(QColor(255,255,255))
, m_variantSelected(QColor(250,250,176))
, m_styleTree(KTreeWidget::None)
, m_depthIndentation(DEPTH_INDENTATION)
, m_colorBranch(Qt::black)
, m_bDbClickedAsCollapsedOrExpanded(true)
{

}

KTreeWidgetPrivate::~KTreeWidgetPrivate()
{
	KX_SAFE_DELETE(m_creator);
	KX_SAFE_DELETE(m_model);
}

void KTreeWidgetPrivate::init( )
{
	Q_Q(KTreeWidget);
	q->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	q->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
	m_creator = new KTreeItemCreatorT<KTreeItem>();
}

KAbstractTreeModel* KTreeWidgetPrivate::setModel( KAbstractTreeModel *model )
{
	Q_Q(KTreeWidget);
	KAbstractTreeModel *modelOld = m_model;
	if(m_model)
	{
		QObject::disconnect(m_model, SIGNAL(itemsInserted(QList<qint64>)), q, SLOT(on_model_itemsInserted(QList<qint64>)));
		QObject::disconnect(m_model, SIGNAL(itemsRemoved(QList<qint64>)), q, SLOT(on_model_itemsRemoved(QList<qint64>)));
		QObject::disconnect(m_model, SIGNAL(itemsChanged(QList<qint64>,QList<Qt::ItemDataRole>)), q, SLOT(on_model_itemsChanged(QList<qint64>,QList<Qt::ItemDataRole>)));
		QObject::disconnect(m_model, SIGNAL(modelReset()), q, SLOT(on_model_modelReset()));
	}
	m_model = model;
	QObject::connect(m_model, SIGNAL(itemsInserted(QList<qint64>)), q, SLOT(on_model_itemsInserted(QList<qint64>)));
	QObject::connect(m_model, SIGNAL(itemsRemoved(QList<qint64>)), q, SLOT(on_model_itemsRemoved(QList<qint64>)));
	QObject::connect(m_model, SIGNAL(itemsChanged(QList<qint64>,QList<Qt::ItemDataRole>)), q, SLOT(on_model_itemsChanged(QList<qint64>,QList<Qt::ItemDataRole>)));
	QObject::connect(m_model, SIGNAL(modelReset()), q, SLOT(on_model_modelReset()));
	return modelOld;
}

bool KTreeWidgetPrivate::isNodeVisible( qint64 nodeid )
{
	Q_Q(KTreeWidget);
	qint64 idref = m_model->parent(nodeid);
	if(idref == m_model->nodeRoot())
	{
		/*自身是顶点*/
		return true;
	}
	return m_itemsExpanded.contains(idref);
}

qreal KTreeWidgetPrivate::itemsHeight()
{
	int height = m_itemsTree.count() * m_heightItem;
	return height;
}

qreal KTreeWidgetPrivate::maxScrollValue()
{
	Q_Q(KTreeWidget);
	return itemsHeight() - m_contentArea.height() + m_heightContentBottomHidden;
}

void KTreeWidgetPrivate::makeList( )
{
	qint64 root = m_model->nodeRoot();
	TreeItemCall tic;
	tic.m_itemsExpanded = m_itemsExpanded;
	tic.m_itemsOld = m_itemsTree;//旧数据。
	m_model->enumTree(root, &tic);
	m_itemsTree = tic.m_itemsResult;
	if(!tic.m_itemsNew.isEmpty() || !tic.m_itemsOld.isEmpty())
	{
		/*代表已经发生变化要刷新区域*/
		resetAreas();
	}
}

void KTreeWidgetPrivate::doItemExpanded( qint64 nodeid )
{
	Q_Q(KTreeWidget);
	m_itemsExpanded.insert(nodeid);
	/*重新计算列表条目*/
	updateList();
	emit q->itemExpanded(nodeid);
}

void KTreeWidgetPrivate::doItemCollapsed( qint64 nodeid )
{
	Q_Q(KTreeWidget);
	m_itemsExpanded.remove(nodeid);
	/*重新计算列表条目*/
	updateList();
	emit q->itemCollapsed(nodeid);
}

void KTreeWidgetPrivate::updateList()
{
	Q_Q(KTreeWidget);
	m_makeListTimer.start(0, q);
}

void KTreeWidgetPrivate::updateLayout()
{
	Q_Q(KTreeWidget);
	m_layoutTimer.start(0, q);
}

void KTreeWidgetPrivate::updateArea()
{
	Q_Q(KTreeWidget);
	m_areaResizeTimer.start(0, q);
}

void KTreeWidgetPrivate::resetAreas()
{
	Q_Q(KTreeWidget);
	QSizeF winSize = q->size();
	if(winSize.height() < itemsHeight())
	{
		//需要滚动条
		if(m_scrollbar == NULL)
		{
			m_scrollbar = new KScrollBar(q);
			QObject::connect(m_scrollbar, SIGNAL(valueChanged(qreal)), q, SLOT(on_scroll_valueChanged(qreal)));
		}
		qreal thickness = m_scrollbar->frameThickness();
		m_vscrollArea = QRectF(winSize.width() - thickness, 0, thickness, winSize.height());
		m_contentArea = QRectF(0, 0, winSize.width() - m_thinknessScrollbar, winSize.height());
		m_scrollbar->setGeometry(m_vscrollArea);
		m_scrollbar->setFixSize(m_vscrollArea.size());
		qreal maxval = maxScrollValue();
		m_scrollbar->setRange(0, maxval);
		m_valueCurrentScroll = qBound<int>(0, m_valueCurrentScroll, maxval);
		m_scrollbar->setSingleStep(m_heightItem);
		m_scrollbar->setPageStep(m_contentArea.height() - m_heightItem);
		m_scrollbar->show();
	}
	else
	{
		//不需要滚动条
		m_valueCurrentScroll = 0;
		m_contentArea = QRectF(0, 0, winSize.width(), winSize.height());
		if(m_scrollbar)
		{
			m_scrollbar->hide();
		}
	}
	/*重新调整所有项的尺寸*/
	doLayout();
}

#define BLOCK_NUMBER	(10)

void KTreeWidgetPrivate::calcLayoutFirstItem( int &itemIdxFirst, qreal &itemPosVisibleFirst, int iFrom, int iTo )
{
	int nCount = iTo - iFrom;
	if(nCount < 3 * BLOCK_NUMBER)
	{
		for(int i = iFrom; i < iTo; i++)
		{
			if(itemPosVisibleFirst + m_heightItem > m_contentArea.top())
			{
				itemIdxFirst = i;
				break;
			}
			else
			{
				itemPosVisibleFirst += m_heightItem;
			}
		}
		return;
	}
	int nBlock = (nCount + BLOCK_NUMBER - 1) / BLOCK_NUMBER;
	for(int iblock = 0; iblock < BLOCK_NUMBER; iblock++)
	{
		int heightBlock = m_heightItem * nBlock;
		int iItemBegin = iblock * nBlock + iFrom;
		int iItemEnd = iItemBegin + nBlock;
		if(itemPosVisibleFirst + heightBlock > m_contentArea.top())
		{
			if(iItemEnd > m_itemsTree.size())
			{
				iItemEnd = m_itemsTree.size();
			}
			calcLayoutFirstItem(itemIdxFirst, itemPosVisibleFirst, iItemBegin, iItemEnd);
			break;
		}
		else
		{
			itemPosVisibleFirst += heightBlock;
		}
	}
}

void KTreeWidgetPrivate::doLayout()
{
	Q_Q(KTreeWidget);

	if(m_itemsTree.isEmpty())
	{
		/*直接回收所有资源，并返回。*/
		QHash<qint64, QPointer<KTreeItem>> items = m_widgetItems;
		m_widgetItems.clear();
		m_indexWidgetItemBegin = 0;
		int iLeft = 0;
		for(QHash<qint64, QPointer<KTreeItem>>::iterator iter = items.begin(); iter != items.end(); iter++,iLeft++)
		{
			KTreeItem *pli = iter.value();
			if(iLeft <= 3)
			{
				m_creator->recycle(pli);
				pli->hide();
			}
			else
			{
				delete pli;
			}
		}
		return;
	}
	/*至少有一个元素。*/
	//显示在列表中的第一个Item
	//计算下边框是否隐藏为列表可见区的第一项。
	qreal itemPosVisibleFirst = -m_valueCurrentScroll;
	int itemIdxFirst = 0;
	calcLayoutFirstItem(itemIdxFirst, itemPosVisibleFirst, 0, m_itemsTree.size());
	//最后一个元素显示在列表中的。
	//计算上边框是否隐藏来判断是否为列表可见区中最后一项
	qreal itemPosVisibleLast = itemPosVisibleFirst;
	int itemIdxLast = m_itemsTree.count() - 1;
	for(int i = itemIdxFirst; i < m_itemsTree.count(); i++)
	{
		if(itemPosVisibleLast + m_heightItem > m_contentArea.bottom())
		{
			itemIdxLast = i;
			break;
		}
		else
		{
			itemPosVisibleLast += m_heightItem;
		}
	}
	QHash<qint64, QPointer<KTreeItem>> items = m_widgetItems;
	m_widgetItems.clear();
	m_indexWidgetItemBegin = itemIdxFirst;

	/*处理已经重复的对象*/
	bool bdepth = m_styleTree & KTreeWidget::HasBranch;
	qreal itemPosVisible = itemPosVisibleFirst;
	QSet<int> itemsok;
	for(int i = itemIdxFirst; !items.isEmpty() && i <= itemIdxLast; i++)
	{
		qint64 nodeid = m_itemsTree.at(i);
		KTreeItem *pli = items.take(nodeid);
		if(pli == NULL)
		{
			itemPosVisible += m_heightItem;
			continue;
		}
		QRectF itemRt;
		itemRt = QRectF(m_contentArea.left(), itemPosVisible, m_contentArea.width(), m_heightItem);
		itemPosVisible += m_heightItem;
		QMargins margins = m_itemMargins;
		if(bdepth)
		{
			int ndepth = m_model->nodeDepth(nodeid);
			ndepth *= m_depthIndentation;
			margins = QMargins(m_itemMargins.left() + ndepth, m_itemMargins.top(), m_itemMargins.right(), m_itemMargins.bottom());
		}
		pli->setMargins(margins);
		pli->setGeometry(itemRt);
		pli->setFixSize(itemRt.size());
		if(nodeid == m_nodeIdSelect)
		{
			pli->setSelected(m_variantSelected);
		}
		else if(pli->isUnderMouse())
		{
			pli->setHovered(m_variantHovered);
		}
		else
		{
			pli->setUnselected(m_variantUnselect);
		}
		pli->show();
		m_widgetItems.insert(nodeid, pli);
		itemsok.insert(i);
	}

	/*处理没有设置的对象。*/
	itemPosVisible = itemPosVisibleFirst;
	for(int i = itemIdxFirst; i <= itemIdxLast; i++)
	{
		if(itemsok.contains(i))
		{
			itemPosVisible += m_heightItem;
			continue;
		}
		qint64 nodeid = m_itemsTree.at(i);
		KTreeItem *pli = NULL;
		if(items.isEmpty())
		{
			pli = m_creator->create(q);
		}
		else
		{
			QHash<qint64, QPointer<KTreeItem>>::iterator iter = items.begin();
			pli = iter.value();
			items.erase(iter);
		}
		QRectF itemRt;
		itemRt = QRectF(m_contentArea.left(), itemPosVisible, m_contentArea.width(), m_heightItem);
		itemPosVisible += m_heightItem;
		pli->resetNodeId(nodeid);
		QMargins margins = m_itemMargins;
		if(bdepth)
		{
			int ndepth = m_model->nodeDepth(nodeid);
			ndepth *= m_depthIndentation;
			margins = QMargins(m_itemMargins.left() + ndepth, m_itemMargins.top(), m_itemMargins.right(), m_itemMargins.bottom());
		}
		pli->setMargins(margins);
		pli->setGeometry(itemRt);
		pli->setFixSize(itemRt.size());
		if(nodeid == m_nodeIdSelect)
		{
			pli->setSelected(m_variantSelected);
		}
		else if(pli->isUnderMouse())
		{
			pli->setHovered(m_variantHovered);
		}
		else
		{
			pli->setUnselected(m_variantUnselect);
		}
		pli->show();
		m_widgetItems.insert(nodeid, pli);
	}

	int iLeft = 0;
	for(QHash<qint64, QPointer<KTreeItem>>::iterator iter = items.begin(); iter != items.end(); iter++,iLeft++)
	{
		KTreeItem *pli = iter.value();
		if(iLeft <= 3)
		{
			m_creator->recycle(pli);
			pli->hide();
		}
		else
		{
			delete pli;
		}
	}
}

void KTreeWidgetPrivate::doMousePress( QGraphicsSceneMouseEvent *event )
{
	Q_Q(KTreeWidget);
	QPointF pt = event->pos();
	bool bdepth = m_styleTree & KTreeWidget::HasBranch;
	for(QHash<qint64,QPointer<KTreeItem>>::iterator iter = m_widgetItems.begin(); iter != m_widgetItems.end(); iter++)
	{
		KTreeItem *pli = iter.value();
		QRectF geom = pli->geometry();
		if(geom.contains(pt))
		{
			qint64 nid = pli->nodeId();
			if(bdepth)
			{
				QMargins m = pli->margins();
				QRectF plusRt = QRectF(geom.left() + m.left() - m_depthIndentation, geom.top(), m_depthIndentation, geom.height());
				if(plusRt.contains(pt))
				{
					if(m_itemsExpanded.contains(nid))
					{
						doItemCollapsed(nid);
					}
					else
					{
						doItemExpanded(nid);
					}
					return;
				}
			}
			if(nid == m_nodeIdSelect)
			{
				emit q->itemClicked(nid, event->button());
				return;
			}
			KTreeItem *pliOld = m_widgetItems.value(m_nodeIdSelect);
			if(pliOld)
			{
				pliOld->setUnselected(m_variantUnselect);
			}
			m_nodeIdSelect = pli->nodeId();
			pli->setSelected(m_variantSelected);
			emit q->indexChanged(m_nodeIdSelect);
			emit q->itemClicked(nid, event->button());
			return;
		}
	}
	emit q->clicked(event->button());
}

void KTreeWidgetPrivate::doMouseRelease( QGraphicsSceneMouseEvent *event )
{
	QPointF pt = event->pos();
}

void KTreeWidgetPrivate::doMouseWheel( QGraphicsSceneWheelEvent *event )
{
	Q_Q(KTreeWidget);
	if(m_scrollbar)
	{
		m_scrollbar->handleMouseWheelEvent(event);
	}
}

void KTreeWidgetPrivate::doMouseDbClick( QGraphicsSceneMouseEvent *event )
{
	Q_Q(KTreeWidget);
	QPointF pt = event->pos();
	for(QHash<qint64,QPointer<KTreeItem>>::iterator iter = m_widgetItems.begin(); iter != m_widgetItems.end(); iter++)
	{
		KTreeItem *pli = iter.value();
		QRectF geom = pli->geometry();
		if(geom.contains(pt))
		{
			qint64 nid = pli->nodeId();
			if(m_bDbClickedAsCollapsedOrExpanded)
			{
				if(m_itemsExpanded.contains(nid))
				{
					doItemCollapsed(nid);
				}
				else
				{
					doItemExpanded(nid);
				}
			}
			else
			{
				emit q->itemDbClicked(nid, event->button());
			}
			return;
		}
	}
}

void KTreeWidgetPrivate::itemsInserted( const QList<qint64>& ids )
{
	Q_Q(KTreeWidget);
	bool bupdate = false;
	for(int i = 0; i < ids.count(); i++)
	{
		if(isNodeVisible(ids.at(i)))
		{
			bupdate = true;
			break;
		}
	}

	if(bupdate)
	{
		updateList();
	}
	else if(m_styleTree & KTreeWidget::HasBranch)
	{
		for(int i = 0; i < ids.size(); i++)
		{
			qint64 nodeid = m_model->parent(ids.at(i));
			int count = m_model->childCount(nodeid);
			if(count == 1)
			{
				KTreeItem *pli = m_widgetItems.value(nodeid);
				if(pli)
				{
					pli->update();
				}
			}
		}
	}
}

void KTreeWidgetPrivate::itemsRemoved( const QList<qint64>& ids )
{
	for(int i = 0; i < ids.count(); i++)
	{
		m_itemsExpanded.remove(ids.at(i));
	}
	updateList();
}

void KTreeWidgetPrivate::itemsChanged( const QList<qint64>& ids, const QList<Qt::ItemDataRole> &roles )
{

}

void KTreeWidgetPrivate::modelReset()
{
	m_itemsTree.clear();
	updateList();
}

bool KTreeWidgetPrivate::tryToSetAction( qint64 nodeid, int id, const QVariant& val, QVariant& result )
{
	KTreeItem *pli = m_widgetItems.value(nodeid);
	if(pli)
	{
		return pli->setAction(id, val, result);
	}
	return false;
}

KTreeWidget::KTreeWidget( QGraphicsItem *parent/*=NULL*/ )
 : KWidget(*new KTreeWidgetPrivate(), parent)
{
	Q_D(KTreeWidget);
	d->init();
}

KTreeWidget::KTreeWidget( KTreeWidgetPrivate &dd, QGraphicsItem *parent )
 : KWidget(dd, parent)
{
	Q_D(KTreeWidget);
	d->init();
}

KTreeWidget::~KTreeWidget()
{

}

void KTreeWidget::setItemCreator( KTreeItemCreator *creator )
{
	Q_D(KTreeWidget);
	KX_SAFE_DELETE(d->m_creator);
	d->m_creator = creator;
}

KAbstractTreeModel * KTreeWidget::model() const
{
	Q_D(const KTreeWidget);
	return d->m_model;
}

KAbstractTreeModel * KTreeWidget::setModel( KAbstractTreeModel *model )
{
	Q_D(KTreeWidget);
	return d->setModel(model);
}

void KTreeWidget::on_model_itemsInserted( const QList<qint64>& ids )
{
	Q_D(KTreeWidget);
	d->itemsInserted(ids);
}

void KTreeWidget::on_model_itemsRemoved( const QList<qint64>& ids )
{
	Q_D(KTreeWidget);
	d->itemsRemoved(ids);
}

void KTreeWidget::on_model_itemsChanged( const QList<qint64>& ids, const QList<Qt::ItemDataRole> &roles )
{
	Q_D(KTreeWidget);
	d->itemsChanged(ids, roles);
}

void KTreeWidget::on_model_modelReset()
{
	Q_D(KTreeWidget);
	d->modelReset();
}

bool KTreeWidget::event(QEvent *e)
{
	Q_D(KTreeWidget);

	if(e->type() == QEvent::Timer)
	{
		QTimerEvent *te = (QTimerEvent*)e;
		quint32 tid = te->timerId();
		if(tid == d->m_layoutTimer.timerId())
		{
			d->m_layoutTimer.stop();
			d->doLayout();
		}
		else if(tid == d->m_areaResizeTimer.timerId())
		{
			d->m_areaResizeTimer.stop();
			d->resetAreas();
		}
		else if(tid == d->m_makeListTimer.timerId())
		{
			d->m_makeListTimer.stop();
			d->makeList();
		}
	}
	else if(e->type() == QEvent::GraphicsSceneResize)
	{
		/*触发Areas的调整*/
		d->m_areaResizeTimer.start(0, this);
	}
	else if(e->type() == QEvent::GraphicsSceneMousePress)
	{
		d->doMousePress((QGraphicsSceneMouseEvent*)e);
	}
	else if(e->type() == QEvent::GraphicsSceneMouseRelease)
	{
		d->doMouseRelease((QGraphicsSceneMouseEvent*)e);
	}
	else if(e->type() == QEvent::GraphicsSceneMouseDoubleClick)
	{
		d->doMouseDbClick((QGraphicsSceneMouseEvent*)e);
	}
	else if(e->type() == QEvent::GraphicsSceneWheel)
	{
		d->doMouseWheel((QGraphicsSceneWheelEvent*)e);
	}
	return __super::event(e);
}

QMargins KTreeWidget::itemMargins() const
{
	Q_D(const KTreeWidget);
	return d->m_itemMargins;
}

void KTreeWidget::setItemMargins( const QMargins m )
{
	Q_D(KTreeWidget);
	d->m_itemMargins = m;
}

void KTreeWidget::setItemBackgroundVariant( const QVariant& vUnselect, const QVariant& vSelected, const QVariant& vHovered )
{
	Q_D(KTreeWidget);
	d->m_variantSelected = vSelected;
	d->m_variantUnselect = vUnselect;
	d->m_variantHovered = vHovered;
}

QVariant KTreeWidget::itemUnselectBackgroundVariant() const
{
	Q_D(const KTreeWidget);
	return d->m_variantUnselect;
}

QVariant KTreeWidget::itemSelectedBackgroundVariant() const
{
	Q_D(const KTreeWidget);
	return d->m_variantSelected;
}

QVariant KTreeWidget::itemHoveredBackgroundVariant() const
{
	Q_D(const KTreeWidget);
	return d->m_variantHovered;
}

void KTreeWidget::setContentHiddenArea( int itop, int ibottom )
{
	Q_D(KTreeWidget);
	d->m_heightContentTopHidden = qMax(itop, 0);
	d->m_heightContentBottomHidden = qMax(ibottom, 0);
}

QObject * KTreeWidget::verticalScrollBar() const
{
	Q_D(const KTreeWidget);
	return d->m_scrollbar;
}

void KTreeWidget::on_scroll_valueChanged( qreal val )
{
	Q_D(KTreeWidget);
	d->m_valueCurrentScroll = val;
	d->updateLayout();
}

QVariant KTreeWidget::itemCustomVariant() const
{
	Q_D(const KTreeWidget);
	return d->m_cfgCustom;
}

void KTreeWidget::setItemCustomVariant( const QVariant& val )
{
	Q_D(KTreeWidget);
	d->m_cfgCustom = val;
}

QMap<Qt::ItemDataRole,QVariant> KTreeWidget::nodeData( qint64 nodeid ) const
{
	Q_D(const KTreeWidget);
	return d->m_model->data(nodeid);
}

qint64 KTreeWidget::selectNode() const
{
	Q_D(const KTreeWidget);
	return d->m_nodeIdSelect;
}

void KTreeWidget::setSelectNode( qint64 id )
{
	Q_D(KTreeWidget);
	if(!d->m_itemsTree.contains(id))
		return;

	if(id == d->m_nodeIdSelect)
		return;
	d->m_nodeIdSelect = id;
	d->updateLayout();
	emit indexChanged(id);
}

void KTreeWidget::clearSelected()
{
	Q_D(KTreeWidget);
	d->m_nodeIdSelect = -1;
}

void KTreeWidget::setItemHeight( int h )
{
	Q_D(KTreeWidget);
	d->m_heightItem = h;
}

void KTreeWidget::setExpandedNode( qint64 id )
{
	Q_D(KTreeWidget);
	d->doItemExpanded(id);
}

void KTreeWidget::setCollapsedNode( quint64 id )
{
	Q_D(KTreeWidget);
	d->doItemCollapsed(id);
}

int KTreeWidget::nodeDepth( qint64 nodeid ) const
{
	Q_D(const KTreeWidget);
	return d->m_model->nodeDepth(nodeid);
}

KTreeWidget::TreeStyles KTreeWidget::treeStyle() const
{
	Q_D(const KTreeWidget);
	return (TreeStyles)d->m_styleTree;
}

void KTreeWidget::setTreeStyle( const TreeStyles style )
{
	Q_D(KTreeWidget);
	d->m_styleTree = style;
}

int KTreeWidget::indentation()
{
	Q_D(KTreeWidget);
	return d->m_depthIndentation;
}

void KTreeWidget::setIndentation( int w )
{
	Q_D(KTreeWidget);
	d->m_depthIndentation = w;
}

QColor KTreeWidget::branchColor() const
{
	Q_D(const KTreeWidget);
	return d->m_colorBranch;
}

void KTreeWidget::setBranchColor( const QColor& clr )
{
	Q_D(KTreeWidget);
	d->m_colorBranch = clr;
}

bool KTreeWidget::hasChild( qint64 nodeid )
{
	Q_D(KTreeWidget);
	return d->m_model->childCount(nodeid) > 0;
}

bool KTreeWidget::hasExpanded( qint64 nodeid )
{
	Q_D(KTreeWidget);
	return d->m_itemsExpanded.contains(nodeid);
}

bool KTreeWidget::hasNextSibling( qint64 nodeid )
{
	Q_D(KTreeWidget);
	return d->m_model->hasNextSibling(nodeid);
}

bool KTreeWidget::hasPrevSibling( qint64 nodeid )
{
	Q_D(KTreeWidget);
	return d->m_model->hasPreviousSibling(nodeid);
}

bool KTreeWidget::tryToSetAction( qint64 nodeid, int id, const QVariant& val, QVariant& result )
{
	Q_D(KTreeWidget);
	return d->tryToSetAction(nodeid, id, val, result);
}

bool KTreeWidget::doubleClickedAsCollapsedOrExpanded()
{
	Q_D(KTreeWidget);
	return d->m_bDbClickedAsCollapsedOrExpanded;
}

void KTreeWidget::setDoubleClickedAsCollapsedOrExpanded( bool on )
{
	Q_D(KTreeWidget);
	d->m_bDbClickedAsCollapsedOrExpanded = on;
}

KTreeItem::KTreeItem( KTreeWidget *view )
:KWidget(view)
,m_treeWidget(view)
,m_model(m_treeWidget->model())
{
	setAcceptHoverEvents(true);
}

KTreeItem::~KTreeItem()
{

}

qint64 KTreeItem::nodeId() const
{
	return m_nodeid;
}

void KTreeItem::itemReset( )
{
	if(layoutType() == KWidget::None)
	{
		setLayoutType(KWidget::HBox);
		setSpacing(3);
	}

	QString ico = itemIcon();
	QString txt = itemText();

	if(m_label == NULL)
	{
		m_label = new KLabel(this);
		addItem(m_label);
	}

	if(!ico.isEmpty() && m_icon == NULL)
	{
		m_icon = new KImageItem(this);
		insertItem(m_icon, 0);
	}

	if(m_icon)
	{
		m_icon->setImagePath(ico);
		QSize s = m_icon->pixmapSize();
		m_icon->setFixWidth(s.width());
	}

	if(m_label)
	{
		m_label->setText(txt);
	}
}

QString KTreeItem::itemIcon() const
{
	return m_data.value(Qt::DecorationRole).toString();
}

QString KTreeItem::itemText() const
{
	return m_data.value(Qt::DisplayRole).toString();
}

QVariant KTreeItem::itemData(Qt::ItemDataRole role) const
{
	return m_data.value(role).toString();
}

KTreeWidget * KTreeItem::treeView() const
{
	return m_treeWidget;
}

bool KTreeItem::itemSelected() const
{
	return m_nodeid == m_treeWidget->selectNode();
}

void KTreeItem::setSelected( const QVariant& bv )
{
	if(bv.type() == QVariant::Color)
	{
		QColor clr = bv.value<QColor>();
		setBackgroundColor(clr);
	}
}

void KTreeItem::setUnselected( const QVariant& bv )
{
	if(bv.type() == QVariant::Color)
	{
		QColor clr = bv.value<QColor>();
		setBackgroundColor(clr);
	}
}

void KTreeItem::setHovered( const QVariant& bv )
{
	if(bv.type() == QVariant::Color)
	{
		QColor clr = bv.value<QColor>();
		setBackgroundColor(clr);
	}
}

void KTreeItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	if(!itemSelected())
	{
		QVariant v = m_treeWidget->itemHoveredBackgroundVariant();
		setHovered(v);
	}
}

void KTreeItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	if(!itemSelected())
	{
		QVariant v = m_treeWidget->itemUnselectBackgroundVariant();
		setUnselected(v);
	}
}

QVariant KTreeItem::customVariant() const
{
	return m_treeWidget->itemCustomVariant();
}

void KTreeItem::resetNodeId( const qint64 nodeid )
{
	m_nodeid = nodeid;
	m_data = m_treeWidget->nodeData(nodeid);
	itemReset();
	update();
}

void KTreeItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	drawBranch(painter);
}

void KTreeItem::drawBranch( QPainter *p )
{
	KTreeWidget::TreeStyles styles = m_treeWidget->treeStyle();
	if(!(styles & KTreeWidget::HasBranch))
	{
		return;
	}
	int ndepth = m_treeWidget->nodeDepth(m_nodeid);
	QColor clr = branchColor();
	int wdepth = m_treeWidget->indentation();
	QMargins m = margins();
	QRectF winRt = rect();
	QBrush brush(clr, Qt::Dense4Pattern);
	int xbegin = m.left() - wdepth;
	int ymid = winRt.center().y();
	if(hasChildren())
	{
		p->fillRect(xbegin + 4, winRt.top(), 1, ymid - 4 - winRt.top(), brush);
		QPen penold = p->pen();
		p->setPen(clr);
		if(hasExpanded())
		{
			p->drawLine(xbegin+2, ymid, xbegin + 6, ymid);
		}
		else
		{
			p->drawLine(xbegin+2, ymid, xbegin + 6, ymid);
			p->drawLine(xbegin + 4, ymid - 2, xbegin + 4, ymid + 2);
		}
		p->setPen(penold);
		QRect rt(xbegin, ymid-4, 8, 8);
		p->drawRect(rt);
		p->fillRect(xbegin + 8, ymid, wdepth - 8, 1, brush);
		if(hasNextSibling(m_nodeid))
		{
			p->fillRect(xbegin + 4, ymid + 4, 1, winRt.bottom() - ymid - 4, brush);
		}
	}
	else
	{
		p->fillRect(xbegin + 4, winRt.top(), 1, ymid - winRt.top(), brush);
		p->fillRect(xbegin + 4, ymid, wdepth - 8, 1, brush);
		if(hasNextSibling(m_nodeid))
		{
			p->fillRect(xbegin + 4, ymid, 1, winRt.bottom() - ymid, brush);
		}
	}
	xbegin -= wdepth;
	KAbstractTreeModel *model = m_treeWidget->model();
	qint64 nodeid = m_nodeid;
	for(int i = 0; i < ndepth - 1; i++)
	{
		int x = xbegin + 4;
		int y = winRt.top();
		nodeid = model->parent(nodeid);
		if(model->hasNextSibling(nodeid))
		{
			p->fillRect(x, y, 1, winRt.height(), brush);
		}
		xbegin -= wdepth;
	}
}

QColor KTreeItem::branchColor() const
{
	return m_treeWidget->branchColor();
}

bool KTreeItem::hasChildren() const
{
	return m_treeWidget->hasChild(m_nodeid);
}

bool KTreeItem::hasExpanded() const
{
	return m_treeWidget->hasExpanded(m_nodeid);
}

bool KTreeItem::hasNextSibling( qint64 nodeid )
{
	return m_treeWidget->hasNextSibling(m_nodeid);
}

bool KTreeItem::hasPrevSibling( qint64 nodeid )
{
	return m_treeWidget->hasPrevSibling(m_nodeid);
}

bool KTreeItem::setAction( int id, const QVariant& val, QVariant& result )
{
	return false;
}
