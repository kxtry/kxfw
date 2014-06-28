#include "stdafx.h"
#include "kmenu.h"
#include "kutility.h"
#include "ktheme.h"
#include "kaction.h"
#include "kwidget.h"

#define ICON_MARGIN	(0)
#define TAB_SPACING (20)
#define ARROW_SPACING (4)

struct MenuStyleDataEx : public MenuStyleData
{
	QRegion rgnLeftTop;
	QRegion rgnRightTop;
	QRegion rgnLeftBot;
	QRegion rgnRightBot;

	MenuStyleDataEx()
	{
	}

	MenuStyleDataEx(const MenuStyleData& d)
	{
		frame = d.frame;
		maskMargin = d.maskMargin;
		menuVMargin = d.menuVMargin;
		menuHMargin = d.menuHMargin;
		mask = d.mask;
		background = d.background;
		itemMinimumHeight = d.itemMinimumHeight;
		menuMinimumWidth = d.menuMinimumWidth;
		check = d.check;
		checkDisable = d.checkDisable;
		iconMinimumSize = d.iconMinimumSize;
		overlapSize = d.overlapSize;
		hovered = d.hovered;
		seperator = d.seperator;
		leftFrameThinkness = d.leftFrameThinkness;
		rightFrameThinkness = d.rightFrameThinkness;
		arrow = d.arrow;
		textColor = d.textColor;
		textColorDisable = d.textColorDisable;
		
		if(mask.isNull())
			return;

		QRegion maskRgn(mask.mask());
		int width = mask.width();
		int height = mask.height();
		rgnLeftTop = maskRgn.intersected(QRect(0, 0, width / 2, height / 2));
		rgnRightTop = maskRgn.intersected(QRect(width / 2, 0, width - width / 2, height / 2));
		rgnLeftBot = maskRgn.intersected(QRect(0, height / 2, width / 2, height - height / 2));
		rgnRightBot = maskRgn.intersected(QRect(width / 2, height / 2, width - width / 2, height - height / 2));
	}
};

class KMenuPrivate
{
public:
	KMenu *q;
	QRegion winRegion;
	int iconWidth;
	int textWidth;
	int arrowWidth;
	bool hasShortcut;
	QMap<QString, QPointer<KAction>> customActions; 

	QColor backgroundColor;
	QColor textColor; //正常情况下字体颜色
	QColor textColorDisable; //禁用情况下字体颜色。

	QPointer<KWidget> target;
	KMenu::Direction direction;
	QPoint offsetPt;
	bool textAlignCenter;
public:
	KMenuPrivate()
		: iconWidth(0)
		, textWidth(0)
		, arrowWidth(0)
		, hasShortcut(false)
		, textAlignCenter(false)
	{
	}

	~KMenuPrivate()
	{

	}

	QRegion createWindowRegion(const MenuStyleDataEx& style, const QRect& winRt )
	{
		QRegion winRegion;
		if(!style.mask.isNull())
		{
			int width = style.mask.width();
			int height = style.mask.height();
			QRect r = winRt.adjusted(width / 2, height / 2, -width + width / 2, -height + height / 2);

			winRegion = QRegion(r);
			QRect contentRt = winRt.adjusted(style.maskMargin.left(), style.maskMargin.top(), -style.maskMargin.right(), -style.maskMargin.bottom());
			QRect leftRt(0, r.top(), r.left(), r.height());
			leftRt = leftRt.intersect(contentRt);
			winRegion += leftRt;

			QRect topRt(r.left(), 0, r.width(), r.top());
			topRt = topRt.intersect(contentRt);
			winRegion += topRt;

			QRect rightRt(r.right(), r.top(), winRt.width() - r.right(), r.height());
			rightRt = rightRt.intersect(contentRt);
			winRegion += rightRt;

			QRect botRt(r.left(), r.bottom(), r.width(), winRt.height() - r.bottom());
			botRt = botRt.intersect(contentRt);
			winRegion += botRt;

			winRegion += style.rgnLeftTop;

			winRegion.translate(-r.width(), 0);
			winRegion += style.rgnRightTop;
			winRegion.translate(r.width(), 0);

			winRegion.translate(0, -r.height());
			winRegion += style.rgnLeftBot;
			winRegion.translate(0, r.height());

			winRegion.translate(-r.width(), -r.height());
			winRegion += style.rgnRightBot;
			winRegion.translate(r.width(), r.height());
		}
		else
		{
			winRegion = winRt;
		}
		return winRegion;
	}

	void drawPrimitivePanelMenu(QPainter *p, const QStyleOption *opt, const MenuStyleDataEx& style)
	{
		QRect winRt = q->rect();
		winRegion = createWindowRegion(style, winRt);
		QPainterPath path;
		path.addRegion(winRegion);
		if(backgroundColor.isValid())
		{
			p->fillPath(path, backgroundColor);
		}
		else
		{
			p->fillPath(path, style.background);
		}
	}

	void drawPrimitiveFrameMenu(QPainter *p, const QStyleOption *opt, const MenuStyleDataEx& style)
	{
		QRectF winRt = q->rect();
		p->save();
		p->setClipRect(winRt.toRect(), Qt::ReplaceClip);
		KUtility::drawGridFrameFour(p, winRt, style.frame);
		p->restore();
	}

	void drawControl(QPainter *p, QStyle::ControlElement element, const QStyleOption *opt, const MenuStyleDataEx& style)
	{
		if(element == QStyle::CE_MenuItem)
		{
			//菜单项的绘制。
			if (const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>(opt))
			{
				QRect itemRt = mi->rect;
				QRect winRt = q->rect();
				int tab = mi->tabWidth;
				bool enabled = mi->state & QStyle::State_Enabled;
				bool checked = mi->checkType != QStyleOptionMenuItem::NotCheckable ? mi->checked : false;
				bool selected = mi->state & QStyle::State_Selected;

				/*计算出实际可绘制区域。*/
				winRt.adjust(style.maskMargin.left(), style.maskMargin.top(), -style.maskMargin.right(), -style.maskMargin.bottom());
				winRt.adjust(style.leftFrameThinkness, 0, -style.rightFrameThinkness, 0);

				//真实可绘制的内容区域
				QRectF drawRt = itemRt;
				drawRt.setLeft(winRt.left());
				drawRt.setRight(winRt.right());

				KAction *pAction = NULL;
				int idxTab = mi->text.indexOf('\t');
				if(idxTab >= 0)
				{
					QString txt = mi->text.left(idxTab);
					pAction = customActions.value(txt);
				}
				else
				{
					pAction = customActions.value(mi->text);
				}

				if(pAction)
				{
					//tabWidth:为快捷键区域长度。
					//tabSpace:为快捷键和文字之间的区域长度。
					//textWidth:为文字长度区域
					//iconWidth:为图标显示区域长度。
					pAction->paint(p, drawRt, iconWidth, textWidth, TAB_SPACING, mi->tabWidth);
					return ;
				}

				if(mi->menuItemType == QStyleOptionMenuItem::Separator)
				{
					//分割线
					KUtility::drawStretchHMiddle(p, drawRt, style.seperator, false);
					return;
				}

				if(selected)
				{
					//绘制选中项
					KUtility::drawGridFour(p, drawRt, style.hovered);
				}

				winRt.adjust(style.menuHMargin, style.menuVMargin, -style.menuHMargin, -style.menuVMargin);
				drawRt.setLeft(winRt.left());
				drawRt.setRight(winRt.right());

				int actualWidth = iconWidth + textWidth + mi->tabWidth + arrowWidth + (mi->tabWidth > 0 ? TAB_SPACING : 0);
				int xspace = drawRt.width() - actualWidth;
				int xoffset = xspace / 2;
				if(iconWidth > 0 && (mi->tabWidth > 0 || arrowWidth > 0))
				{
					xoffset = xspace / 2;
				}
				else if(iconWidth > 0)
				{
					xoffset = xspace / 4;
				}
				//绘制图标
				int ixbegin = drawRt.left() + xoffset;
				if (!mi->icon.isNull() )
				{
					QSize icosz = mi->icon.actualSize(QSize(1024,1024), QIcon::Normal, QIcon::On);
					QRectF iconRt(0, 0, iconWidth, drawRt.height());
					iconRt.translate(drawRt.topLeft());
					QPixmap pix = mi->icon.pixmap(icosz, enabled ? QIcon::Normal : QIcon::Disabled, QIcon::On);
					KUtility::drawOneHVCenter(p, iconRt, pix);
				}
				else if(checked) 
				{
					QRectF iconRt(0, 0, iconWidth, drawRt.height());
					iconRt.translate(drawRt.topLeft());
					KUtility::drawOneHVCenter(p, iconRt, enabled ? style.check : style.checkDisable);
				}

				//绘制文字
				ixbegin += iconWidth;
				QRect txtRt(ixbegin, drawRt.top(), textWidth, drawRt.height());
				int flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
				flags |= textAlignCenter ? Qt::AlignHCenter : Qt::AlignLeft;
				p->setFont(mi->font);
				QPen pen = p->pen();
				if(textColor.isValid())
				{
					if(textColorDisable.isValid())
					{
						pen.setColor(enabled ? textColor : textColorDisable);
					}
					else
					{
						pen.setColor(enabled ? textColor : style.textColorDisable);
					}
				}
				else
				{
					if(textColorDisable.isValid())
					{
						pen.setColor(enabled ? style.textColor : textColorDisable);
					}
					else
					{
						pen.setColor(enabled ? style.textColor : style.textColorDisable);
					}
				}
				p->setPen(pen);

				if(idxTab >= 0)
				{
					QString text = mi->text.left(idxTab);
					QString shortcut = mi->text.mid(idxTab+1);

					p->drawText(txtRt, flags, text);

					ixbegin += textWidth;
					ixbegin += TAB_SPACING;
					QRect shortcutRt(ixbegin, drawRt.top(), mi->tabWidth, drawRt.height());
					p->drawText(shortcutRt, flags, shortcut);	
				}
				else
				{
					p->drawText(txtRt, flags, mi->text);
					ixbegin += textWidth;
					if(mi->tabWidth > 0)
					{
						ixbegin += TAB_SPACING;
					}
				}
				
				ixbegin += mi->tabWidth;
				if(mi->menuItemType == QStyleOptionMenuItem::SubMenu)
				{
					QRectF arrowRt(ixbegin, drawRt.top(), drawRt.right() - ixbegin, drawRt.height());
					KUtility::drawOneHVCenter(p, arrowRt, style.arrow);
				}
			}
		}
	}

	QSize sizeFromContents(QStyle::ContentsType ct, const QStyleOption *opt, const QSize &csz, const MenuStyleDataEx& style)
	{
		if(ct == QStyle::CT_MenuItem)
		{
			if(const QStyleOptionMenuItem *mi = qstyleoption_cast<const QStyleOptionMenuItem *>(opt)) 
			{
				bool enabled = mi->state & QStyle::State_Enabled;
				bool checked = mi->checkType != QStyleOptionMenuItem::NotCheckable ? mi->checked : false;
				bool selected = mi->state & QStyle::State_Selected;

				if(mi->menuItemType == QStyleOptionMenuItem::Separator) 
				{
					return QSize(10, style.seperator.height());
				}

				KAction *pAction = NULL;
				int idxTab = mi->text.indexOf('\t');
				if(idxTab >= 0)
				{
					QString txt = mi->text.left(idxTab);
					pAction = customActions.value(txt);
				}
				else
				{
					pAction = customActions.value(mi->text);
				}

				if(pAction)
				{
					QSize sz = pAction->contentSize();
					int minheight = qMax(sz.height(), style.itemMinimumHeight);
					int minwidth = sz.width() + style.menuHMargin * 2 + style.leftFrameThinkness + style.rightFrameThinkness;

					return QSize(minwidth, minheight);
				}

				int minwidth = 0;
				int minheight = 0;
				if(!mi->icon.isNull())
				{
					QSize icosz = mi->icon.actualSize(QSize(1024,1024), QIcon::Normal, QIcon::On);
					int icoh = qMax(icosz.height(), icosz.width());
					if(icoh < style.iconMinimumSize)
					{
						icoh = style.iconMinimumSize;
					}
					minheight = icoh + ICON_MARGIN * 2;
					minwidth = icoh + ICON_MARGIN * 2;
					iconWidth = qMax(iconWidth, minwidth);
				}
				else if(checked)
				{
					QSize icosz = style.check.size();
					int icoh = qMax(icosz.height(), icosz.width());
					if(icoh < style.iconMinimumSize)
					{
						icoh = style.iconMinimumSize;
					}
					minheight = icoh + ICON_MARGIN * 2;
					minwidth = icoh + ICON_MARGIN * 2;
					iconWidth = qMax(iconWidth, minwidth);
				}

				if(idxTab >= 0)
				{
					//表明它包有快捷键。
					minwidth += TAB_SPACING;
				}
				else if(mi->menuItemType == QStyleOptionMenuItem::SubMenu)
				{
					minwidth += style.arrow.width() + ARROW_SPACING;
					arrowWidth = style.arrow.width() + ARROW_SPACING;
				}

				if(idxTab >= 0)
				{
					QString text = mi->text.left(idxTab);
					QString shortcut = mi->text.mid(idxTab+1);

					QFontMetrics fm(mi->font);
					int w = fm.width(text);
					textWidth = qMax(textWidth, w);
					minwidth += w;

					hasShortcut = true;
				}
				else
				{
					QFontMetrics fm(mi->font);
					int w = fm.width(mi->text);
					textWidth = qMax(textWidth, w);
					minwidth += w;
				}

				minheight = qMax(minheight, style.itemMinimumHeight);
				//增加边距，确保可自由绘制边距，而不会出错。
				minwidth += style.menuHMargin * 2 + style.leftFrameThinkness + style.rightFrameThinkness;
				int actualMinWidth = iconWidth + textWidth + arrowWidth + style.menuHMargin * 2 + style.leftFrameThinkness + style.rightFrameThinkness;
				if(hasShortcut)
				{
					actualMinWidth += TAB_SPACING;
				}
				actualMinWidth = qMax(actualMinWidth, style.menuMinimumWidth);
				return QSize(actualMinWidth, minheight);
			}
		}
		return csz;
	}
};



class KMenuStyleHandler : public QWindowsStyle
{
public:
	MenuStyleDataEx style;
public:
	void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w = 0) const
	{
		const KMenu * menu = qobject_cast<const KMenu*>(w);
		KMenuPrivate *prv = menu->d;

		if (pe == PE_PanelMenu)
		{
			prv->drawPrimitivePanelMenu(p, opt, style);
			return;
		}		
		
		if(pe == PE_FrameMenu)
		{
			prv->drawPrimitiveFrameMenu(p, opt, style);
			return;
		}
	}

	void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w = 0) const
	{
		const KMenu * menu = qobject_cast<const KMenu*>(w);
		KMenuPrivate *prv = menu->d;

		if(element == CE_MenuEmptyArea)
		{
			//不填充空白区。
			return;
		}
		if(element == CE_MenuTearoff)
		{
			//菜单变成ToolMenu的点击区。
			return;
		}
		if(element == CE_MenuScroller)
		{
			//菜单超多项时，滚动按钮。
			return;
		}

		prv->drawControl(p, element, opt, style);
	}

	int pixelMetric(PixelMetric pm, const QStyleOption *option = 0, const QWidget *w = 0) const
	{
		if(pm == PM_MenuDesktopFrameWidth || pm == PM_MenuTearoffHeight || pm == PM_MenuScrollerHeight)
		{
			return 0;
		}
		if(pm == PM_MenuHMargin)
		{
			/*由于此处直接设置的边距限制导致在绘制时，可能会出故障，而故障真正原因未查到。*/
			return 0;//(style.contentMargin.left() + style.contentMargin.right()) / 2 + style.frameThinkness;
		}
		if(pm == PM_MenuVMargin)
		{
			return style.menuVMargin;
		}
		if(pm == PM_MenuPanelWidth)
		{
			//至少返回1，目的是为了能够调用PE_FrameMenu进行绘制边框。
			return 1;
		}
		if(pm == PM_SubMenuOverlap)
		{
			//子菜单的重叠区域。
			return -style.overlapSize;
		}
		if(pm == PM_SmallIconSize)
		{
			return 0;
		}
		return 0;
	}

	QSize sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &csz, const QWidget *w = 0) const
	{
		const KMenu * menu = qobject_cast<const KMenu*>(w);
		KMenuPrivate *prv = menu->d;

		return prv->sizeFromContents(ct, opt, csz, style);
	}
};

Q_GLOBAL_STATIC(KMenuStyleHandler, getMenuStyleHandler)

struct MenuStyleHandlerWrapper 
{
	QPointer<KMenuStyleHandler> style;

	MenuStyleHandlerWrapper()
	{

	}

	MenuStyleHandlerWrapper(const MenuStyleDataEx& d)
	{
		if(style)
		{
			delete style;
		}
		style = new KMenuStyleHandler();
		style->style = d;
	}

	~MenuStyleHandlerWrapper()
	{
		if(style)
		{
			delete style;
		}
	}
};


typedef QMap<int, MenuStyleHandlerWrapper> MenuStyleHandlers;

Q_GLOBAL_STATIC(MenuStyleHandlers, getMenuStyleHandlers)

Q_GLOBAL_STATIC(KMenuStyleHandler, _getDefaultMenuStyleHandler)

KMenuStyleHandler *getDefaultMenuStyleHandler()
{
	static bool binit = false;
	KMenuStyleHandler *pstyle = _getDefaultMenuStyleHandler();
	if(!binit)
	{
		MenuStyleData style;
		style.background = QColor(Qt::white);
		style.check = KTheme::getPixmap(":/image/theme/menucheck.png");
		style.checkDisable = KTheme::getPixmap(":/image/theme/menucheckdisable.png");
		style.iconMinimumSize = 16;
		style.itemMinimumHeight = 25;
		style.menuHMargin = 8;
		style.menuVMargin = 8;
		style.leftFrameThinkness = 2;
		style.rightFrameThinkness = 1;
		style.mask = KTheme::getPixmap(":/image/theme/maskimage.png");
		style.maskMargin = QMargins(8,8,8,8);
		style.frame = KTheme::getPixmap(":/image/theme/frameimage.png");
		style.hovered = KTheme::getPixmap(":/image/theme/menuhover.png");
		style.seperator = KTheme::getPixmap(":/image/theme/menusperator.png");
		style.arrow = KTheme::getPixmap(":/image/theme/menuarrow.png");
		style.overlapSize = 10;
		style.textColor = QColor(Qt::black);
		style.textColorDisable = QColor(Qt::lightGray);
		pstyle->style = style;
		binit = true;
	}
	return pstyle;
}

KMenu::KMenu( QWidget *parent /*= NULL*/ )
: QMenu(parent)
, d(new KMenuPrivate())
{
	d->q = this;

	Qt::WindowFlags flags = windowFlags();
	setWindowFlags(flags | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);
	quint32 exstyle = GetWindowLong(winId(), GWL_EXSTYLE);
	SetWindowLong(winId(), GWL_EXSTYLE, exstyle | WS_EX_LAYERED);

	DWORD styles = GetClassLong(HWND(winId()), GCL_STYLE);
	styles &= ~CS_DROPSHADOW;
	SetClassLong((HWND)(winId()), GCL_STYLE, styles);
}

KMenu::~KMenu()
{
	delete d;
}

KMenu * KMenu::createPopupMenu( int stylekey /*= 0*/ )
{
	KMenu *menu = new KMenu();

	MenuStyleHandlers *pstyles = getMenuStyleHandlers();
	if(pstyles->contains(stylekey))
	{
		MenuStyleHandlerWrapper wrapper = pstyles->value(stylekey);
		menu->setStyle(wrapper.style);
		menu->setContentsMargins(wrapper.style->style.maskMargin);
	}
	else
	{
		KMenuStyleHandler *pstyle = getDefaultMenuStyleHandler();
		menu->setStyle(pstyle);
		menu->setContentsMargins(pstyle->style.maskMargin);
	}

	QObject::connect(menu, SIGNAL(aboutToShow()),  menu, SLOT(on_aboutToShow()));

	return menu;
}

QAction* KMenu::exec()
{
	QAction* pAct = QMenu::exec(QCursor::pos());
	d->direction = NotDefine;
	d->target = NULL;
	return pAct;
}

QAction* KMenu::exec( KWidget *target, Direction direction, const QPoint& offset/*=QPoint(0,0)*/ )
{
	if(direction == NotDefine)
	{
		return exec();
	}

	d->target = target;
	d->direction = direction;
	d->offsetPt = offset;
	return QMenu::exec();
}

bool KMenu::addMenuStyle( int key, MenuStyleData& style )
{
	MenuStyleHandlers *pstyles = getMenuStyleHandlers();
	if(pstyles->contains(key))
		return false;
	MenuStyleDataEx ex = style;
	pstyles->insert(key, MenuStyleHandlerWrapper(ex));
	return true;
}

void KMenu::on_aboutToShow()
{
	QList<QAction*> myacts = actions();
	for(QList<QAction*>::iterator iter = myacts.begin(); iter != myacts.end(); iter++)
	{
		QAction *act = *iter;
		KAction *mact = qobject_cast<KAction*>(act);
		if(mact)
		{
			QString txt = act->text();
			d->customActions.insert(txt, mact);
		}
	}
}

void KMenu::showEvent( QShowEvent *event )
{
	resetGeometry();
}

void KMenu::resetGeometry()
{
	if(d->target == NULL)
		return;

	bool bHandled = false;
	emit aboutToAdjust(this, &bHandled);
	if(bHandled)
	{
		return;
	}
	QRectF targetRt = d->target->rectToScreen();
	QSizeF targetSize = targetRt.size();
	QSizeF winSize = size();
	QRectF winRt(0, 0, winSize.width(), winSize.height());
	QPointF winCenterPt = winRt.center();
	QPointF targetCenterPt = targetRt.center();		

	QSizeF hitDistance = (targetSize + winSize) / 2;
	Direction tmpDirection = d->direction;

	QMargins margin = contentsMargins();

	if(tmpDirection == LeftTop)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.top());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.top());
		QPointF offsetPt = targetPt - QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(margin.right(), -margin.top());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == LeftCenter)
	{
		QPointF offsetPt = targetCenterPt - QPointF(hitDistance.width(), 0) - winCenterPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(margin.right(), 0);

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == LeftBottom)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.bottom());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.bottom());
		QPointF offsetPt = targetPt - QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(margin.right(), margin.bottom());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == RightTop)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.top());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.top());
		QPointF offsetPt = targetPt + QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(-margin.left(), -margin.top());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == RightCenter)
	{
		QPointF offsetPt = targetCenterPt + QPointF(hitDistance.width(), 0) - winCenterPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(-margin.left(), 0);

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == RightBottom)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.bottom());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.bottom());
		QPointF offsetPt = targetPt + QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(-margin.left(), margin.bottom());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == TopLeft)
	{
		QPointF targetPt = QPointF(targetRt.left(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.left(), winCenterPt.y());
		QPointF offsetPt = targetPt - QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(-margin.left(), margin.bottom());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == TopCenter)
	{
		QPointF offsetPt = targetCenterPt - QPointF(0, hitDistance.height()) - winCenterPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(0, margin.bottom());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == TopRight)
	{
		QPointF targetPt = QPointF(targetRt.right(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.right(), winCenterPt.y());
		QPointF offsetPt = targetPt - QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(margin.right(), margin.bottom());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == BottomLeft)
	{
		QPointF targetPt = QPointF(targetRt.left(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.left(), winCenterPt.y());
		QPointF offsetPt = targetPt + QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(-margin.left(), -margin.top());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
	else if(tmpDirection == BottomCenter)
	{
		QPointF winCenterPt = winRt.center();
		QPointF targetCenterPt = targetRt.center();		
		QPointF offsetPt = targetCenterPt + QPointF(0, hitDistance.height()) - winCenterPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(0, -margin.top());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}	
	else if(tmpDirection == BottomRight)
	{
		QPointF targetPt = QPointF(targetRt.right(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.right(), winCenterPt.y());
		QPointF offsetPt = targetPt + QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		QPointF marginPt(margin.right(), -margin.top());

		offsetPt += marginPt;

		move(offsetPt.toPoint());
	}
}

void KMenu::setTextAlignCenter( bool on )
{
	d->textAlignCenter = on;
}

void KMenu::setBackgroundColor( const QColor &clr )
{
	d->backgroundColor = clr;
}

void KMenu::setTextColor( const QColor& clr )
{
	d->textColor = clr;
}

void KMenu::setTextDisableColor( const QColor& clr )
{
	d->textColorDisable = clr;
}
