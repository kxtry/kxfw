#include "stdafx.h"
#include "kwidget_p.h"
#include "klistwidget.h"
#include "klabel.h"
#include "kimageitem.h"
#include "kscrollbar.h"
#include "kabstractlistmodel.h"
#include "kxmlui.h"

#include "klistwidget_p.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KListWidget)

#define ITEM_DEFAULT_HEIGHT		(20)

#define SCROLLBAR_THICKNESS		(18)

KListWidgetPrivate::KListWidgetPrivate()
	: m_indexSelect(-1)
	, m_indexWidgetItemBegin(0)
	, m_valueCurrentScroll(0)
	, m_thinknessScrollbar(18)
	, m_heightContentBottomHidden(10)
	, m_heightContentTopHidden(0)
	, m_heightSelect(ITEM_DEFAULT_HEIGHT)
	, m_heightUnselect(ITEM_DEFAULT_HEIGHT)
	, m_variantHovered(QColor(250,250,206))
	, m_variantUnselect(QColor(255,255,255))
	, m_variantSelected(QColor(250,250,176))
{
}

KListWidgetPrivate::~KListWidgetPrivate()
{
	KX_SAFE_DELETE(m_creator);
	KX_SAFE_DELETE(m_model);
}

void KListWidgetPrivate::init()
{
	Q_Q(KListWidget);
	q->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	q->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
	m_creator = new KListItemCreatorT<KListItem>();
}

void KListWidgetPrivate::itemsInserted(int index, int n)
{
	updateArea();
}

void KListWidgetPrivate::itemsRemoved(int index, int n)
{
	updateArea();
}

void KListWidgetPrivate::itemsMoved(int from, int to, int n)
{

}

void KListWidgetPrivate::itemsChanged(int index, int n, const QList<Qt::ItemDataRole> &roles)
{

}

void KListWidgetPrivate::modelReset()
{
	m_indexSelect = -1;
	m_indexWidgetItemBegin = 0;
	updateArea();
}

void KListWidgetPrivate::updateLayout()
{
	Q_Q(KListWidget);
	m_layoutTimer.start(0, q);
}

void KListWidgetPrivate::updateArea()
{
	Q_Q(KListWidget);
	m_areaResizeTimer.start(0, q);
}

qreal KListWidgetPrivate::itemsHeight()
{
	int height = m_model->count() * m_heightUnselect;
	if(m_indexSelect >= 0 && m_indexSelect < m_model->count())
	{
		height += m_heightSelect - m_heightUnselect;
	}
	return height;
}

qreal KListWidgetPrivate::maxScrollValue()
{
	Q_Q(KListWidget);
	return itemsHeight() - m_contentArea.height() + m_heightContentBottomHidden;
}

void KListWidgetPrivate::resetAreas()
{
	Q_Q(KListWidget);
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
		m_scrollbar->setSingleStep(m_heightUnselect);
		m_scrollbar->setPageStep(m_contentArea.height() - m_heightUnselect);
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

void KListWidgetPrivate::calcLayoutFirstItem( int &itemIdxFirst, qreal &itemPosVisibleFirst, int iFrom, int iTo )
{
	int nCount = iTo - iFrom;
	if(nCount < 3 * BLOCK_NUMBER)
	{
		for(int i = iFrom; i < iTo; i++)
		{
			if(m_indexSelect == i)
			{
				if(itemPosVisibleFirst + m_heightSelect > m_contentArea.top())
				{
					itemIdxFirst = i;
					break;
				}
				else
				{
					itemPosVisibleFirst += m_heightSelect;
				}
			}
			else
			{
				if(itemPosVisibleFirst + m_heightUnselect > m_contentArea.top())
				{
					itemIdxFirst = i;
					break;
				}
				else
				{
					itemPosVisibleFirst += m_heightUnselect;
				}
			}
		}
		return;
	}
	int nBlock = (nCount + BLOCK_NUMBER - 1) / BLOCK_NUMBER;
	for(int iblock = 0; iblock < BLOCK_NUMBER; iblock++)
	{
		int heightBlock = m_heightSelect * nBlock;
		int iItemBegin = iblock * nBlock + iFrom;
		int iItemEnd = iItemBegin + nBlock;
		if(iItemBegin < m_indexSelect && m_indexSelect < iItemEnd)
		{
			heightBlock += m_heightSelect - m_heightUnselect;
		}
		if(itemPosVisibleFirst + heightBlock > m_contentArea.top())
		{
			if(iItemEnd > m_model->count())
			{
				iItemEnd = m_model->count();
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

void KListWidgetPrivate::doLayout()
{
	Q_Q(KListWidget);

	if(m_model->isEmpty())
	{
		/*直接回收所有资源，并返回。*/
		QHash<int, QPointer<KListItem>> items = m_widgetItems;
		m_widgetItems.clear();
		m_indexWidgetItemBegin = 0;
		int iLeft = 0;
		for(QHash<int,QPointer<KListItem>>::iterator iter = items.begin(); iter != items.end(); iter++, iLeft++)
		{
			KListItem *pli = iter.value();
			if(iLeft < 3)
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
#if 1
	calcLayoutFirstItem(itemIdxFirst, itemPosVisibleFirst, 0, m_model->count());
#else
	int nblock = (m_model->count() + 49) / 50;
	for(int iblock = 0; iblock < nblock; iblock++)
	{
		int height50 = m_heightUnselect * 50;
		if(iblock * 50 < m_indexSelect && m_indexSelect < (iblock+1) * 50)
		{
			height50 += m_heightSelect - m_heightUnselect;
		}
		if(itemPosVisibleFirst + height50 > m_contentArea.top())
		{
			for(int i = iblock * 50; i < m_model->count(); i++)
			{
				if(m_indexSelect == i)
				{
					if(itemPosVisibleFirst + m_heightSelect > m_contentArea.top())
					{
						itemIdxFirst = i;
						break;
					}
					else
					{
						itemPosVisibleFirst += m_heightSelect;
					}
				}
				else
				{
					if(itemPosVisibleFirst + m_heightUnselect > m_contentArea.top())
					{
						itemIdxFirst = i;
						break;
					}
					else
					{
						itemPosVisibleFirst += m_heightUnselect;
					}
				}
			}
			break;
		}
		else
		{
			itemPosVisibleFirst += height50;
		}
	}
#endif
	//最后一个元素显示在列表中的。
	//计算上边框是否隐藏来判断是否为列表可见区中最后一项
	qreal itemPosVisibleLast = itemPosVisibleFirst;
	int itemIdxLast = m_model->count() - 1;
	for(int i = itemIdxFirst; i < m_model->count(); i++)
	{
		if(m_indexSelect == i)
		{
			if(itemPosVisibleLast + m_heightSelect > m_contentArea.bottom())
			{
				itemIdxLast = i;
				break;
			}
			else
			{
				itemPosVisibleLast += m_heightSelect;
			}
		}
		else
		{
			if(itemPosVisibleLast + m_heightUnselect > m_contentArea.bottom())
			{
				itemIdxLast = i;
				break;
			}
			else
			{
				itemPosVisibleLast += m_heightUnselect;
			}
		}
	}
	QHash<int,QPointer<KListItem>> items = m_widgetItems;
	m_widgetItems.clear();
	m_indexWidgetItemBegin = itemIdxFirst;

	/*处理已经重复的对象*/
	qreal itemPosVisible = itemPosVisibleFirst;
	QSet<int> itemsok;
	for(int i = itemIdxFirst; !items.isEmpty() && i <= itemIdxLast; i++)
	{
		KListItem *pli = items.take(i);
		if(pli == NULL)
		{
			if(i == m_indexSelect)
			{
				itemPosVisible += m_heightSelect;
			}
			else
			{
				itemPosVisible += m_heightUnselect;
			}
			continue;
		}
		QRectF itemRt;
		if(i == m_indexSelect)
		{
			itemRt = QRectF(m_contentArea.left(), itemPosVisible, m_contentArea.width(), m_heightSelect);
			itemPosVisible += m_heightSelect;
		}
		else
		{
			itemRt = QRectF(m_contentArea.left(), itemPosVisible, m_contentArea.width(), m_heightUnselect);
			itemPosVisible += m_heightUnselect;
		}
		pli->setMargins(m_itemMargins);
		pli->setGeometry(itemRt);
		pli->setFixSize(itemRt.size());
		if(i == m_indexSelect)
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
		m_widgetItems.insert(i, pli);
		itemsok.insert(i);
	}

	/*处理没有设置的对象。*/
	itemPosVisible = itemPosVisibleFirst;
	for(int i = itemIdxFirst; i <= itemIdxLast; i++)
	{
		if(itemsok.contains(i))
		{
			if(i == m_indexSelect)
			{
				itemPosVisible += m_heightSelect;
			}
			else
			{
				itemPosVisible += m_heightUnselect;
			}
			continue;
		}
		KListItem *pli;
		if(items.isEmpty())
		{
			pli = m_creator->create(q);
		}
		else
		{
			QHash<int,QPointer<KListItem>>::iterator iter = items.begin();
			pli = iter.value();
			items.erase(iter);
		}
		QRectF itemRt;
		if(i == m_indexSelect)
		{
			itemRt = QRectF(m_contentArea.left(), itemPosVisible, m_contentArea.width(), m_heightSelect);
			itemPosVisible += m_heightSelect;
		}
		else
		{
			itemRt = QRectF(m_contentArea.left(), itemPosVisible, m_contentArea.width(), m_heightUnselect);
			itemPosVisible += m_heightUnselect;
		}
		pli->resetIndex(i);
		pli->setMargins(m_itemMargins);
		pli->setGeometry(itemRt);
		pli->setFixSize(itemRt.size());
		if(i == m_indexSelect)
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
		m_widgetItems.insert(i, pli);
	}
	int iLeft = 0;
	for(QHash<int,QPointer<KListItem>>::iterator iter = items.begin(); iter != items.end(); iter++, iLeft++)
	{
		KListItem *pli = iter.value();
		if(iLeft < 3)
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

void KListWidgetPrivate::doMousePress(QGraphicsSceneMouseEvent *event)
{
	Q_Q(KListWidget);
	QPointF pt = event->pos();
	for(QHash<int,QPointer<KListItem>>::iterator iter = m_widgetItems.begin(); iter != m_widgetItems.end(); iter++)
	{
		KListItem *pli = iter.value();
		QRectF geom = pli->geometry();
		if(geom.contains(pt))
		{
			int index = pli->index();
			if(index == m_indexSelect)
			{
				emit q->itemClicked(index, event->button());
				return;
			}
			KListItem *pliOld = m_widgetItems.value(m_indexSelect);
			if(pliOld)
			{
				pliOld->setUnselected(m_variantUnselect);
			}
			m_indexSelect = pli->index();
			pli->setSelected(m_variantSelected);
			emit q->indexChanged(m_indexSelect);
			emit q->itemClicked(index, event->button());
			if(m_heightUnselect != m_heightSelect)
			{
				updateLayout();
			}
			return;
		}
	}
	emit q->clicked(event->button());
}

void KListWidgetPrivate::doMouseRelease(QGraphicsSceneMouseEvent *event)
{
	QPointF pt = event->pos();
}

void KListWidgetPrivate::doMouseDbClick(QGraphicsSceneMouseEvent *event)
{
	Q_Q(KListWidget);
	QPointF pt = event->pos();
	for(QHash<int,QPointer<KListItem>>::iterator iter = m_widgetItems.begin(); iter != m_widgetItems.end(); iter++)
	{
		KListItem *pli = iter.value();
		QRectF geom = pli->geometry();
		if(geom.contains(pt))
		{
			int index = pli->index();
			emit q->itemDbClicked(index, event->button());
			return;
		}
	}
}

void KListWidgetPrivate::doMouseWheel(QGraphicsSceneWheelEvent *event)
{
	Q_Q(KListWidget);
	if(m_scrollbar)
	{
		m_scrollbar->handleMouseWheelEvent(event);
	}
}

int KListWidgetPrivate::findOneIndexByText( const QString& txt )
{
	for(int i = 0; i < m_model->count(); i++)
	{
		QMap<Qt::ItemDataRole,QVariant> vals = m_model->data(i);
		QString hitText = vals.value(Qt::DisplayRole).toString();
		if(hitText == txt)
			return i;
	}
	return -1;
}

QList<int> KListWidgetPrivate::findAllIndexByText( const QString& txt )
{
	QList<int> indexs;
	for(int i = 0; i < m_model->count(); i++)
	{
		QMap<Qt::ItemDataRole,QVariant> vals = m_model->data(i);
		QString hitText = vals.value(Qt::DisplayRole).toString();
		if(hitText == txt)
		{
			indexs.push_back(i);
		}
	}
	return indexs;
}

KAbstractListModel* KListWidgetPrivate::setModel(KAbstractListModel *model)
{
	Q_Q(KListWidget);
	KAbstractListModel *modelOld = m_model;
	if(m_model)
	{
		QObject::disconnect(m_model, SIGNAL(itemsInserted(int,int)), q, SLOT(on_model_itemsInserted(int,int)));
		QObject::disconnect(m_model, SIGNAL(itemsRemoved(int,int)), q, SLOT(on_model_itemsRemoved(int,int)));
		QObject::disconnect(m_model, SIGNAL(itemsMoved(int,int,int)), q, SLOT(on_model_itemsMoved(int,int,int)));
		QObject::disconnect(m_model, SIGNAL(itemsChanged(int,int,QList<Qt::ItemDataRole>)), q, SLOT(on_model_itemsChanged(int,int,QList<Qt::ItemDataRole>)));
		QObject::disconnect(m_model, SIGNAL(modelReset()), q, SLOT(on_model_modelReset()));
	}
	m_model = model;
	QObject::connect(m_model, SIGNAL(itemsInserted(int,int)), q, SLOT(on_model_itemsInserted(int,int)));
	QObject::connect(m_model, SIGNAL(itemsRemoved(int,int)), q, SLOT(on_model_itemsRemoved(int,int)));
	QObject::connect(m_model, SIGNAL(itemsMoved(int,int,int)), q, SLOT(on_model_itemsMoved(int,int,int)));
	QObject::connect(m_model, SIGNAL(itemsChanged(int,int,QList<Qt::ItemDataRole>)), q, SLOT(on_model_itemsChanged(int,int,QList<Qt::ItemDataRole>)));
	QObject::connect(m_model, SIGNAL(modelReset()), q, SLOT(on_model_modelReset()));
	return modelOld;
}

bool KListWidgetPrivate::tryToSetAction( int index, int id, const QVariant& val, QVariant& result )
{
	KListItem *pli = m_widgetItems.value(index);
	if(pli)
	{
		return pli->setAction(id, val, result);
	}
	return false;
}


KListWidget::KListWidget( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KListWidgetPrivate(), parent)
{
	Q_D(KListWidget);

	d->init();

}

KListWidget::KListWidget( KListWidgetPrivate& dd, QGraphicsItem *parent )
: KWidget(dd, parent)
{
	Q_D(KListWidget);
	d->init();
}

KListWidget::~KListWidget()
{

}

bool KListWidget::event(QEvent *e)
{
	Q_D(KListWidget);

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

void KListWidget::setItemCreator( KListItemCreator *creator )
{
	Q_D(KListWidget);
	if(d->m_creator == creator)
		return;
	KX_SAFE_DELETE(d->m_creator);
	d->m_creator = creator;
}

void KListWidget::setItemHeight( int iunselect, int iselect )
{
	Q_D(KListWidget);
	d->m_heightSelect = iselect;
	d->m_heightUnselect = iunselect;
}

int KListWidget::selectIndex()
{
	Q_D(KListWidget);
	return d->m_indexSelect;
}

void KListWidget::setSelectIndex( int i )
{
	Q_D(KListWidget);
	if(i < 0 || i >= d->m_model->count())
		return;
	if(d->m_indexSelect == i)
		return;
	d->m_indexSelect = i;
	d->updateLayout();
	emit indexChanged(i);
}

void KListWidget::construct()
{
	Q_D(KListWidget);
	
}

QMargins KListWidget::itemMargins() const
{
	Q_D(const KListWidget);
	return d->m_itemMargins;
}

void KListWidget::setItemMargins( const QMargins m )
{
	Q_D(KListWidget);
	d->m_itemMargins = m;
}

void KListWidget::setItemBackgroundVariant( const QVariant& vUnselect, const QVariant& vSelected, const QVariant& vHovered )
{
	Q_D(KListWidget);
	d->m_variantSelected = vSelected;
	d->m_variantUnselect = vUnselect;
	d->m_variantHovered = vHovered;
}

QVariant KListWidget::itemUnselectBackgroundVariant() const
{
	Q_D(const KListWidget);
	return d->m_variantUnselect;
}

QVariant KListWidget::itemSelectedBackgroundVariant() const
{
	Q_D(const KListWidget);
	return d->m_variantSelected;
}

QVariant KListWidget::itemHoveredBackgroundVariant() const
{
	Q_D(const KListWidget);
	return d->m_variantHovered;
}

void KListWidget::setContentHiddenArea( int itop, int ibottom )
{
	Q_D(KListWidget);
	d->m_heightContentTopHidden = qMax(itop, 0);
	d->m_heightContentBottomHidden = qMax(ibottom, 0);
}

QObject * KListWidget::verticalScrollBar() const
{
	Q_D(const KListWidget);
	return d->m_scrollbar;
}

void KListWidget::on_scroll_valueChanged( qreal val )
{
	Q_D(KListWidget);
	d->m_valueCurrentScroll = val;
	d->updateLayout();
}

void KListWidget::setItemCustomVariant( const QVariant& val )
{
	Q_D(KListWidget);
	d->m_cfgCustom = val;
}

QVariant KListWidget::itemCustomVariant() const
{
	Q_D(const KListWidget);
	return d->m_cfgCustom;
}

void KListWidget::clearSelected()
{
	Q_D(KListWidget);
	d->m_indexSelect = -1;

	d->updateLayout();
}

int KListWidget::findOneIndexByText( const QString& txt )
{
	Q_D(KListWidget);
	return d->findOneIndexByText(txt);
}

QList<int> KListWidget::findAllIndexByText( const QString& txt )
{
	Q_D(KListWidget);
	return d->findAllIndexByText(txt);
}

KAbstractListModel* KListWidget::model() const
{
	Q_D(const KListWidget);
	return d->m_model;
}

KAbstractListModel* KListWidget::setModel( KAbstractListModel *model )
{
	Q_D(KListWidget);
	return d->setModel(model);
}

void KListWidget::on_model_itemsInserted( int idx, int n )
{
	Q_D(KListWidget);
	d->itemsInserted(idx, n);
}

void KListWidget::on_model_itemsRemoved( int idx, int n )
{
	Q_D(KListWidget);
	d->itemsRemoved(idx, n);
}

void KListWidget::on_model_itemsMoved( int from, int to, int n )
{
	Q_D(KListWidget);
	d->itemsMoved(from, to, n);
}

void KListWidget::on_model_itemsChanged( int idx, int n, const QList<Qt::ItemDataRole> &roles )
{
	Q_D(KListWidget);
	d->itemsChanged(idx, n, roles);
}

void KListWidget::on_model_modelReset()
{
	Q_D(KListWidget);
	d->modelReset();
}

QMap<Qt::ItemDataRole,QVariant> KListWidget::itemData( int idx ) const
{
	Q_D(const KListWidget);
	return d->m_model->data(idx);
}

int KListWidget::count() 
{
	Q_D(KListWidget);
	return d->m_model->count();
}

int KListWidget::itemsHeight()
{
	Q_D(KListWidget);
	return d->itemsHeight();
}

bool KListWidget::tryToSetAction( int index, int id, const QVariant& val, QVariant& result )
{
	Q_D(KListWidget);
	return d->tryToSetAction(index, id, val, result);
}



KListItem::KListItem( KListWidget *view )
 :KWidget(view)
 ,m_listWidget(view)
 ,m_model(m_listWidget->model())
{
	setAcceptHoverEvents(true);
}

KListItem::~KListItem()
{

}

int KListItem::index() const
{
	return m_index;
}

void KListItem::itemReset( )
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

QString KListItem::itemIcon() const
{
	return m_data.value(Qt::DecorationRole).toString();
}

QString KListItem::itemText() const
{
	return m_data.value(Qt::DisplayRole).toString();
}

QVariant KListItem::itemData(Qt::ItemDataRole role) const
{
	return m_data.value(role).toString();
}

KListWidget * KListItem::listView() const
{
	return m_listWidget;
}

bool KListItem::itemSelected() const
{
	return m_index == m_listWidget->selectIndex();
}

void KListItem::setSelected( const QVariant& bv )
{
	if(bv.type() == QVariant::Color)
	{
		QColor clr = bv.value<QColor>();
		setBackgroundColor(clr);
	}
}

void KListItem::setUnselected( const QVariant& bv )
{
	if(bv.type() == QVariant::Color)
	{
		QColor clr = bv.value<QColor>();
		setBackgroundColor(clr);
	}
}

void KListItem::setHovered( const QVariant& bv )
{
	if(bv.type() == QVariant::Color)
	{
		QColor clr = bv.value<QColor>();
		setBackgroundColor(clr);
	}
}

void KListItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	if(!itemSelected())
	{
		QVariant v = m_listWidget->itemHoveredBackgroundVariant();
		setHovered(v);
	}
}

void KListItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	if(!itemSelected())
	{
		QVariant v = m_listWidget->itemUnselectBackgroundVariant();
		setUnselected(v);
	}
}

QVariant KListItem::customVariant() const
{
	return m_listWidget->itemCustomVariant();
}

void KListItem::resetIndex( const int idx )
{
	m_index = idx;
	m_data = m_listWidget->itemData(idx);
	itemReset();
}

bool KListItem::setAction(int id, const QVariant& val, QVariant& result)
{
	return false;
}
