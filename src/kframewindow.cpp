#include "stdafx.h"
#include "kframewindow.h"
#include "ktitlebar.h"
#include "kspacer.h"
#include "kxmlui.h"
#include "kframewindow_p.h"

KX_WINDOW_CREATOR_GLOBAL_STATIC(KFrameWindow)

KFrameWindow::KFrameWindow( QWidget *parent/*=NULL*/ )
: KPopupWindow(*new KFrameWindowPrivate, parent)
{
	init();
}

KFrameWindow::KFrameWindow( KPopupWindowPrivate &dd, QWidget *parent /* = NULL*/ )
: KPopupWindow(dd, parent)
{
	init();
}

KFrameWindow::~KFrameWindow()
{

}

QObject * KFrameWindow::titlebar() const
{
	Q_D(const KFrameWindow);

	return d->m_titlebar;
}

KWidget * KFrameWindow::rootWidget()
{
	Q_D(KFrameWindow);

	return d->m_content;
}

void KFrameWindow::init()
{
	Q_D(KFrameWindow);
	KWidget *root = __super::rootWidget();
	KTitlebar *titlebar = new KTitlebar(root);
	titlebar->setObjectName(QString("titlebar%1").arg((qlonglong)this));
	KWidget *content = new KSpacer(root);
	content->setObjectName(QString("content%1").arg((qlonglong)this));
	content->setLayoutType(KWidget::VBox);
	root->addItem(titlebar);
	root->addItem(content);
	d->m_titlebar = titlebar;
	d->m_content = content;
	QMetaObject::invokeMethod(titlebar, "updateTitelbar", Qt::QueuedConnection);
}

