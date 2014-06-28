#include "stdafx.h"
#include "qlineeditex.h"
#include "kmenu.h"

QLineEditEx::QLineEditEx(QWidget *parent)
: QLineEdit(parent)
{

}

QLineEditEx::~QLineEditEx(void)
{
}

void QLineEditEx::contextMenuEvent( QContextMenuEvent *event )
{
	event->ignore();
}

void QLineEditEx::keyPressEvent( QKeyEvent *event )
{
	QLineEdit::keyPressEvent(event);
	if (QLineEdit::Password == echoMode() && Qt::Key_CapsLock == event->key())
	{
		emit capsLockChanged();
	}
	else if(Qt::Key_Up == event->key())
	{
		emit updownKeyClicked(true);
	}
	else if(Qt::Key_Down == event->key())
	{
		emit updownKeyClicked(false);
	}
	event->accept();
	emit clicked(event);
}

void QLineEditEx::mousePressEvent( QMouseEvent *event )
{
	QLineEdit::mousePressEvent(event);
	emit buttonClicked(event->button());
}

void QLineEditEx::focusOutEvent( QFocusEvent *event )
{
	QLineEdit::focusOutEvent(event);
	emit focusOut();
}

void QLineEditEx::focusInEvent( QFocusEvent *event )
{
	QLineEdit::focusInEvent(event);
	emit focusIn();
}

QSize QLineEditEx::contentSize()
{
	QFontMetrics fm(font());
	QString txt = text();
	int w = fm.width(txt) + fm.leading()*2;
	int h = fm.height();
	return QSize(w, h);
}

#define ACCEL_KEY(k) (QLatin1Char('\t') + QString(QKeySequence(k)))

void QLineEditEx::showContextMenu()
{
	KMenu *popup = KMenu::createPopupMenu(0);
	QAction *action = 0;

	if (!isReadOnly()) 
	{
		action = popup->addAction(QLineEdit::tr("&Undo") + ACCEL_KEY(QKeySequence::Undo));
		action->setEnabled(isUndoAvailable());
		connect(action, SIGNAL(triggered()), SLOT(undo()));

		action = popup->addAction(QLineEdit::tr("&Redo") + ACCEL_KEY(QKeySequence::Redo));
		action->setEnabled(isRedoAvailable());
		connect(action, SIGNAL(triggered()), SLOT(redo()));

		popup->addSeparator();
	}

	if (!isReadOnly()) 
	{
		action = popup->addAction(QLineEdit::tr("Cu&t") + ACCEL_KEY(QKeySequence::Cut));
		action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
		connect(action, SIGNAL(triggered()), SLOT(cut()));
	}

	action = popup->addAction(QLineEdit::tr("&Copy") + ACCEL_KEY(QKeySequence::Copy));
	action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
	connect(action, SIGNAL(triggered()), SLOT(copy()));

	if (!isReadOnly()) 
	{
		action = popup->addAction(QLineEdit::tr("&Paste") + ACCEL_KEY(QKeySequence::Paste));
		action->setEnabled(!isReadOnly() && !QApplication::clipboard()->text().isEmpty());
		connect(action, SIGNAL(triggered()), SLOT(paste()));
	}

	if (!isReadOnly()) 
	{
		action = popup->addAction(QLineEdit::tr("Delete"));
		action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
		connect(action, SIGNAL(triggered()), this, SLOT(clear()));
	}

	action = popup->addAction(QLineEdit::tr("Select All") + ACCEL_KEY(QKeySequence::SelectAll));
	action->setEnabled(!text().isEmpty());
	connect(action, SIGNAL(triggered()), SLOT(selectAll()));
	popup->exec();
}
