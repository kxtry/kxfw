#include "stdafx.h"
#include "ktextobject.h"


class KTextObjectPrivate
{
	Q_DECLARE_PUBLIC(KTextObject);
public:
	KTextObjectPrivate();
	virtual ~KTextObjectPrivate();

protected:
	KTextObject *q_ptr;
	KTextEdit *m_edit;
};

KTextObjectPrivate::KTextObjectPrivate()
: q_ptr( NULL )
, m_edit( NULL )
{

}

KTextObjectPrivate::~KTextObjectPrivate()
{

}


//////////////////////////////////////////////////////////////////////////

KTextObject::KTextObject( KTextEdit* editor ) 
: d_ptr( new KTextObjectPrivate )
{
	d_ptr->q_ptr = this;
	d_func()->m_edit = editor;
	_init();
}

KTextObject::KTextObject( KTextObjectPrivate &dd, KTextEdit* editor )
: d_ptr( &dd )
{
	d_ptr->q_ptr = this;
	d_func()->m_edit = editor;
	_init();
}

KTextObject::~KTextObject()
{
	Q_ASSERT_X(d_ptr, __FUNCTION__, "private data is null.");
	delete d_ptr;
	d_ptr = NULL;
}

QSizeF KTextObject::intrinsicSize( QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	return QSizeF( 80.0, 36.0 );
}

void KTextObject::drawObject( QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format )
{
	Q_D(KTextObject);

}

void KTextObject::_init()
{
	Q_D(KTextObject);
	if (d->m_edit && d->m_edit->document())
	{
		setParent(d->m_edit->document());
	}
}

void KTextObject::commonEvent( QMouseEvent* event, quint32 textObjectIndex )
{
	Q_D(KTextObject);

	QPointF mousePos = d_func()->m_edit->mapToContents( event->pos() );

	switch ( event->type() )
	{
	case QEvent::MouseButtonPress:
		break;
	case  QEvent::MouseButtonRelease:
		break;
	case QEvent::MouseMove:
		break;
	}
}