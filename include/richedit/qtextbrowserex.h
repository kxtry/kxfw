
#pragma once

#include "stdafx.h"
#include "ktextcursor.h"
#include <QTextBrowser>
#include <QTimer>

class QTextCursor;

const char KTextEditMime[] = "MIME/TextEdit";


class KXFW_API QTextBrowserEx : public QTextBrowser
{
	Q_OBJECT;
	Q_DISABLE_COPY(QTextBrowserEx);
public:
	enum EPasteFlag
	{
		NotSupport = 0,
		Text = 0x1,
		Url = 0x2,
		Image = 0x4,
		Html = 0x8,
		Custom = 0x10,
		All = 0xFFFFFFFF
	};
	Q_DECLARE_FLAGS(EPasteFlags, EPasteFlag)
	Q_FLAGS(EPasteFlags)
	Q_PROPERTY(EPasteFlags pasteFlags READ pasteFlags WRITE setPasteFlags)
public:
	QTextBrowserEx( QWidget* parent = NULL );
	~QTextBrowserEx(void);

	EPasteFlags pasteFlags();
	void setPasteFlags(EPasteFlags fg);

	QTextCursor	cursorFromPoint( const QPoint &point );
	bool isLinkObject(const QPoint &point);

	void scrollToEnd();
	void scrollToTop();

	void makeCtrlEnterAsEnter( bool makeCtrlEnterAsEnter );
	void setEditingFinishKey(Qt::Key key=Qt::Key_Return, QFlags<Qt::KeyboardModifier> modifier=Qt::NoModifier);

	void removeLinkInfo( const QString &linkKey, const QString &linkValue );

	void setMaxLength( int nMaxLen );

	void setReadOnly(bool isReadOnly );
	void setAcceptEnter( bool accept );

	int	 exactPositionFromPoint(const QPoint& pt);
	QTextCharFormat exactCharFormat(int pos);
	QTextCharFormat exactCharFormat(const QPoint& pt);

	QPoint mapToContents(const QPoint &point) const;
	QRect documentExposeRect() const;

	int horizontalOffset() const;
	int verticalOffset() const;

	void setVerticalScrollBarStyleColor(const QColor& border, const QColor& normal, const QColor& hover, const QColor& pressed, int thinkness=10);
	void setHorizontalScrollBarStyleColor(const QColor& border, const QColor& normal, const QColor& hover, const QColor& pressed, int thinkness=10);

	/*当TextFragment为图片或自定议控件时，它的长度是大于或等于1，当它大于1时，需要枚举出内部每个元素*/
	
	void getTextFragments(int ipos, int count, QList<QPair<QString,QTextCharFormat>>& frags) const;
	void getTextFragments(QList<QPair<QString,QTextCharFormat>>& frags) const;
	void getSelectTextFragments(QList<QPair<QString,QTextCharFormat>>& frags) const;

signals:
	/*用户可检查是否为自定义控件，进行自行处理。*/
	void insertMimiData(QTextCursor &tc, const QString txt, const QTextCharFormat& fmt, bool& bHandled);
	void returnPressed();
	void contextMenu( QContextMenuEvent* event );
	void clickedLink(const QString &linkKey, const QString &linkValue );
	void htmlLinkClicked(const QString& url);
	void mouseEvent(QEvent *e, bool *pbHandled);
	void keyEvent(QEvent *e, bool *pbHandled);

protected slots:
	void on_link_clicked(const QUrl&);
	bool on_edit_aboutToInsertText( QString& addText, const QTextCharFormat &format );

protected:
	virtual QMimeData* createMimeDataFromSelection() const;
	virtual void insertFromMimeData(const QMimeData *source);
	virtual bool canInsertFromMimeData(const QMimeData *source) const;
	virtual void contextMenuEvent ( QContextMenuEvent * event );

	virtual void keyPressEvent(QKeyEvent *e);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void inputMethodEvent(QInputMethodEvent *e);
	virtual void focusInEvent(QFocusEvent *e);
	virtual void focusOutEvent(QFocusEvent *e);
private:
	void init();
	int positionFromPoint(const QPoint &pt); 
	void selectImageOnMousePress(const QPoint &pt);
	void textObjectEvent(QMouseEvent* event);

private:

	int m_maxLength;

	bool m_makeCtrlEnterAsEnter;

	QPoint m_linkPoint;

	bool m_isAcceptEnter;

	Qt::Key m_keyEditFinish;
	QFlags<Qt::KeyboardModifier> m_moidfierEditFinish;

	EPasteFlags m_pasteFlags;
};
