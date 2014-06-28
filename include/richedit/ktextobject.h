#pragma once

#include <QTextObjectInterface>
#include <QTextFormat>
#include "ktextedit.h"

class KTextObjectPrivate;

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

	void commonEvent(QMouseEvent* event, quint32 textObjectIndex);

protected:
	explicit KTextObject( KTextObjectPrivate &dd, KTextEdit* editor );
	virtual ~KTextObject();

	virtual QSizeF intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format);
	virtual void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format);

protected:
	KTextObjectPrivate * d_ptr;

private:
	void _init();
};

