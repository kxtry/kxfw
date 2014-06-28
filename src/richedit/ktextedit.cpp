#include "stdafx.h"

#include "kframeitem.h"
#include "ktextedit.h"

#include "qtextbrowserex.h"
#include "ktextcursor.h"

#include "kmenu.h"
#include "kxmlui.h"

#include "ktheme.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KTextEdit)

//////////////////////////////////////////////////////////////////////////

KTextEdit::KTextEdit(QGraphicsItem *parent /* = NULL*/ )
: KWidget(parent)
, m_edit(NULL)
, m_proxyWidget(NULL)
, m_bHtmlUrlForWeb(true)
, m_dragPolicyMode(DragAtNoContent)
, m_bAutoUniqueTextFont(false)
, m_bHasContextMenu(true)
, m_posContentChanged(0)
, m_charsRemovedContentChanged(0)
, m_charsAddedContentChanged(0)
, m_bDoUniqueTextNow(false)
{
	setFlag(QGraphicsItem::ItemClipsToShape);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape);

	init();

	if ( parent )
	{
		m_edit->setVisible( parent->isVisible() );
	}
}

KTextEdit::~KTextEdit(void)
{
	//对于动态删除layout 如果不delete 还会残留在界面上
	if ( m_edit )
	{
		delete m_edit;
	}
}

void KTextEdit::init()
{
	m_edit = new QTextBrowserEx();

	m_proxyWidget = new QGraphicsProxyWidget(this);
	m_proxyWidget->setWidget( m_edit );
	m_proxyWidget->setFocusPolicy(Qt::StrongFocus);

	m_edit->setStyleSheet("QTextEdit { border-style: none; background: transparent}");

	m_edit->setAcceptDrops(true);
	m_edit->setFrameShape(QFrame::NoFrame);
	m_edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_edit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_edit->setUndoRedoEnabled(true);

	QObject::connect(this, SIGNAL(visibleChanged()), this, SLOT(on_visibleChanged()));

	setBackgroundColor(QColor(255,255,255,128));
	setDragPolicy( KWidget::Nothing );

	QObject::connect(m_edit, SIGNAL(insertMimiData(QTextCursor&,QString,QTextCharFormat,bool&)), this, SIGNAL(insertMimiData(QTextCursor&,QString,QTextCharFormat,bool&)));

	QObject::connect(m_edit, SIGNAL(clickedLink(const QString&, const QString&)), this, SIGNAL(linkClicked(const QString&, const QString&)));

	QObject::connect(m_edit, SIGNAL(htmlLinkClicked(const QString&)), this, SLOT(on_htmlLinkClicked(const QString&)));

	QObject::connect(m_edit, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));

	QObject::connect(m_edit, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)), this, SIGNAL(currentCharFormatChanged(const QTextCharFormat&)));

	QObject::connect(m_edit, SIGNAL(contextMenu(QContextMenuEvent*)), this, SLOT(on_contextMenu(QContextMenuEvent*)));

	QObject::connect(m_edit, SIGNAL(textChanged()),SIGNAL(textChanged()));

	QObject::connect(m_edit, SIGNAL(mouseEvent(QEvent*,bool*)), this, SLOT(on_mouseEvent(QEvent*,bool*)));

	QObject::connect(m_edit->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(on_contentsChange(int,int,int)));
	QObject::connect(m_edit->document(), SIGNAL(contentsChanged()), this, SLOT(on_contentsChanged()));

	setVerticalScrollBarStyleColor(KTheme::themeColor());
	setHorizontalScrollBarStyleColor(KTheme::themeColor());

	QTextDocument *doc = m_edit->document();
	doc->setProperty("edit", QVariant::fromValue<QObject*>(m_edit));
}

void KTextEdit::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	resetGeometry(QRectF(QPointF(0, 0), event->newSize()));
}

QString KTextEdit::toPlainText() const
{
	return m_edit->toPlainText();
}

QString KTextEdit::toHtml() const
{
	return m_edit->toHtml();
}

void KTextEdit::insertText( const QString& text )
{
	m_edit->insertPlainText( text );
}

void KTextEdit::insertHtml( const QString & text )
{
	m_edit->insertHtml( text );
}

bool KTextEdit::hasFocus() const
{
	return m_edit->hasFocus();
}

void KTextEdit::setFocus()
{
	m_edit->setFocus();
}


KTextCursor KTextEdit::textCursor() const 
{
	return m_edit->textCursor();
}

bool KTextEdit::isSelectable()
{
	Qt::TextInteractionFlags flags = m_edit->textInteractionFlags();
	return (flags & (Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard));
}

void KTextEdit::setSelectable( bool bSelected )
{
	Qt::TextInteractionFlags flags = m_edit->textInteractionFlags();
	if(bSelected)
	{
		flags |= Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard;
	}
	else
	{
		flags &=~(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
	}
	m_edit->setTextInteractionFlags(flags);
}

bool KTextEdit::isReadOnly() const
{
	return m_edit->isReadOnly();
}

void KTextEdit::setReadOnly( bool readOnly )
{
	m_edit->setReadOnly( readOnly );

	/*
	因为中文输入法是在QGraphicsItem::setFocus时，去处理是否相应增加中文输入的配置项。
	如果proxyWidget已经是提前处理setFocus后再设置为可编辑，则不会再处理当前中文输入法的请求。
	*/
	bool bime = m_edit->testAttribute(Qt::WA_InputMethodEnabled);
	m_proxyWidget->setFlag(QGraphicsItem::ItemAcceptsInputMethod, !readOnly);
}

QFontMetrics KTextEdit::fontMetrics() const
{
	return m_edit->fontMetrics();
}

void KTextEdit::setFrameShape( QFrame::Shape frameShape )
{
	m_edit->setFrameShape(frameShape);
}


Qt::ScrollBarPolicy KTextEdit::verticalScrollBarPolicy()
{
	return m_edit->verticalScrollBarPolicy();
}

Qt::ScrollBarPolicy KTextEdit::horizontalScrollBarPolicy()
{
	return m_edit->horizontalScrollBarPolicy();
}

void KTextEdit::setHorizontalScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	m_edit->setHorizontalScrollBarPolicy(policy);
}

void KTextEdit::setVerticalScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	m_edit->setVerticalScrollBarPolicy(policy);
}

void KTextEdit::setStyleSheet( const QString &styleSheet )
{
	m_edit->setStyleSheet(styleSheet);
}

void KTextEdit::setWordWrapMode( QTextOption::WrapMode policy )
{
	m_edit->setWordWrapMode(policy);
}

void KTextEdit::scrollToEnd()
{
	m_edit->scrollToEnd();
}

void KTextEdit::scrollToTop()
{
	m_edit->scrollToTop();
}

void KTextEdit::scrollToLeft()
{
	QScrollBar* horizontalScroll = m_edit->horizontalScrollBar();
	horizontalScroll->setSliderPosition(horizontalScroll->minimum());
}

int KTextEdit::verticalSliderPosition()
{
	QScrollBar* verticalScroll = m_edit->verticalScrollBar();
	return verticalScroll->sliderPosition();
}

int KTextEdit::horizontalSliderPostion()
{
	QScrollBar* horizontalScroll = m_edit->horizontalScrollBar();
	return horizontalScroll->sliderPosition();
}

void KTextEdit::setVerticalSliderPosition(int val)
{
	QScrollBar* verticalScroll = m_edit->verticalScrollBar();
	verticalScroll->setSliderPosition(val);
}

void KTextEdit::setHorizontalSliderPostion(int val)
{
	QScrollBar* horizontalScroll = m_edit->horizontalScrollBar();
	horizontalScroll->setSliderPosition(val);
}

QTextDocument* KTextEdit::document()
{
	return m_edit->document();
}

void KTextEdit::resetGeometry( const QRectF &r )
{
	if(m_edit)
	{
        QRectF tmpRt = r;
        tmpRt.moveTo(QPointF(0, 0));
        m_proxyWidget->setGeometry(tmpRt);
        m_edit->setGeometry(tmpRt.toRect());
	}
}

void KTextEdit::makeCtrlEnterAsEnter( bool makeCtrlEnterAsEnter )
{
	m_edit->makeCtrlEnterAsEnter( makeCtrlEnterAsEnter );
}

QString KTextEdit::toText() const
{
	return m_edit->toPlainText();
}

void KTextEdit::setText(const QString& txt)
{
	m_edit->setText(txt);
}

KTextCursor KTextEdit::cursorForPosition( const QPoint& pos )
{
	return m_edit->cursorFromPoint(pos);
}

bool KTextEdit::isLinkObject(const QPoint& pos)
{
	return m_edit->isLinkObject(pos);
}

void KTextEdit::selectAll()
{
	m_edit->selectAll();
}

QScrollBar* KTextEdit::verticalScrollBar() const
{
	return m_edit->verticalScrollBar();
}

QScrollBar* KTextEdit::horizontalScrollBar() const
{
	return m_edit->horizontalScrollBar();
}

void KTextEdit::setOverwriteMode( bool isOverWrite )
{
	m_edit->setOverwriteMode( isOverWrite );
}

bool KTextEdit::overwriteMode() const
{
	return m_edit->overwriteMode();
}

void KTextEdit::copy()
{
	m_edit->copy();
}

void KTextEdit::cut()
{
	m_edit->cut();
}

void KTextEdit::paste()
{
	m_edit->paste();
}

void KTextEdit::clear()
{
	m_edit->clear();
}

void KTextEdit::undo()
{
	m_edit->undo();
}

void KTextEdit::redo()
{
	m_edit->redo();
}

void KTextEdit::deleteText()
{
	QTextCursor cursor = m_edit->textCursor();
	cursor.deleteChar();
}

void KTextEdit::setMaxLength( int maxLength )
{
	m_edit->setMaxLength(maxLength);
}

void KTextEdit::setAcceptDrops( bool accept )
{
	m_edit->setAcceptDrops(accept);
}

void KTextEdit::setAcceptEnter( bool accept )
{
	m_edit->setAcceptEnter(accept);
}

void KTextEdit::on_contextMenu( QContextMenuEvent* event )
{
	if(!m_bHasContextMenu)
		return;
	KMenu *menu = KMenu::createPopupMenu();
	bool bContinue = true;
	emit contextMenu(menu, &bContinue);
	if(bContinue)
	{
		{
			QAction *action = menu->addAction("选择全部", this, SLOT(on_selectAll_triggered(bool)), QKeySequence::SelectAll);
		}

		{
			QAction *action = menu->addAction("粘贴", this, SLOT(on_pasteAction_triggered(bool)), QKeySequence::Paste);
			bool bEnable = false;
			QClipboard *clipboard = QApplication::clipboard();
			if(clipboard )
			{
				const QMimeData *mimedata = clipboard->mimeData();
				if (mimedata->hasHtml() || mimedata->hasImage() || mimedata->hasText() || mimedata->hasUrls() || mimedata->hasFormat(KTextEditMime))
				{
					bEnable = true;
				}
			}
			action->setEnabled(bEnable && !isReadOnly());
		}

		QTextCursor cursor = textCursor();
		bool hasText = !cursor.selection().isEmpty();
		{
			QAction *action = menu->addAction("剪切", this, SLOT(on_cutAction_triggered(bool)), QKeySequence::Cut);
			action->setEnabled(hasText && !isReadOnly());
		}

		{
			QAction *action = menu->addAction("复制", this, SLOT(on_copyAction_triggered(bool)), QKeySequence::Copy);
			action->setEnabled(hasText);
		}
	}
	if(menu->isEmpty())
	{
		menu->deleteLater();
	}
	else
	{
		menu->exec();
	}
}

void KTextEdit::on_cutAction_triggered( bool )
{
	cut();
}

void KTextEdit::on_copyAction_triggered( bool )
{
	copy();
}

void KTextEdit::on_pasteAction_triggered( bool )
{
	paste();
}

void KTextEdit::on_selectAll_triggered( bool )
{
	selectAll();
}

void KTextEdit::setUndoRedoEnabled( bool enable )
{
	m_edit->setUndoRedoEnabled(enable);
}

QWidget* KTextEdit::realWidget() const
{
	return m_edit;
}

QPointF KTextEdit::mapToContents(const QPointF &point) const
{ 
	return m_edit->mapToContents(point.toPoint());
}

int KTextEdit::horizontalOffset() const
{ 
	return m_edit->horizontalOffset();
}

int KTextEdit::verticalOffset() const
{ 
	return m_edit->verticalOffset();
}

QFont KTextEdit::currentFont() const
{
	return m_edit->currentFont();
}

void KTextEdit::setCurrentFont( const QFont & f )
{
	m_edit->setCurrentFont( f );
}

void KTextEdit::on_mouseEvent( QEvent *e, bool *pbHandled )
{
	QEvent::Type t = e->type();
	if(t == QEvent::MouseMove)
	{
		QMouseEvent *me = (QMouseEvent*)e;
		bool isLink = isLinkObject(me->pos());
		QCursor cursor(isLink ? Qt::PointingHandCursor : Qt::ArrowCursor);
		m_proxyWidget->setCursor(cursor);
	}
}

int KTextEdit::exactPositionFromPoint( const QPoint& point )
{
	return m_edit->exactPositionFromPoint(point);
}

void KTextEdit::setTextCursor( const QTextCursor &cursor )
{
	return m_edit->setTextCursor(cursor);
}

void KTextEdit::setTabChangesFocus( bool enable )
{
	return m_edit->setTabChangesFocus(enable);
}

QPainterPath KTextEdit::shape() const
{
	QPainterPath path;
	//path.addRoundedRect(boundingRect(), 5, 5);
	path.addRect(boundingRect());
	return path;
}

bool KTextEdit::hasScrollbar()
{
	Qt::ScrollBarPolicy hsbp = m_edit->horizontalScrollBarPolicy();
	Qt::ScrollBarPolicy vsbp = m_edit->verticalScrollBarPolicy();
	return ((hsbp & Qt::ScrollBarAlwaysOn) || (vsbp & Qt::ScrollBarAlwaysOn));
}

void KTextEdit::setScrollbarVisible( bool bVisible )
{
	setVerticalScrollBarPolicy(bVisible ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(bVisible ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
}

bool KTextEdit::isModified()
{
	return document()->isModified();
}

void KTextEdit::setModified(bool m)
{
	document()->setModified(m);
}

void KTextEdit::setFontBold(bool on)
{
	m_edit->setFontWeight(on ? QFont::Bold : QFont::Normal);
}

void KTextEdit::setFontItalic(bool on)
{
	m_edit->setFontItalic(on);
}

void KTextEdit::setFontUnderline(bool on)
{
	m_edit->setFontUnderline(on);
}

QString KTextEdit::fontFamily() const
{
	return m_edit->fontFamily();
}

void KTextEdit::setFontFamily(const QString& family)
{
	m_edit->setFontFamily(family);
}

void KTextEdit::setTextColor( const QColor &color )
{
	m_edit->setTextColor( color );
}

void KTextEdit::setFontPointSize(int s)
{
	m_edit->setFontPointSize(s);
}

bool KTextEdit::fontStrikeOut()
{
	return textCursor().charFormat().fontStrikeOut();
}

void KTextEdit::setFontStrikeOut(bool on)
{
	QTextCharFormat fmt;
	fmt.setFontStrikeOut(on);
	m_edit->mergeCurrentCharFormat(fmt);
}

void KTextEdit::setTextBackgroundColor( const QColor& clr )
{
	m_edit->setTextBackgroundColor(clr);
}

void KTextEdit::setTextAlignment( Qt::Alignment a )
{
	m_edit->setAlignment(a);
}

bool KTextEdit::fontBold()
{
	return m_edit->fontWeight() == QFont::Bold;
}

bool KTextEdit::fontItalic()
{
	return m_edit->fontItalic();
}

bool KTextEdit::fontUnderline()
{
	return m_edit->fontUnderline();
}

QColor KTextEdit::textColor() const
{
	return m_edit->textColor();
}

int KTextEdit::fontPointSize()
{
	return m_edit->fontPointSize();
}

QColor KTextEdit::textBackgroundColor() const
{
	return m_edit->textBackgroundColor();
}

Qt::Alignment KTextEdit::textAlignment()
{
	return m_edit->alignment();
}

bool KTextEdit::findText( const QString& keyFind, bool bCaseSensitive, bool bEntireWord, bool bForward )
{
	QTextDocument::FindFlags flags = 0;
	if(bCaseSensitive)
	{
		flags |= QTextDocument::FindCaseSensitively;
	}
	if(bEntireWord)
	{
		flags |= QTextDocument::FindWholeWords;
	}
	if(!bForward)
	{
		flags |= QTextDocument::FindBackward;
	}
	return m_edit->find(keyFind, flags);
}

bool KTextEdit::replaceText( const QString& keyFind, const QString& keyReplace, bool bCaseSensitive, bool bEntireWord, bool bForward )
{
	if(findText(keyFind, bCaseSensitive, bEntireWord, bForward))
	{
		textCursor().insertText(keyReplace);
		return true;
	}
	return false;
}

void KTextEdit::on_visibleChanged()
{
	bool bVis = isVisible();
	m_proxyWidget->setVisible(bVis);
	m_edit->setVisible(bVis);
}

QString KTextEdit::plainText() const
{
	return m_edit->toPlainText();
}

void KTextEdit::setPlainText( const QString& txt )
{
	m_edit->setPlainText(txt);
}

QString KTextEdit::htmlText() const
{
	return m_edit->toHtml();
}

void KTextEdit::setHtmlText( const QString& txt )
{
	m_edit->setHtml(txt);
}

void KTextEdit::on_htmlLinkClicked( const QString& url )
{
	if(m_bHtmlUrlForWeb)
	{
		QDesktopServices::openUrl(url);
		return;
	}
	emit htmlLinkClicked(url);
}

bool KTextEdit::hasHtmlUrlForWeb()
{
	return m_bHtmlUrlForWeb;
}

void KTextEdit::setHasHtmlUrlForWeb( bool on )
{
	m_bHtmlUrlForWeb = on;
}

void KTextEdit::getTextFragments( int ipos, int count, QList<QPair<QString,QTextCharFormat>>& frags ) const
{
	m_edit->getTextFragments(ipos, count, frags);
}

void KTextEdit::getTextFragments( QList<QPair<QString,QTextCharFormat>>& frags ) const
{
	m_edit->getTextFragments(frags);
}

void KTextEdit::getSelectTextFragments( QList<QPair<QString,QTextCharFormat>>& frags ) const
{
	m_edit->getSelectTextFragments(frags);
}

void KTextEdit::setVerticalScrollBarStyleColor( const QColor& border, const QColor& normal, const QColor& hover, const QColor& pressed )
{
	m_edit->setVerticalScrollBarStyleColor(border, normal, hover, pressed);
}

void KTextEdit::setVerticalScrollBarStyleColor( const QColor& clr )
{
	m_edit->setVerticalScrollBarStyleColor(QColor(100,100,100), clr, clr.light(107), clr.dark(110));
}

void KTextEdit::setHorizontalScrollBarStyleColor( const QColor& border, const QColor& normal, const QColor& hover, const QColor& pressed )
{
	m_edit->setHorizontalScrollBarStyleColor(border, normal, hover, pressed);
}

void KTextEdit::setHorizontalScrollBarStyleColor( const QColor& clr )
{
	m_edit->setHorizontalScrollBarStyleColor(QColor(100,100,100), clr, clr.light(107), clr.dark(110));
}

void KTextEdit::doThemeChange()
{
	if(!hasTheme())
		return;
	updateTheme();
	__super::doThemeChange();
}

void KTextEdit::updateTheme()
{
	setVerticalScrollBarStyleColor(KTheme::themeColor());
	setHorizontalScrollBarStyleColor(KTheme::themeColor());
}

QColor KTextEdit::scrollBarColor() const
{
	return QColor();
}

void KTextEdit::setScrollBarColor( const QColor& clr )
{
	setVerticalScrollBarStyleColor(clr);
	setHorizontalScrollBarStyleColor(clr);
}

KWidget::DragPolicy KTextEdit::hitTest( const QPointF& point )
{
	if(m_dragPolicyMode == DragAtNoContent)
	{
		DragPolicy dp;
		QPointF pt = mapFromScene(point);
		int pos = m_edit->exactPositionFromPoint(pt.toPoint());
		if(pos >= 0)
		{
			return Nothing;
		}
		QScrollBar *vscroll = m_edit->verticalScrollBar();
		QScrollBar *hscroll = m_edit->horizontalScrollBar();
		QRectF itemRt = rect();
		if(vscroll)
		{
			if(vscroll->isVisibleTo(m_edit))
			{
				QSize vs = vscroll->size();
				QRectF winRt(0, 0, vs.width(), vs.height());
				winRt.moveTo(itemRt.right()-vs.width(), 0);
				if(winRt.contains(pt))
					return Nothing;
			}
		}
		if(hscroll)
		{
			if(hscroll->isVisibleTo(m_edit))
			{
				QSize vs = hscroll->size();
				QRectF winRt(0, 0, vs.width(), vs.height());
				winRt.moveTo(0, itemRt.bottom()-vs.height());
				if(winRt.contains(pt))
					return Nothing;
			}
		}	
	}
	return __super::hitTest(point);
}

KTextEdit::DragPolicyMode KTextEdit::editDragPolicy()
{
	return m_dragPolicyMode;
}

void KTextEdit::setEditDragPolicy( DragPolicyMode edp )
{
	m_dragPolicyMode = edp;
}

bool KTextEdit::autoUniqueTextFormat()
{
	return m_bAutoUniqueTextFont;
}

void KTextEdit::setAutoUniqueTextFormat( bool on )
{
	m_bAutoUniqueTextFont = on;
}

void KTextEdit::setUniqueTextFormat(const QFont& f, const QColor& clr)
{
	m_uniqueTextColor = clr;
	setDocumentDefaultFont(f);
}

QColor KTextEdit::uniqueTextColor() const
{
	return m_uniqueTextColor;
}

void KTextEdit::setUniqueTextColor( const QColor& clr )
{
	m_uniqueTextColor = clr;	
}

void KTextEdit::on_contentsChanged()
{
	if(isReadOnly())
		return;
	if(m_bDoUniqueTextNow)
		return;
	if(m_charsAddedContentChanged > 0)
	{
		QTextDocument *doc = m_edit->document();
		if(m_bAutoUniqueTextFont)
		{
			/*使用文档默认的*/
			makeUniqueTextFormat();
		}
		QTextCursor cursor = textCursor();
		cursor.setPosition(m_posContentChanged + m_charsAddedContentChanged);
		QTextCharFormat fmt = cursor.charFormat();
		if(fmt.isAnchor())
		{
			QTextCharFormat fmtnew;
			m_bDoUniqueTextNow = true;
			fmtnew.setFont(fmt.font());
			cursor.setCharFormat(fmtnew);
			m_edit->setTextCursor(cursor);
			m_bDoUniqueTextNow = false;
		}
	}
}

void KTextEdit::on_contentsChange( int pos, int charsRemoved, int charsAdded )
{
	m_posContentChanged = pos;
	m_charsAddedContentChanged = charsAdded;
	m_charsRemovedContentChanged = charsRemoved;
}

void KTextEdit::setDocumentDefaultFont( const QFont& f )
{
	/*以下两个函数的作用效果是一样的。*/
#if 0
	m_edit->setFont(f);
#else
	m_edit->document()->setDefaultFont(f);
#endif
	if(m_bAutoUniqueTextFont)
	{
		makeUniqueTextFormat();
	}
}

void KTextEdit::setDocumentCharFormat( const QTextCharFormat &format )
{
	QTextCursor cursor = textCursor();

	if(!format.isValid())
		return;

	/*m_bDoUniqueTextNow：增加此属性，目的是避免死循环。*/
	m_bDoUniqueTextNow = true;
	int pos = cursor.position();
	cursor.select(QTextCursor::Document);
	if(cursor.hasSelection())
	{
		cursor.mergeCharFormat(format);
		cursor.clearSelection();
	}
	cursor.setPosition(pos);
	m_bDoUniqueTextNow = false;
}

void KTextEdit::setDocumentDefaultFontSize( int s )
{
	/*以下两个函数的作用效果是一样的。*/
	QFont f = m_edit->document()->defaultFont();
	f.setPointSize(s);
	m_edit->document()->setDefaultFont(f);
}

QFont KTextEdit::documentDefaultFont() const
{
	return m_edit->document()->defaultFont();
}

int KTextEdit::documentDefaultFontSize()
{
	return m_edit->document()->defaultFont().pointSize();
}

void KTextEdit::makeUniqueTextFormat( const QFont& f, const QColor& clr )
{
	QTextCharFormat fmtnew;
	fmtnew.setFont(f);
	fmtnew.setForeground(clr);
	setDocumentCharFormat(fmtnew);
}

void KTextEdit::makeUniqueTextFormat()
{
	QTextDocument *doc = m_edit->document();
	QFont f = doc->defaultFont();
	QTextCharFormat fmtnew;
	fmtnew.setFont(f);
	fmtnew.setForeground(m_uniqueTextColor);
	setDocumentCharFormat(fmtnew);
}

bool KTextEdit::hasContextMenu()
{
	return m_bHasContextMenu;
}

void KTextEdit::setHasContextMenu( bool on )
{
	m_bHasContextMenu = on;
}

QGraphicsProxyWidget * KTextEdit::proxyWidget() const
{
	return m_proxyWidget;
}
