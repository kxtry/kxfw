#pragma once;

#include "kwidget.h"

class KTextItemPrivate;

class KXFW_API KTextItem : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KTextItem)

	Q_PROPERTY(QString text READ toHtml WRITE setText)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
	Q_PROPERTY(QString file READ filePath WRITE setFilePath)
	Q_PROPERTY(bool idealSizeAsMinimun READ idealSizeAsMinimun WRITE setIdealSizeAsMinimum)
public:
	explicit KTextItem(QGraphicsItem *parent = NULL);
	KTextItem(const QString& text, QGraphicsItem *parent = NULL);
	~KTextItem();

	QString toHtml() const;
	QString toText() const;
	void setText(const QString& txt);
	QSizeF idealSize() const;
	QSizeF hitTextSize(int width);

	int fontSize();
	void setFontSize(int s);

	QColor textColor() const;
	void setTextColor(const QColor& txt);

	QString filePath() const;
	void setFilePath(const QString& file);

	bool idealSizeAsMinimun();
	void setIdealSizeAsMinimum(bool on);

protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
	void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
	void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
	DragPolicy hitTest(const QPointF& point);
	QSizeF sizeHint( Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const;
private:
	void init();
	QTextCursor cursorFromPoint( const QPoint &point );
	bool isLinkObject(const QPoint &point);
	QString urlFromPoint(const QPoint& pt);
	QTextDocument *document();
	QTextCursor textCursor();
	
private:
	int _positionFromPoint( const QPoint &point );
};