#include "stdafx.h"
#include "kwidget.h"
#include "kwidget_p.h"
#include "kxmlui.h"
#include "ktheme.h"
#include "kutility.h"
#include "kwindow.h"
#include "qgraphicsflowlayout.h"
#include "xmlParser.h"



KX_WIDGET_CREATOR_GLOBAL_STATIC(KWidget)


KWidgetPrivate::KWidgetPrivate()
{
}

KWidgetPrivate::~KWidgetPrivate()
{

}

void KWidgetPrivate::init()
{
	Q_Q(KWidget);
	dragPolicy = KWidget::AskParent;
	themePolicy = KWidget::ThemeWithParent;
	bAddToAnchor = false;
	bThemeCheck = false;
	bRemovePostEvent = false;
	grayEffect = NULL;
	layoutType = KWidget::None;
	contexMenuPolicy = Qt::NoContextMenu;
	q->setFocusPolicy(Qt::ClickFocus);
	q->setContentsMargins(0, 0, 0, 0);
	q->resetSizeHint();
/*
	static int s_index = 0;
	QString name = QString("EmptyName%1").arg(s_index++);
	q->setObjectName(name);*/
	q->setAcceptHoverEvents(false);
	q->setAcceptedMouseButtons(0);
}



KWidget::KWidget( QGraphicsItem *parent /* = NULL*/ ) 
: QGraphicsWidget(parent)
, d_ptr(new KWidgetPrivate)
{
	d_ptr->q_ptr = this;
	d_ptr->init();
}

KWidget::KWidget( KWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ )
: QGraphicsWidget(parent) 
, d_ptr(&dd)
{
	d_ptr->q_ptr = this;
	d_ptr->init();
}

KWidget::~KWidget()
{
	if(d_ptr->bRemovePostEvent)
	{
		QCoreApplication::removePostedEvents(this);
	}
	delete d_ptr;
}

qreal KWidget::width() const
{
	return size().width();
}

qreal KWidget::height() const
{
	return size().height();
}


qreal KWidget::fixWidth() const
{
	return __super::preferredWidth();
}

void KWidget::setFixWidth( qreal w )
{
	setMinimumWidth(w);
	setPreferredWidth(w);
	setMaximumWidth(w);
}

qreal KWidget::fixHeight() const
{
	return __super::preferredHeight();
}

void KWidget::setFixHeight( qreal h )
{
	setMinimumHeight(h);
	setPreferredHeight(h);
	setMaximumHeight(h);
}

QSizeF KWidget::fixSize() const
{
	return __super::preferredSize();
}

void KWidget::setFixSize( const QSizeF& s )
{
	setMinimumSize(s);
	setPreferredSize(s);
	setMaximumSize(s);
}

void KWidget::setFixSize( qreal w, qreal h )
{
	setFixSize(QSizeF(w,h));
}

Qt::ContextMenuPolicy KWidget::contextMenuPolicy()
{
	return d_func()->contexMenuPolicy;
}

void KWidget::setContextMenuPolicy( Qt::ContextMenuPolicy c )
{
	d_func()->contexMenuPolicy = c;
}

QString KWidget::toolTip()
{
	return d_func()->toolTipText;
}

void KWidget::setToolTip( const QString& tip )
{
	d_func()->toolTipText = tip;
}

bool KWidget::bylayout()
{
	KWidget *itemParent = qobject_cast<KWidget*>(parent());
	if(itemParent == NULL)
		return false;
	QGraphicsLayoutItem *item = dynamic_cast<QGraphicsLayoutItem*>(this);
	QGraphicsLayout *layout = itemParent->layout();
	for(int i = 0; i < layout->count(); i++)
	{
		if(layout->itemAt(i) == item)
			return true;
	}
	return false;
}

void KWidget::setByLayout( bool by )
{
	KWidget *itemParent = qobject_cast<KWidget*>(parent());
	if(itemParent == NULL)
		return;
	if(by)			
	{
		itemParent->addLayoutItem(this);
	}
	else
	{
		itemParent->removeLayoutItem(this);
	}
}

QColor KWidget::backgroundColor() const
{
	QPalette p = palette();
	return p.color(QPalette::Window);
}

void KWidget::setBackgroundColor( const QColor& clr )
{
	Q_D(KWidget);
	if(d->backgroundBrush == QBrush(clr))
		return;
	d->backgroundBrush = QBrush(clr);
	updateBackgroundBrush();
}

void KWidget::setBackgroundColor( int r, int g, int b, int a /*= 255*/ )
{
	setBackgroundColor(QColor(r, g, b, a));
}

QBrush KWidget::backgroundBrush() const
{
	Q_D(const KWidget);
	QPalette p = palette();
	return p.brush(QPalette::Window);
}

void KWidget::setBackgroundBrush( const QBrush& brush )
{
	Q_D(KWidget);
	if(d->backgroundBrush == brush)
		return;
	d->backgroundBrush = brush;
	updateBackgroundBrush();
}

Qt::BrushStyle KWidget::backgroundStyle() const
{
	Q_D(const KWidget);
	return d->backgroundBrush.style();
}

void KWidget::setBackgroundStyle( Qt::BrushStyle bs )
{
	Q_D(KWidget);
	if(d->backgroundBrush.style() == bs)
		return;
	d->backgroundBrush.setStyle(bs);
	updateBackgroundBrush();
}

QtGradient KWidget::backgroundGradient() const
{
	Q_D(const KWidget);
	return QtGradient();
}

void KWidget::setBackgroundGradient( const QtGradient& gd )
{
	Q_D(KWidget);
	QBrush brush;
	switch(gd.t)
	{
	case QGradient::LinearGradient:
		{
			QLinearGradient lg(gd.d.l.startx, gd.d.l.starty, gd.d.l.finalx, gd.d.l.finaly);
			lg.setCoordinateMode(QGradient::ObjectBoundingMode);
			lg.setSpread(gd.spread);
			lg.setStops(gd.gs);
			brush = QBrush(lg);
		}
		break;
	case QGradient::ConicalGradient:
		{
			QConicalGradient cg(gd.d.c.centerx, gd.d.c.centery,gd.d.c.angle);
			cg.setCoordinateMode(QGradient::ObjectBoundingMode);
			cg.setSpread(gd.spread);
			cg.setStops(gd.gs);
			brush = QBrush(cg);
		}
		break;
	case QGradient::RadialGradient:
		{
			QRadialGradient rg(gd.d.r.centerx, gd.d.r.centery,gd.d.r.radius);
			rg.setCoordinateMode(QGradient::ObjectBoundingMode);
			rg.setSpread(gd.spread);
			rg.setStops(gd.gs);
			brush = QBrush(rg);
		}
		break;
	}
	if(d->backgroundBrush == brush)
		return;
	d->backgroundBrush = brush;
	updateBackgroundBrush();
}

KWidget::LayoutType KWidget::layoutType()
{
	return d_func()->layoutType;
}

void KWidget::setLayoutType( KWidget::LayoutType type )
{
	d_func()->layoutType = type;
	if(type == VBox || type == HBox)
	{
		setLayout(new QGraphicsLinearLayout(type == HBox ? Qt::Horizontal : Qt::Vertical, this));
	}
	else if(type == Anchor)
	{
		setLayout(new QGraphicsAnchorLayout(this));
	}
	else if(type = Flow)
	{
		setLayout(new QGraphicsFlowLayout(this));
	}
}

void KWidget::setLayout(QGraphicsLayout *layout)
{
	QGraphicsWidget::setLayout(layout);
	setMargins(QMargins(0, 0, 0, 0));
	setSpacing(0);
}

int KWidget::spacing()
{
	QGraphicsLayout *tmpLayout = layout();
	if(d_func()->layoutType == VBox || d_func()->layoutType == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(tmpLayout);
		if(boxLayout)
		{
			return boxLayout->spacing();
		}
	}
	return 0;
}

void KWidget::setSpacing( int space )
{
	QGraphicsLayout *tmpLayout = layout();
	if(d_func()->layoutType == VBox || d_func()->layoutType == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(tmpLayout);
		if(boxLayout)
		{
			return boxLayout->setSpacing(space);
		}
	}
}

int KWidget::itemSpacing()
{
	QGraphicsLayout *layout = parentLayout();
	if(d_func()->layoutType == VBox || d_func()->layoutType == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(layout);
		if(boxLayout)
		{
			QGraphicsLayoutItem *item = dynamic_cast<QGraphicsLayoutItem*>(this);
			for(int i = 0; i < boxLayout->count(); i++)
			{
				if(boxLayout->itemAt(i) == item)
				{
					return boxLayout->itemSpacing(i);
				}
			}
		}
	}
	return 0;
}

void KWidget::setItemSpacing( int space )
{
	QGraphicsLayout *layout = parentLayout();
	LayoutType type = parentLayoutType();
	if(type == VBox || type == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(layout);
		if(boxLayout)
		{
			QGraphicsLayoutItem *item = dynamic_cast<QGraphicsLayoutItem*>(this);
			for(int i = 0; i < boxLayout->count(); i++)
			{
				if(boxLayout->itemAt(i) == item)
				{
					boxLayout->setItemSpacing(i, space);
					return ;
				}
			}
		}
	}
}

QMargins KWidget::margins()
{
	QGraphicsLayout *tmpLayout = layout();
	if(tmpLayout == NULL)
		return QMargins();
	qreal l, t, r, b;
	tmpLayout->getContentsMargins(&l, &t, &r, &b);
	return QMargins(l, t, r, b);
}

void KWidget::setMargins( const QMargins& m )
{
	QGraphicsLayout *tmpLayout = layout();
	if(tmpLayout == NULL)
		return;
	tmpLayout->setContentsMargins(m.left(), m.top(), m.right(), m.bottom());
}

QString KWidget::anchor()
{
	return QString();
}

void KWidget::setAnchor( const QString& a )
{
	Q_D(KWidget);
	//检查布局
	KWidget* p = qobject_cast<KWidget*>(parent());
	Q_ASSERT_X(p, __FUNCTION__, "parent is null");
	Q_ASSERT_X(p->layoutType() == KWidget::Anchor, __FUNCTION__, "layout type should be anchor");
	QGraphicsLayout *layout = p->layout();
	QGraphicsAnchorLayout *anchorLayout = static_cast<QGraphicsAnchorLayout*>(layout);
	if(anchorLayout == NULL)
	{
		Q_ASSERT_X(false, __FUNCTION__, "layout is not group layout");
		return ;
	}

	if(d->bAddToAnchor)
	{
		p->removeLayoutItem(this);
	}
	//参数转换
	int index = KWidget::staticMetaObject.indexOfEnumerator("AnchorPoint");
	QMetaEnum enu = KWidget::staticMetaObject.enumerator(index);
	QStringList list = a.split('|');
	Q_ASSERT_X(list.count()==4, __FUNCTION__, QString("%1 is not valide anchor info").arg(a).toLatin1().constData());
	Qt::AnchorPoint anchorPoint[4]={Qt::AnchorLeft};
	int distance[4]={0};
	for (int i = 0; i < list.count() && i < 4; ++i)
	{
		QStringList innerList = list[i].split(':');
		Q_ASSERT_X(innerList.count()==2, __FUNCTION__, QString("%1 is not valide anchor info").arg(a).toLatin1().constData());
		if (innerList.count() >= 2)
		{
			int value = enu.keyToValue(innerList[0].toLatin1().constData());
			Q_ASSERT_X(value != -1, __FUNCTION__, (innerList[0] + "is not a valid enum value").toLatin1().constData());
			if (value == -1)
			{
				value = 0;
			}
			anchorPoint[i] = (Qt::AnchorPoint)value;
			distance[i] = innerList[1].toInt();
		}
	}

	//AnchorLeft:
	int idx = 0;
	QGraphicsAnchor *anchorLeft = anchorLayout->addAnchor(anchorLayout, anchorPoint[idx], this, Qt::AnchorLeft);
	if(anchorLeft)
	{
		anchorLeft->setSpacing(distance[idx]);
	}
	else if(anchorPoint[idx] >= Qt::AnchorTop)
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("it's not the same orientation for AnchorLeft: bad string is[%1]").arg(list.at(idx)).toStdString().c_str());
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad parameter.bad string:%1").arg(list.at(idx)).toStdString().c_str());
	}

	//AnchorTop:
	idx = 1;
	QGraphicsAnchor *anchorTop = anchorLayout->addAnchor(anchorLayout, anchorPoint[idx], this, Qt::AnchorTop);
	if(anchorTop)
	{
		anchorTop->setSpacing(distance[idx]);
	}
	else if(anchorPoint[idx] < Qt::AnchorTop)
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("it's not the same orientation for AnchorTop: bad string is[%1]").arg(list.at(idx)).toStdString().c_str());
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad parameter.bad string:%1").arg(list.at(idx)).toStdString().c_str());
	}

	//AnchorRight:
	idx = 2;
	QGraphicsAnchor *anchorRight = anchorLayout->addAnchor(this, Qt::AnchorRight, anchorLayout, anchorPoint[idx]);
	if(anchorRight)
	{
		anchorRight->setSpacing(distance[idx]);
	}
	else if(anchorPoint[idx] >= Qt::AnchorTop)
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("it's not the same orientation for AnchorRight: bad string is[%1]").arg(list.at(idx)).toStdString().c_str());
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad parameter.bad string:%1").arg(list.at(idx)).toStdString().c_str());
	}

	//AnchorBottom
	idx = 3;
	QGraphicsAnchor *anchorBottom = anchorLayout->addAnchor(this, Qt::AnchorBottom, anchorLayout, anchorPoint[idx]);
	if(anchorBottom)
	{
		anchorBottom->setSpacing(distance[idx]);
	}
	else if(anchorPoint[idx] < Qt::AnchorTop)
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("it's not the same orientation for AnchorBottom: bad string is[%1]").arg(list.at(idx)).toStdString().c_str());
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad parameter.bad string:%1").arg(list.at(idx)).toStdString().c_str());
	}
	d->bAddToAnchor = true;
}


void KWidget::addItem( KWidget* item )
{
	addObjectItem(item);
	addGraphicsItem(item);
	addLayoutItem(item);
}

void KWidget::insertItem( KWidget *item, int i )
{
	addObjectItem(item);
	addGraphicsItem(item);
	insertLayoutItem(item, 0);
}

void KWidget::removeItem( KWidget *item )
{
	removeLayoutItem(item);
	removeGraphicsItem(item);
	removeObjectItem(item);
}

void KWidget::addChild(KWidget* child)
{
	addGraphicsItem(child);
	addObjectItem(child);
}

void KWidget::removeChild(KWidget* child)
{
	removeGraphicsItem(child);
	removeObjectItem(child);
}

void KWidget::addLayoutItem( KWidget* item )
{
	if(item == NULL)
		return;
	QGraphicsLayout *tmpLayout = layout();
	if(tmpLayout == NULL)
		return ;
	Q_D(KWidget);
	if(d->layoutType == VBox || d->layoutType == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(tmpLayout);
		if(boxLayout)
		{
			boxLayout->addItem(item);
		}
	}
	else if(d->layoutType == Flow)
	{
		QGraphicsFlowLayout *flowLayout = dynamic_cast<QGraphicsFlowLayout*>(tmpLayout);
		flowLayout->addItem(item);
	}
}

void KWidget::removeLayoutItem( KWidget *item )
{
	Q_D(KWidget);
	if(item == NULL)
		return;
	QGraphicsLayout *tmpLayout = layout();
	if(tmpLayout == NULL)
		return;
	d->bAddToAnchor = false;
	for(int i = 0; i < tmpLayout->count(); i++)
	{
		if(tmpLayout->itemAt(i) == item)
		{
			tmpLayout->removeAt(i);
			break;
		}
	}
}

void KWidget::insertLayoutItem( KWidget* item, int index )
{
	QGraphicsLayout *tmpLayout = (QGraphicsLayout*)(layout());
	if(tmpLayout == NULL)
		return;
	if(d_func()->layoutType == VBox || d_func()->layoutType == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(tmpLayout);
		if(boxLayout)
		{
			boxLayout->insertItem(index, item);
		}
	}
}

void KWidget::addGraphicsItem( KWidget* ctrl )
{
	ctrl->setParentItem(this);
}

void KWidget::removeGraphicsItem( KWidget* ctrl )
{
	ctrl->setParentItem(NULL);
}

void KWidget::addObjectItem( KWidget* item )
{
	item->setParent(this);
}

void KWidget::removeObjectItem( KWidget *item )
{
	item->setParent(NULL);
}

bool KWidget::hasContent()
{
	return !(flags() & QGraphicsItem::ItemHasNoContents);
}

void KWidget::enableContent( bool on )
{
	setFlag(QGraphicsItem::ItemHasNoContents, !on);
}

Qt::Alignment KWidget::alignment()
{
	QGraphicsLayout *tmpLayout = (QGraphicsLayout*)(parentLayout());
	if(tmpLayout == NULL)
		return Qt::Vertical;
	if(d_func()->layoutType == VBox || d_func()->layoutType == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(tmpLayout);
		if(boxLayout)
		{
			return boxLayout->alignment(this);
		}
	}
	return Qt::Vertical;
}

void KWidget::setAlignment( Qt::Alignment align )
{
	QGraphicsLayout *tmpLayout = (QGraphicsLayout*)(parentLayout());
	if(tmpLayout == NULL)
		return ;
	LayoutType type = parentLayoutType();
	if(type == VBox || type == HBox)
	{
		QGraphicsLinearLayout *boxLayout = dynamic_cast<QGraphicsLinearLayout*>(tmpLayout);
		if(boxLayout)
		{
			boxLayout->setAlignment(this, align);
		}
	}
}

void KWidget::setDragPolicy( DragPolicy flag )
{
	d_func()->dragPolicy = flag;
}

KWidget::DragPolicy KWidget::dragPolicy() const
{
	return d_func()->dragPolicy;
}

KWidget::DragPolicy KWidget::hitTest( const QPointF& point )
{
	Q_D(const KWidget);
	return d->dragPolicy;
}

QGraphicsView * KWidget::view() const
{
	QGraphicsScene *gs = scene();
	if(gs == NULL)
		return NULL;
	QList<QGraphicsView*>& lstgv = gs->views();
	if(lstgv.isEmpty())
		return NULL;
	return lstgv[0];
}

QWidget * KWidget::viewport() const
{
	QGraphicsView *gv = view();
	if(gv == NULL)
		return NULL;
	return gv->viewport();
}

void KWidget::setData( const QVariant& v )
{
	int i = v.toInt();
	int j = 0;
}

void KWidget::setData( const XMLNode *v )
{
	int i = 0;
}

QVariant KWidget::data( int key )
{
	return __super::data(key);
}

void KWidget::setData( int key, const QVariant& v )
{
	__super::setData(key, v);
}

void KWidget::resetSizeHint()
{
	QSize vscreen = KTheme::virtualScreenSize();
	vscreen *= 3;
	setPreferredSize(vscreen.width(), vscreen.height());
	setMaximumSize(vscreen.width(), vscreen.height());
}

void KWidget::clearSizeHint()
{
	setMinimumSize(-1, -1);
	setPreferredSize(-1, -1);
	setMaximumSize(-1, -1);
}

void KWidget::activateLayout()
{
	QGraphicsLayout *layout = this->layout();
	layout->activate();
}

QRect KWidget::geometryToScreen()
{
	return KUtility::mapWidgetToScreen(this);
}

void KWidget::construct()
{

}

QGraphicsLayout * KWidget::parentLayout()
{
	if(parent() == NULL)
		return NULL;
	KWidget *itemParent = qobject_cast<KWidget*>(parent());
	if(itemParent == NULL)
		return NULL;
	return itemParent->layout();
}

KWidget::LayoutType KWidget::parentLayoutType()
{
	if(parent() == NULL)
		return None;
	KWidget *itemParent = qobject_cast<KWidget*>(parent());
	if(itemParent == NULL)
		return None;
	return itemParent->layoutType();
}

KWidget::WidgetFlags KWidget::widgetFlags() const
{
	QGraphicsItem::GraphicsItemFlags flags = __super::flags();
	flags &= (quint32)All;
	return (quint32)flags;
}

void KWidget::setWidgetFlags( WidgetFlags flags )
{
	QGraphicsItem::GraphicsItemFlags vals = __super::flags();
	if(flags.testFlag(HasNoContents))
		vals |= QGraphicsItem::ItemHasNoContents;
	else
		vals &= ~QGraphicsItem::ItemHasNoContents;
	if(flags.testFlag(ClipsChildrenToShape))
		vals |= QGraphicsItem::ItemClipsChildrenToShape;
	else
		vals &= ~QGraphicsItem::ItemClipsChildrenToShape;
	if(flags.testFlag(ClipsToShape))
		vals |= QGraphicsItem::ItemClipsToShape;
	else 
		vals &= ~QGraphicsItem::ItemClipsToShape;
	if(flags.testFlag(IgnoresParentOpacity))
		vals |= QGraphicsItem::ItemIgnoresParentOpacity;
	else
		vals &= ~QGraphicsItem::ItemIgnoresParentOpacity;
	if(flags.testFlag(ScenePosition))
		vals |= QGraphicsItem::ItemSendsScenePositionChanges;
	else
		vals &= ~QGraphicsItem::ItemSendsScenePositionChanges;

	setFlags(vals);
}

bool KWidget::itemPanel() const
{
	QGraphicsItem::GraphicsItemFlags vals = __super::flags();
	return vals.testFlag(QGraphicsItem::ItemIsPanel) && acceptHoverEvents();
}

void KWidget::setItemPanel( bool bPanel )
{
	setFlag(QGraphicsItem::ItemIsPanel, bPanel);
	setAcceptHoverEvents(bPanel);
}

void KWidget::setAcceptedMouseButtons( Qt::MouseButtons buttons )
{
	Q_D(KWidget);
	__super::setAcceptedMouseButtons(buttons);
	if((buttons & Qt::LeftButton) 
		&& d->dragPolicy == AskParent)
	{
		d->dragPolicy = Nothing;
	}
}

bool KWidget::sizeToChild()
{
	QSizeF mins = minimumSize();
	QSizeF maxs = maximumSize();
	if(mins.isValid() && maxs.isValid())
		return false;
	return true;
}

void KWidget::setSizeToChild( bool on )
{
	if(on)
	{
		clearSizeHint();
	}
	else
	{
		resetSizeHint();
	}
}

bool KWidget::widthToChild()
{
	qreal minwidth = minimumWidth();
	qreal maxwidth = maximumWidth();
	if(minwidth == -1.0 && maxwidth == -1.0)
		return true;
	return false;
}

void KWidget::setWidthToChild(bool on)
{
	setPreferredWidth(-1);
	setMaximumWidth(-1);
	setMinimumWidth(-1);
}

bool KWidget::heightToChild()
{
	qreal minwidth = minimumHeight();
	qreal maxwidth = maximumHeight();
	if(minwidth == -1.0 && maxwidth == -1.0)
		return true;
	return false;
}

void KWidget::setHeightToChild(bool on)
{
	setPreferredHeight(-1);
	setMaximumHeight(-1);
	setMinimumHeight(-1);
}

void KWidget::removeBadEventWhenDelete()
{
	Q_D(KWidget);
	/*在销废该对象时，只有队列中的相关PostEvent才有可能对系统造成崩溃*/
	d->bRemovePostEvent = true;
}

KWidget::ThemePolicy KWidget::themePolicy() const
{
	Q_D(const KWidget);
	return d->themePolicy;
}

void KWidget::setThemePolicy( KWidget::ThemePolicy tp )
{
	Q_D(KWidget);
	d->themePolicy = tp;
	clearThemeCheckFlag();
}

void KWidget::changeTheme()
{
	if(hasTheme())
	{
		doThemeChange();
	}
}

void KWidget::doThemeChange()
{
	updateBackgroundBrush();
}

void KWidget::unsetBackgroundBrush()
{
	Q_D(KWidget);

	setAutoFillBackground(false);
	QPalette p = palette();
	QBrush brush;
	d->backgroundBrush = brush;
	p.setBrush(QPalette::Window, brush);
	setPalette(p);
}

void KWidget::updateBackgroundBrush()
{
	Q_D(KWidget);
	
	if(d->backgroundBrush.style() == Qt::NoBrush)
		return;
	QBrush brush;
	if(hasTheme())
	{
		brush = KTheme::convertToThemeBrush(d->backgroundBrush);
	}
	else
	{
		brush = d->backgroundBrush;
	}

	setAutoFillBackground(true);
	QPalette p = palette();
	p.setBrush(QPalette::Window, brush);
	setPalette(p);
}

bool KWidget::hasTheme()
{
	Q_D(KWidget);
	if(d->themePolicy == NoTheme)
		return false;
	if(d->themePolicy == ThemeWithParent)
	{
		if(d->bThemeCheck)
		{
			return d->bThemeResult;
		}
#ifdef _DEBUG
		QString objName = objectName();
		if(objName == "PushButtonBk")
		{
			int i = 0;
		}
#endif
		d->bThemeCheck = true;
		QGraphicsItem *itemParent = parentItem();
		while(itemParent)
		{
			KWidget *widgetParent = qobject_cast<KWidget*>(itemParent->toGraphicsObject());
			if(widgetParent == NULL)
			{
				itemParent = itemParent->parentItem();
				continue;
			}
#ifdef _DEBUG
			QString objName = widgetParent->objectName();
			if(objName == "btn1")
			{
				int i = 0;
			}
#endif
			ThemePolicy scp = widgetParent->themePolicy();
			if(scp == NoTheme)
			{
				d->bThemeResult = false;
				return false;
			}
			if(scp == ThemeWithParent)
			{
				itemParent = itemParent->parentItem();
				continue;
			}
			d->bThemeResult = true;
			return true;
		}
		d->bThemeResult = false;
		return false;
	}
	return true;
}

QVariant KWidget::itemChange( GraphicsItemChange change, const QVariant & value )
{
	Q_D(KWidget);
	if(change == QGraphicsItem::ItemParentHasChanged
		|| change == QGraphicsItem::ItemParentChange)
	{
		clearThemeCheckFlag();
	}
	else if(change == QGraphicsItem::ItemScenePositionHasChanged)
	{
		emit scenePosition(value.toPointF());
	}
	else if(change == QGraphicsItem::ItemVisibleHasChanged)
	{
		bool bvis = isVisible();
		visibleEvent(bvis);
		if(bvis)
		{
			showEvent();
		}
		else
		{
			hideEvent();
		}
	}
	return __super::itemChange(change, value);
}

void KWidget::clearThemeCheckFlag()
{
	Q_D(KWidget);
	d->bThemeCheck = false;
	QList<QGraphicsItem*> items = childItems();
	for(int i = 0; i < items.count(); i++)
	{
		KWidget *widget = qobject_cast<KWidget*>(items.at(i)->toGraphicsObject());
		if(widget == NULL)
		{
			continue;
		}
		widget->clearThemeCheckFlag();
	}
}

QString KWidget::xmlPath() const
{
	return "";
}

void KWidget::setXmlPath( const QString& xml )
{
	if(xml.isEmpty())
		return;
	KXmlUI::widgetFromFile(xml, this);
}

QRectF KWidget::rectToScreen() const
{
	QRectF rt = rect();
	QGraphicsView *gview = view();

	QPointF pt = mapToScene(rt.topLeft());
	QPoint vpt = gview->mapFromScene(pt);
	QPoint gpt = gview->mapToGlobal(vpt);
	rt.moveTo(gpt);
	return rt;
}

QRectF KWidget::rectToView() const
{
	QGraphicsView *gview = view();
	QRectF rt = rect();
	
	QPointF pt = mapToScene(rt.topLeft());
	QPoint vpt = gview->mapFromScene(pt);
	rt.moveTo(vpt);
	return rt;
}

QRectF KWidget::rectToScene() const
{
	QRectF rt = rect();

	QPointF pt = mapToScene(rt.topLeft());
	rt.moveTo(pt);
	return rt;
}



QObject * KWidget::findObject( const QString& name )
{
	return KUtility::findObject(this, name);
}

bool KWidget::grayToDisable()
{
	Q_D(KWidget);
	return d->grayEffect != NULL;
}

void KWidget::setGrayToDisable( bool on )
{
	Q_D(KWidget);
	if(on)
	{
		if(d->grayEffect)
			return;
		d->grayEffect = new QGraphicsColorizeEffect(this);
		setGraphicsEffect(d->grayEffect);
		d->grayEffect->setColor("gray");
		d->grayEffect->setEnabled(!isEnabled());
		QObject::connect(this, SIGNAL(enabledChanged()), this, SLOT(on_enabledChanged()), Qt::UniqueConnection);
	}
	else
	{
		QObject::disconnect(this, SIGNAL(enabledChanged()), this, SLOT(on_enabledChanged()));
		setGraphicsEffect(NULL);
	}
}

void KWidget::on_enabledChanged()
{
	Q_D(KWidget);
	d->grayEffect->setEnabled(!isEnabled());
}

QSizeF KWidget::calcIdealSize()
{
	setFixSize(-1,-1);
	invalidateLayout();
	return minimumSize();
}

void KWidget::invalidateLayout()
{
	QGraphicsLayout *layout = this->layout();
	if(layout == NULL)
		return;
	layout->invalidate();
	QList<QGraphicsItem *> childs = childItems();
	for(int i = 0; i < childs.count(); i++)
	{
		QGraphicsItem *item = childs[i];
		KWidget *widget = qobject_cast<KWidget*>(item->toGraphicsObject());
		if(widget == NULL)
			continue;
		widget->invalidateLayout();
	}
}

void KWidget::visibleEvent( bool bvis )
{

}

void KWidget::showEvent()
{

}

void KWidget::hideEvent()
{

}

QString KWidget::skinName() const
{
	return QString();
}

void KWidget::setSkinName( const QString& name )
{
	//直接调用全局皮肤设置。
	KTheme::setSkinName(this, name);
}

qreal KWidget::xRotateAngle()
{
	return 0.0;	
}

void KWidget::setxRotateAngle( qreal angle )
{
	QTransform t = transform();

	setTransform(t.rotate(angle, Qt::XAxis));
}

qreal KWidget::yRotateAngle()
{
	return 0.0;
}

void KWidget::setyRotateAngle( qreal angle )
{
	QTransform t = transform();
	setTransform(t.rotate(angle, Qt::YAxis));
}

qreal KWidget::zRotateAngle()
{
	return 0.0;
}

void KWidget::setzRotateAngle( qreal angle )
{
	QTransform t = transform();
	setTransform(t.rotate(angle, Qt::ZAxis));
}

bool KWidget::setPropertyEx( const char* name, const char* value )
{
	return KXmlUI::setPropertyEx(this, name, value);
}

KWidget * KWidget::layoutWidget()
{
	return this;
}

bool KWidget::isUnderMouseEx()
{
	QPoint pt = QCursor::pos();
	QRectF rt = rectToScreen();
	return rt.contains(pt);
}
