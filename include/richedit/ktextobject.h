#pragma once

#include <QTextObjectInterface>
#include <QTextFormat>
#include "ktextedit.h"

class KTextObjectPrivate;

#define defTextObjectIndex	QTextFormat::ObjectIndex + 1000

class KXFW_API KTextObject: public QObject, public QTextObjectInterface
{
public:
	Q_OBJECT
	Q_INTERFACES(QTextObjectInterface)
	Q_DISABLE_COPY(KTextObject)
	Q_DECLARE_PRIVATE_D(d_ptr, KTextObject)

public:
	enum { objectType = QTextFormat::UserObject + 1 };

public:
	explicit KTextObject( KTextEdit* editor );

protected:
	explicit KTextObject( KTextObjectPrivate &dd, KTextEdit* editor );
	virtual ~KTextObject();

	virtual QSizeF intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format);
	virtual void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format);
	virtual void commonEvent(QMouseEvent* event, quint32 textObjectIndex);

	KTextEdit *textEdit() const;

protected:
	KTextObjectPrivate * d_ptr;

private:
	friend class QTextBrowserEx;
	void _init();
};

