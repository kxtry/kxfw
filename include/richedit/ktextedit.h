
#pragma once

#include "kwidget.h"
#include "ktextcursor.h"


class KFrameItem;
class QTextBrowserEx;
class KTextObject;

class KXFW_API KTextEdit : public KWidget
{
	Q_OBJECT;
	Q_DISABLE_COPY(KTextEdit)
public:
	enum DragPolicyMode
	{
		DragAtItem,
		DragAtNoContent,
	};
	Q_ENUMS(DragPolicyMode)
	Q_PROPERTY(DragPolicyMode editDrag READ editDragPolicy WRITE setEditDragPolicy)
	Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
	Q_PROPERTY(bool selectable READ isSelectable WRITE setSelectable)
	Q_PROPERTY(QString text READ toText WRITE setText)
	Q_PROPERTY(QString plainText READ plainText WRITE setPlainText)
	Q_PROPERTY(QString htmlText READ htmlText WRITE setHtmlText)
	Q_PROPERTY(bool hasScrollbar READ hasScrollbar WRITE setScrollbarVisible)
	Q_PROPERTY(bool htmlUrlForWeb READ hasHtmlUrlForWeb WRITE setHasHtmlUrlForWeb)
	Q_PROPERTY(Qt::ScrollBarPolicy vcrollPolicy READ verticalScrollBarPolicy WRITE setVerticalScrollBarPolicy)
	Q_PROPERTY(Qt::ScrollBarPolicy hcrollPolicy READ horizontalScrollBarPolicy WRITE setHorizontalScrollBarPolicy)
	Q_PROPERTY(QColor scrollColor READ scrollBarColor WRITE setScrollBarColor)

	Q_PROPERTY(bool hasContextMenu READ hasContextMenu WRITE setHasContextMenu)
	Q_PROPERTY(bool autoUniqueTextFormat READ autoUniqueTextFormat WRITE setAutoUniqueTextFormat)
	Q_PROPERTY(QColor uniqueTextColor READ uniqueTextColor WRITE setUniqueTextColor)
	Q_PROPERTY(QObject* edit READ realWidget)
public:
	explicit KTextEdit( QGraphicsItem *parent = NULL );

	virtual ~KTextEdit();

	bool isSelectable();
	void setSelectable(bool bSelected);

	bool hasContextMenu();
	void setHasContextMenu(bool on);

	bool autoUniqueTextFormat();
	void setAutoUniqueTextFormat(bool on);

	void setUniqueTextFormat(const QFont& f, const QColor& clr);
	void makeUniqueTextFormat(const QFont& f, const QColor& clr);
	void makeUniqueTextFormat();

	int uniqueFontSize();
	void setUniqueFontSize(int s);

	QString uniqueFontFamily() const;
	void setUniqueFontFamily(const QString& f);

	QColor uniqueTextColor() const;
	void setUniqueTextColor(const QColor& clr);

	DragPolicyMode editDragPolicy();
	void setEditDragPolicy(DragPolicyMode edp);

	void insertHtml ( const QString &text );
	void insertText( const QString &text );

	QString toPlainText() const;
	QString plainText() const;
	void setPlainText(const QString& txt);
	
	QString toHtml() const;
	QString htmlText() const;
	void setHtmlText(const QString& txt);

	QString toText() const;
	void setText(const QString& txt);

	bool hasFocus() const;
	void setFocus();	

	KTextCursor	textCursor() const ;
	KTextCursor	cursorForPosition( const QPoint& pos );
	bool isLinkObject(const QPoint& pos);

	QFontMetrics	fontMetrics() const ;
	void setFrameShape ( QFrame::Shape frameShape );

	bool hasScrollbar();
	void setScrollbarVisible(bool bVisible);

	Qt::ScrollBarPolicy verticalScrollBarPolicy();
	void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy policy);

	Qt::ScrollBarPolicy horizontalScrollBarPolicy();
	void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy policy);

	void setStyleSheet(const QString &styleSheet );

	void setWordWrapMode(QTextOption::WrapMode policy);

	int verticalSliderPosition();
	int horizontalSliderPostion();
	void setVerticalSliderPosition(int val);
	void setHorizontalSliderPostion(int val);

	QTextCharFormat charFormat () const;
	void setCharFormat ( const QTextCharFormat & format );
	
	void setTextCursor(const QTextCursor &cursor);

	QTextDocument* document();
	void selectAll();

	bool isReadOnly() const;
	void setReadOnly( bool readOnly );

	void makeCtrlEnterAsEnter( bool makeCtrlEnterAsEnter );
	void setAcceptEnter( bool accept );

	QScrollBar* verticalScrollBar() const ;
	QScrollBar* horizontalScrollBar() const;

	void setOverwriteMode( bool isOverWrite );
	bool overwriteMode() const;

	void setMaxLength( int maxLength );

	void setAcceptDrops( bool accept );

	void setUndoRedoEnabled ( bool enable );

	void setTabChangesFocus(bool enable);

	void setPlaceholderText(const QString &text);
	QString placeHolderText() const;

	bool hasHtmlUrlForWeb();
	void setHasHtmlUrlForWeb(bool on);

	KTextCursor	currentLinkCursor();

	void setVerticalScrollBarStyleColor(const QColor& border, const QColor& normal, const QColor& hover, const QColor& pressed);
	void setHorizontalScrollBarStyleColor(const QColor& border, const QColor& normal, const QColor& hover, const QColor& pressed);

	void setVerticalScrollBarStyleColor(const QColor& clr);
	void setHorizontalScrollBarStyleColor(const QColor& clr);

	QColor scrollBarColor() const;
	void setScrollBarColor(const QColor& clr);

	void registerTextObjectHandler(int objectType, KTextObject* ctrl);

//TODO:
	QWidget* realWidget() const;
	QGraphicsProxyWidget *proxyWidget() const;
	QPointF mapToContents(const QPointF &point) const;
	int horizontalOffset() const;
	int verticalOffset() const;

	QFont currentFont() const;
	/*设置当前选中的文本或光标所在位置的字体*/
	void setCurrentFont(const QFont& f);

	/*设置当前默认字体，当前没有指定当前字体时，即使用该字体*/
	QFont documentDefaultFont() const;
	void setDocumentDefaultFont(const QFont& f);

	int documentDefaultFontSize();
	void setDocumentDefaultFontSize(int s);

	void setDocumentCharFormat(const QTextCharFormat &format);
    
	void getTextFragments(int ipos, int count, QList<QPair<QString,QTextCharFormat>>& frags) const;
	void getTextFragments(QList<QPair<QString,QTextCharFormat>>& frags) const;
	void getSelectTextFragments(QList<QPair<QString,QTextCharFormat>>& frags) const;

public:
	bool isModified();
	bool fontBold();
	void setFontBold(bool on);
	bool fontItalic();
	void setFontItalic(bool on);
	bool fontUnderline();
	void setFontUnderline(bool on);
	bool fontStrikeOut();
	void setFontStrikeOut(bool on);
	QString fontFamily() const;
	void setFontFamily(const QString& family);
	QColor textColor() const;
	void setTextColor(const QColor& clr);
	int fontPointSize();
	void setFontPointSize(int s);
	QColor textBackgroundColor() const;
	void setTextBackgroundColor(const QColor& clr);
	Qt::Alignment textAlignment();
	void setTextAlignment(Qt::Alignment a);
	bool findText(const QString& keyFind, bool bCaseSensitive, bool bEntireWord, bool bForward);
	bool replaceText(const QString& keyFind, const QString& keyReplace, bool bCaseSensitive, bool bEntireWord, bool bForward);
signals:
	void insertMimiData(QTextCursor &tc, const QString txt, const QTextCharFormat& fmt, bool& bHandled); 
	void linkClicked (const QString &linkKey, const QString &linkValue );
	// insert html with link, will emit this signal.
	void htmlLinkClicked( const QString& url );
	void contextMenu(QMenu* menu, bool *pbContinue);
	void returnPressed();
	void textChanged();
	void currentCharFormatChanged(const QTextCharFormat & f);
	void clearContent();

public slots:
	void copy();
	void cut();
	void clear();
	void paste();
	void undo();
	void redo();
	void deleteText();

	void scrollToEnd();
	void scrollToTop();
	void scrollToLeft();


protected slots:
	void on_contextMenu(QContextMenuEvent* event);
	void on_cutAction_triggered( bool );
	void on_copyAction_triggered( bool );
	void on_pasteAction_triggered( bool );
	void on_selectAll_triggered( bool );
	
	void on_mouseEvent(QEvent *e, bool *pbHandled);
	void on_visibleChanged();
	void on_htmlLinkClicked(const QString& url);

	void on_contentsChanged();
	void on_contentsChange(int position, int charsRemoved, int charsAdded);

protected:
	int	exactPositionFromPoint( const QPoint& point );

protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual QPainterPath shape() const;
	virtual void doThemeChange();
	virtual DragPolicy hitTest(const QPointF& point);
private:
	void init();
	void updateTheme();
	void resetGeometry(const QRectF &r);
protected:
	QPointer<QTextBrowserEx> m_edit;
	QPointer<QGraphicsProxyWidget> m_proxyWidget;
private:
	bool m_bHtmlUrlForWeb;
	DragPolicyMode m_dragPolicyMode;

	bool m_bAutoUniqueTextFont;
	QColor m_uniqueTextColor;
	bool m_bHasContextMenu;

	int m_bDoUniqueTextNow;//增加此属性，目的是避免死循环。
	int m_posContentChanged;
	int m_charsRemovedContentChanged;
	int m_charsAddedContentChanged;
};
