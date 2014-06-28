#include "stdafx.h"
#include "kdialog.h"
#include "ktitlebar.h"
#include "kspacer.h"
#include "kxmlui.h"
#include "kpopupwindow_p.h"


KX_WINDOW_CREATOR_GLOBAL_STATIC(KDialogBase)

class KDialogBasePrivate : public KPopupWindowPrivate
{
public:
	KDialogBasePrivate()
	{
		eventLoop = NULL;
		result = 0;
	}

	QEventLoop *eventLoop;
	int result;
};

KDialogBase::KDialogBase( QWidget *parent/*=NULL*/ )
: KPopupWindow(*new KDialogBasePrivate(), parent)
{
	Q_D(KDialogBase);
}

KDialogBase::~KDialogBase()
{

}

int KDialogBase::doModal(KWidget *center, bool bActive, bool bAutoDelete)
{
	Q_D(KDialogBase);


	__super::construct();

	setAttribute(Qt::WA_DeleteOnClose, false);
	setAttribute(Qt::WA_ShowModal, true);
	setWindowModality(Qt::WindowModal);
	moveCenter(center);
	show();	
	emit dialogShow();
	if(bActive)
	{
		activateWindow();
	}
	QEventLoop eventLoop;
	d->eventLoop = &eventLoop;
	int retval = eventLoop.exec(QEventLoop::DialogExec);
	d->eventLoop = NULL;
	setAttribute(Qt::WA_ShowModal, false);
	setWindowModality(Qt::NonModal);
	hide();
	if(bAutoDelete)
	{
		QMetaObject::invokeMethod(this, "deleteLater", Qt::QueuedConnection);
	}
	return retval;
}


void KDialogBase::setAccepted()
{
	done(QDialog::Accepted);
}

void KDialogBase::setRejected()
{
	done(QDialog::Rejected);
}

void KDialogBase::done( int result )
{
	d_func()->result = result;
	close();
}

void KDialogBase::closeEvent( QCloseEvent *event )
{
	__super::closeEvent(event);

	Q_D(KDialogBase);

	if(d->eventLoop)
	{
		d->eventLoop->exit(d->result);
		d->eventLoop = NULL;
	}

	event->accept();
}

void KDialogBase::endDialog( int code )
{
	done(code);
}

void KDialogBase::exit()
{
	QObject *obj = sender();
	if(obj == NULL)
	{
		setAccepted();
		return;
	}
	QString name = obj->objectName();
	int code = name.toInt();
	endDialog(code);
}

bool KDialogBase::accepted() const
{
	return d_func()->result == QDialog::Accepted;
}

bool KDialogBase::rejected() const
{
	return d_func()->result == QDialog::Rejected;
}

int KDialogBase::result() const
{
	return d_func()->result;
}



/*                 KDialog                                         */


KX_WINDOW_CREATOR_GLOBAL_STATIC(KDialog)

KDialog::KDialog( QWidget *parent/*=NULL*/ )
: KDialogBase(parent)
{
	KWidget *root = __super::rootWidget();
	KTitlebar *titlebar = new KTitlebar(root);
	titlebar->setObjectName(QString("titlebar%1").arg((qlonglong)this));
	KWidget *content = new KSpacer(root);
	content->setObjectName(QString("content%1").arg((qlonglong)this));
	content->setLayoutType(KWidget::VBox);
	root->addItem(titlebar);
	root->addItem(content);

}

KDialog::~KDialog()
{

}

QObject * KDialog::titlebar() const
{
	return findChild<KWidget*>(QString("titlebar%1").arg((qlonglong)this));;
}

KWidget * KDialog::rootWidget()
{
	return findChild<KWidget*>(QString("content%1").arg((qlonglong)this));
}
