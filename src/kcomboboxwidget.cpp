#include "stdafx.h"
#include "kcomboboxwidget.h"
#include "kcommonlistwidget.h"
#include "kframeitem.h"
#include "kcheckbutton.h"
#include "kxmlui.h"
#include "kwindow_p.h"
#include "kutility.h"
#include "kspacer.h"
#include "kcomboboxwidget_p.h"
#include "kicon.h"
#include "kabstractlistmodel.h"

#define DROPLISTHEIGHT	(80)
#define DROPBUTTONWIDTH	(30)
#define FRAME_MARGINS	QMargins(1,1,1,1)


KX_WIDGET_CREATOR_GLOBAL_STATIC(KComboBoxWidget)


class KPopupListWindow : public KFollowWindow
{
public:
	QPointer<KCommonListWidget> dropList;
	QPointer<KFrameItem> frame;
public:
	explicit KPopupListWindow(QWidget *parent=NULL)
		: KFollowWindow(parent)
	{
		KWidget *root = rootWidget();
		frame = new KFrameItem(root);
		frame->setFrameType(KFrameItem::GridFour);
		frame->setImagePath(":/image/theme/listbox_bk.png");
		frame->setLayoutType(KWidget::VBox);
		dropList = new KCommonListWidget(frame);
		root->addItem(frame);
		frame->addItem(dropList);
		frame->setMargins(QMargins(FRAME_MARGINS));
	}
	~KPopupListWindow()
	{

	}

	KCommonListWidget *listWidget()
	{
		return dropList;
	}

	void setTarget(KWidget *w)
	{
		KWidget *wold = target();
		if(wold)
		{
			listenEvent(wold, abortAll);
		}
		listenEvent(w, dockFlag);
		__super::setTarget(w);
	}
};

KComboBoxWidgetPrivate::KComboBoxWidgetPrivate()
: dropList(NULL)       
, content(NULL)
, dropListView(NULL)
, dropButton(NULL)
, dropListMaxHeight(DROPLISTHEIGHT)
, dropListMinWidth(0)
{
}

KComboBoxWidget::KComboBoxWidget( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KComboBoxWidgetPrivate(), parent)
{
	init();
}

KComboBoxWidget::KComboBoxWidget( KComboBoxWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ )
: KWidget(dd, parent)
{
	init();
}

KComboBoxWidget::~KComboBoxWidget()
{
}


void KComboBoxWidget::init()
{
	Q_D(KComboBoxWidget);
	setFlag(QGraphicsItem::ItemHasNoContents);
	setMargins(QMargins(0,1,0,1));
	setLayoutType(KWidget::HBox);
	setAcceptHoverEvents(true);

	d->frame = new KFrameItem(this);
	d->frame->setImagePath(":/image/theme/roundframe_normal.png|:/image/theme/roundframe_hover.png|:/image/theme/roundframe_down.png");
	d->frame->setFrameType(KFrameItem::GridFour);

	d->content = new KSpacer(this);
	d->dropButton = new KFrameItem(this);
	d->dropButton->setFrameType(KFrameItem::GridFour);
	d->dropButton->setFrameSwitch(true);
	d->dropButton->setAcceptClick(true);
	d->dropButton->setAcceptHover(true);
	d->dropButton->setImagePath(":/image/theme/comboboxbutton_normal.png|:/image/theme/comboboxbutton_hover.png|:/image/theme/comboboxbutton_down.png|:/image/theme/comboboxbutton_normal.png"
		":/image/theme/comboboxbutton_normal.png|:/image/theme/comboboxbutton_hover.png|:/image/theme/comboboxbutton_down.png|:/image/theme/comboboxbutton_normal.png");
	d->dropButton->setFixWidthByPixmapWidth();
	d->dropButton->setLayoutType(KWidget::VBox);
	d->content->setZValue(1.0);
	d->dropButton->setZValue(1.0);
	KWidget::addItem(d->content);
	KWidget::addItem(d->dropButton);

	d->buttonIcon = new KIcon(d->dropButton);
	d->buttonIcon->setIconPath(":/image/theme/comboboxbutton_icon.png");
	d->dropButton->addItem(d->buttonIcon);

	KPopupListWindow *window = new KPopupListWindow(view());
	d->dropListView = window;
	window->setDirection(KFollowWindow::BottomLeft);
	KFollowWindow::HideFlags flags = KFollowWindow::AlwaysHide;
	window->setHideFlags(flags &~KFollowWindow::ClickAtMySelf);
	window->setTarget(this);
	d->dropList = window->listWidget();

	bool bok1 = QObject::connect(d->dropList, SIGNAL(indexChanged(int)), this, SLOT(on_dropList_indexChanged(int)));
	bool bok2 = QObject::connect(d->dropList, SIGNAL(itemClicked(int,Qt::MouseButton)), this, SLOT(on_dropList_itemClicked(int,Qt::MouseButton)));
	QObject::connect(d->dropListView, SIGNAL(visibleChanged()), this, SLOT(stateChanged()));
	QObject::connect(d->dropListView, SIGNAL(aboutToHide(KFollowWindow::HideFlag,bool*)), this, SLOT(on_aboutToHide(KFollowWindow::HideFlag,bool*)));
	QObject::connect(d->dropButton, SIGNAL(clicked()), this, SLOT(on_dropbutton_clicked()));
}

int KComboBoxWidget::dropListMaxHeight() const
{
	Q_D(const KComboBoxWidget);
	return d->dropListMaxHeight;
}

void KComboBoxWidget::setDropListMaxHeight( const int h )
{
	Q_D(KComboBoxWidget);
	d->dropListMaxHeight = h;
}

int KComboBoxWidget::dropListMinWidth() const
{
	Q_D(const KComboBoxWidget);
	return d->dropListMinWidth;
}

void KComboBoxWidget::setDropListMinWidth( const int w )
{
	Q_D(KComboBoxWidget);
	d->dropListMinWidth = w;
}

int KComboBoxWidget::selectIndex() const
{
	return d_func()->dropList->selectIndex();
}

void KComboBoxWidget::setSelectIndex( int index )
{
	if(index < 0)
		return;
	Q_D(KComboBoxWidget);
	d->dropList->setSelectIndex(index);
	updateContent(d->content, index);
	emit indexChanged(index);
}

QObject * KComboBoxWidget::dropList() const
{
	Q_D(const KComboBoxWidget);

	return d->dropList;
}

QObject * KComboBoxWidget::dropListView() const
{
	Q_D(const KComboBoxWidget);

	return d->dropListView;

}

void KComboBoxWidget::stateChanged()
{
	Q_D(KComboBoxWidget);
	QObject *objSender = sender();

	if(objSender == d->dropListView)
	{
		d->dropButton->setChecked(d->dropListView->isVisible());
	}
	else if(objSender == NULL)
	{

	}
	else if(objSender == d->dropButton)
	{

	}
}

void KComboBoxWidget::_showDropList()
{
	Q_D(KComboBoxWidget);
	if(d->dropList->count() <= 0)
		return;
	int h = d->dropList->itemsHeight();
	QMargins m(FRAME_MARGINS);
	h += m.top() + m.bottom();
	if(h <= 0)
	{
		h = d->dropListMaxHeight;
	}

	if(h > d->dropListMaxHeight)
	{
		h = d->dropListMaxHeight;
	}
	int willWidth = d->dropListMinWidth;
	if(willWidth < width())
	{
		willWidth = width();
	}
	d->dropListView->setWindowSize(willWidth, h);
	d->dropListView->adjustGeometry();
	d->dropListView->show();
}

void KComboBoxWidget::_hideDropList()
{
	Q_D(KComboBoxWidget);
	d->dropListView->hide();
}

void KComboBoxWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(KComboBoxWidget);
	d->frame->setGeometry(rect());
}

QObject * KComboBoxWidget::dropButton() const
{
	Q_D(const KComboBoxWidget);
	return d->dropButton;
}


QObject * KComboBoxWidget::dropIcon() const
{
	Q_D(const KComboBoxWidget);
	return d->buttonIcon;
}

bool KComboBoxWidget::eventFilter( QObject *watched, QEvent *e )
{
	QEvent::Type type = e->type();
	if(e->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *me = (QMouseEvent*)e;
		QRect winRt = KUtility::mapWidgetToScreen(this);
		QPoint pt = me->globalPos();
		if(!winRt.contains(pt))
		{
			Q_D(KComboBoxWidget);
			d->dropListView->hide();
		}
	}
	return false;
}

void KComboBoxWidget::on_dropbutton_clicked()
{
	Q_D(KComboBoxWidget);

	if(d->dropListView->isVisible())
	{
		hideDropList();
	}
	else
	{
		showDropList();
	}
}

void KComboBoxWidget::showDropList()
{
	_showDropList();
}

void KComboBoxWidget::hideDropList()
{
	_hideDropList();
}

void KComboBoxWidget::cleanupContent()
{
	Q_D(KComboBoxWidget);
	deleteContent(d->content);
}

void KComboBoxWidget::setupContent()
{
	Q_D(KComboBoxWidget);
	createContent(d->content);
}

QMargins KComboBoxWidget::contentMargins() const
{
	Q_D(const KComboBoxWidget);
	return d->content->margins();
}

void KComboBoxWidget::setContentMargins( const QMargins& m )
{
	Q_D(KComboBoxWidget);
	d->content->setMargins(m);
}

void KComboBoxWidget::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	Q_D(KComboBoxWidget);
	if(d->frame)
	{
		d->frame->setHover();
	}
}

void KComboBoxWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_D(KComboBoxWidget);
	if(d->frame)
	{
		d->frame->setNormal();
	}
}

QString KComboBoxWidget::frameImage() const
{
	Q_D(const KComboBoxWidget);
	return d->frame->imagePath();
}

void KComboBoxWidget::setFrameImage( const QString& file )
{
	Q_D(KComboBoxWidget);
	d->frame->setImagePath(file);
}

QString KComboBoxWidget::buttonIconPath() const
{
	Q_D(const KComboBoxWidget);
	return d->buttonIcon->iconPath();
}

void KComboBoxWidget::setButtonIconPath( const QString& file )
{
	Q_D(KComboBoxWidget);
	d->buttonIcon->setIconPath(file);
}

QString KComboBoxWidget::buttonImage() const
{
	Q_D(const KComboBoxWidget);
	return d->dropButton->imagePath();
}

void KComboBoxWidget::setButtonImage( const QString& file )
{
	Q_D(KComboBoxWidget);
	d->dropButton->setImagePath(file);
}

int KComboBoxWidget::buttonWidth()
{
	Q_D(KComboBoxWidget);
	return d->dropButton->width();
}

void KComboBoxWidget::setButtonWidth( int w )
{
	Q_D(KComboBoxWidget);
	d->dropButton->setFixWidth(w);
}

QMargins KComboBoxWidget::itemMargins() const
{
	Q_D(const KComboBoxWidget);
	return d->dropList->itemMargins();
}

void KComboBoxWidget::setItemMargins( const QMargins& m )
{
	Q_D(KComboBoxWidget);
	d->dropList->setItemMargins(m);
}

void KComboBoxWidget::clearSelection()
{
	Q_D(KComboBoxWidget);
	clearContent(d->content);
	d->dropList->clearSelected();
}

void KComboBoxWidget::createContent( KWidget* contentRoot )
{

}

void KComboBoxWidget::updateContent( KWidget* contentRoot, int index )
{

}

void KComboBoxWidget::deleteContent( KWidget* contentRoot )
{

}

void KComboBoxWidget::clearContent( KWidget* contentRoot )
{

}

bool KComboBoxWidget::dropListShowUp()
{
	Q_D(KComboBoxWidget);
	KFollowWindow::Direction direction = d->dropListView->direction();
	return direction == KFollowWindow::TopLeft;
}

void KComboBoxWidget::setDropListShowUp( bool on )
{
	Q_D(KComboBoxWidget);
	d->dropListView->setDirection(on ?  KFollowWindow::TopLeft : KFollowWindow::BottomLeft);
}

void KComboBoxWidget::on_aboutToHide( KFollowWindow::HideFlag flag, bool *pbAborted )
{
	Q_D(KComboBoxWidget);
	if(flag == KFollowWindow::TargetWindowInactive)
	{
		if(d->dropList->isUnderMouse())
		{
			*pbAborted = true;
		}
	}
	else if(flag == KFollowWindow::MySelfWindowActiveToInactive)
	{

	}
}

void KComboBoxWidget::addItem( const QString& text )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->addItem(text);
}

void KComboBoxWidget::addItem( const QString& text, const QVariant& other )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->addItem(text, other);
}

void KComboBoxWidget::addItem( const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->addItem(icon, text, other);
}

void KComboBoxWidget::insertItem( int index, const QString& icon, const QString& text, const QVariant& other )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->insertItem(index, icon, text, other);
}

void KComboBoxWidget::removeItem( const QString& text )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->removeItem(text);
}

void KComboBoxWidget::removeItem( int index )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->removeItem(index);
}

void KComboBoxWidget::removeAll()
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->removeAll();
}

int KComboBoxWidget::listCount() const
{
	Q_D(const KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return -1;
	return pList->count();
}

void KComboBoxWidget::on_dropList_indexChanged( int i )
{
	setSelectIndex(i);
	_hideDropList();
}

QString KComboBoxWidget::itemsText() const
{
	Q_D(const KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return QString("");
	return pList->itemsText();
}

void KComboBoxWidget::setItemsText( const QString& txt )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return;
	pList->setItemsText(txt);
}

int KComboBoxWidget::indexOfText( const QString& text )
{
	Q_D(KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return -1;
	return pList->indexOfText(text);
}

QString KComboBoxWidget::itemText( int i ) const
{
	Q_D(const KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return "";

	QMap<Qt::ItemDataRole, QVariant> item = pList->itemData(i);
	return item.value(Qt::DisplayRole).toString();
}

QString KComboBoxWidget::itemIcon( int i ) const
{
	Q_D(const KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return "";

	QMap<Qt::ItemDataRole, QVariant> item = pList->itemData(i);
	return item.value(Qt::DecorationRole).toString();
}

QVariant KComboBoxWidget::itemOther( int i ) const
{
	Q_D(const KComboBoxWidget);
	KCommonListWidget *pList = d->dropList;
	if(pList == NULL)
		return "";

	QMap<Qt::ItemDataRole, QVariant> item = pList->itemData(i);
	return item.value((Qt::ItemDataRole)KAbstractListModel::UserRoleOther);
}

void KComboBoxWidget::on_dropList_itemClicked( int i,Qt::MouseButton mb )
{
	on_dropList_indexChanged(i);
}
