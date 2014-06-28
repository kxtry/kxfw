#include "stdafx.h"
#include "kmessagebox.h"
#include "kwidget.h"
#include "kxmlui.h"
#include "ktextitem.h"
#include "ktextedit.h"
#include "ktitlebar.h"
#include "ktoolbar.h"
#include "kpushbutton.h"
#include "kframeitem.h"

KX_WINDOW_CREATOR_GLOBAL_STATIC(KMessageBox)

struct MsgBoxData
{
	QString szframe;
	QString mask;
	bool bmargins;
	QMargins margins;
	QColor bkcolor;
	QString btnimage;
	QString btnclose;

	QString warnico;
	QString criticalico;
	QString questionico;
	QString infoico;

	MsgBoxData()
	{
		bmargins = false;
	}
};

Q_GLOBAL_STATIC(MsgBoxData, getMsgBoxData)

KMessageBox::KMessageBox( QWidget *parent/*=NULL*/ )
: KDialog(parent)
{

}

KMessageBox::~KMessageBox()
{

}

int KMessageBox::critical( KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text /*= QString()*/, const QString& button2Text /*= QString()*/, int leftSeconds /*= -1*/ )
{
	QWidget *parent = center ? center->view() : NULL;
	KMessageBox *dlg = qobject_cast<KMessageBox*>(KXmlUI::windowFromFile(":/xml/theme/xml/msgbox.xml", parent));
	Q_ASSERT_X(dlg, __FUNCTION__, "failed to create message box window");
	dlg->m_typeMsgBox = eCritical;
	dlg->initSkin();
	dlg->initMessageBox(title, text, button0Text, button1Text, button2Text, leftSeconds);
	dlg->moveCenter(center);
	return dlg->doModal();
}

int KMessageBox::warning( KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text /*= QString()*/, const QString& button2Text /*= QString()*/, int leftSeconds /*= -1*/ )
{
	QWidget *parent = center ? center->view() : NULL;
	KMessageBox *dlg = qobject_cast<KMessageBox*>(KXmlUI::windowFromFile(":/xml/theme/xml/msgbox.xml", parent));
	Q_ASSERT_X(dlg, __FUNCTION__, "failed to create message box window");
	dlg->m_typeMsgBox = eWarning;
	dlg->initSkin();
	dlg->initMessageBox(title, text, button0Text, button1Text, button2Text, leftSeconds);
	dlg->moveCenter(center);
	return dlg->doModal();
}

int KMessageBox::question( KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text /*= QString()*/, const QString& button2Text /*= QString()*/, int leftSeconds /*= -1*/ )
{
	QWidget *parent = center ? center->view() : NULL;
	KMessageBox *dlg = qobject_cast<KMessageBox*>(KXmlUI::windowFromFile(":/xml/theme/xml/msgbox.xml", parent));
	Q_ASSERT_X(dlg, __FUNCTION__, "failed to create message box window");
	dlg->m_typeMsgBox = eQuestion;
	dlg->initSkin();
	dlg->initMessageBox(title, text, button0Text, button1Text, button2Text, leftSeconds);
	dlg->moveCenter(center);
	return dlg->doModal();
}

int KMessageBox::information( KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text /*= QString()*/, const QString& button2Text /*= QString()*/, int leftSeconds /*= -1*/ )
{
	QWidget *parent = center ? center->view() : NULL;
	KMessageBox *dlg = qobject_cast<KMessageBox*>(KXmlUI::windowFromFile(":/xml/theme/xml/msgbox.xml", parent));
	Q_ASSERT_X(dlg, __FUNCTION__, "failed to create message box window");
	dlg->m_typeMsgBox = eInfomation;
	dlg->initSkin();
	dlg->initMessageBox(title, text, button0Text, button1Text, button2Text, leftSeconds);
	dlg->moveCenter(center);
	return dlg->doModal();
}

#define TEXT_MIN_SIZE		QSize(100, 100)
#define TEXT_MAX_SIZE		QSize(500, 400)

void KMessageBox::initMessageBox( const QString &title, const QString& text, const QString& button0Text, const QString& button1Text /*= QString()*/, const QString& button2Text /*= QString()*/, int leftSeconds/*=-1*/ )
{
	/*设置标题*/
	KTitlebar *tbar = qobject_cast<KTitlebar*>(titlebar());
	tbar->setTitle(title);
	/*设置按钮*/
	KToolBar *toolbar = findChild<KToolBar*>("buttonToolbar");
	Q_ASSERT_X(toolbar, __FUNCTION__, "failed to find toolbar named of buttonToolbar");
	m_button0 = toolbar->findChild<KPushButton*>("button0");
	Q_ASSERT_X(m_button0, __FUNCTION__, "failed to find button named of button0");
	QObject::connect(m_button0, SIGNAL(clicked()), this, SLOT(button0Clicked()));
	m_button0->setText(button0Text);
	QMargins m(10,0,10,0);
	m_button0->setSizeToContent(m);
	if(button1Text.isEmpty())
	{
		toolbar->hideButton("button1");
	}
	else
	{
		KPushButton *button1 = toolbar->findChild<KPushButton*>("button1");
		Q_ASSERT_X(button1, __FUNCTION__, "failed to find button named of button1");
		button1->setText(button1Text);
		button1->setSizeToContent(m);
		QObject::connect(button1, SIGNAL(clicked()), this, SLOT(button1Clicked()));
	}
	if(button2Text.isEmpty())
	{
		toolbar->hideButton("button2");
	}
	else
	{
		KPushButton *button2 = toolbar->findChild<KPushButton*>("button2");
		Q_ASSERT_X(button2, __FUNCTION__, "failed to find button named of button2");
		button2->setText(button2Text);
		button2->setSizeToContent(m);
		QObject::connect(button2, SIGNAL(clicked()), this, SLOT(button2Clicked()));
	}

	if(leftSeconds > 0)
	{
		QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_elapse_timeout()));
		m_button0Text = button0Text;
		m_leftSeconds = leftSeconds;
		QString txt = QString("%1(%2)").arg(m_button0Text).arg(m_leftSeconds);
		m_button0->setText(txt);
		m_button0->setSizeToContent(m);
		m_timer.start(1000);
	}
	/*设置文字*/
	KTextItem *textItem = findChild<KTextItem*>("textShower");
	if(textItem == NULL)
		return;
	textItem->setText(text);
	QSizeF okSize = textItem->idealSize();
	textItem->setFixSize(okSize);
	QSizeF s = calcWindowPreferSize();
	resize(s.toSize());
}

void KMessageBox::on_elapse_timeout()
{
	QString txt = QString("%1(%2)").arg(m_button0Text).arg(--m_leftSeconds);
	m_button0->setText(txt);
	QMargins m(10,0,10,0);
	m_button0->setSizeToContent(m);
	if(m_leftSeconds <= 0)
	{
		button0Clicked();
	}
}

void KMessageBox::onEnterPressed( Qt::KeyboardModifiers moidifier )
{
	button0Clicked();
}

void KMessageBox::onEscPressed( Qt::KeyboardModifiers moidifier )
{
	button1Clicked();
}

void KMessageBox::button0Clicked()
{
	m_timer.stop();
	done(MSGBOX_ID0);
}

void KMessageBox::button1Clicked()
{
	m_timer.stop();
	done(MSGBOX_ID1);
}

void KMessageBox::button2Clicked()
{
	m_timer.stop();
	done(MSGBOX_ID2);
}

void KMessageBox::onCloseClick()
{
	button1Clicked();
}

void KMessageBox::setMaskMargins( const QMargins& margins )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->margins = margins;
	mbd->bmargins = true;
}

void KMessageBox::setFrameImage( const QString& frame )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->szframe = frame;
}

void KMessageBox::setBackgroundColor( const QColor& clr )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->bkcolor = clr;
}

void KMessageBox::setMaskImage( const QString& image )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->mask = image;
}

/*
//:/image/theme/messagbox_error.png
//:/image/theme/messagebox_infomation.png
//:/image/theme/messagebox_question.png
//:/image/theme/messagebox_warning.png
*/
void KMessageBox::initSkin()
{
	bool breset = false;
	MsgBoxData *mbd = getMsgBoxData();
	if(mbd->bkcolor.isValid())
	{
		__super::setBackgroundColor(mbd->bkcolor);
	}
	if(!mbd->mask.isEmpty())
	{
		__super::setMaskImage(mbd->mask);
		breset = true;
	}
	if(mbd->bmargins)
	{
		__super::setMaskMargins(mbd->margins);
		breset = true;
	}
	if(!mbd->szframe.isEmpty())
	{
		__super::setFrameImage(mbd->szframe);
		breset = true;
	}
	if(!mbd->btnimage.isEmpty())
	{
		KToolBar *toolbar = findChild<KToolBar*>("buttonToolbar");
		KPushButton *button0 = toolbar->findChild<KPushButton*>("button0");
		KPushButton *button1 = toolbar->findChild<KPushButton*>("button1");
		KPushButton *button2 = toolbar->findChild<KPushButton*>("button2");
		button0->setImagePath(mbd->btnimage);
		button1->setImagePath(mbd->btnimage);
		button2->setImagePath(mbd->btnimage);
	}
	if(!mbd->btnclose.isEmpty())
	{
		KTitlebar *tbar = qobject_cast<KTitlebar*>(titlebar());
		tbar->setClosePath(mbd->btnclose);
	}
	KFrameItem *icon = findChild<KFrameItem*>("iconShower");
	switch(m_typeMsgBox)
	{
	case eCritical:
		{
			if(mbd->criticalico.isEmpty())
			{
				icon->setImagePath(":/image/theme/messagbox_error.png");
			}
			else
			{
				icon->setImagePath(mbd->criticalico);
			}
			icon->setFixSizeByPixmapSize();
		}
		break;
	case eQuestion:
		{
			if(mbd->questionico.isEmpty())
			{
				icon->setImagePath(":/image/theme/messagebox_question.png");
			}
			else
			{
				icon->setImagePath(mbd->questionico);
			}
			icon->setFixSizeByPixmapSize();
		}
		break;
	case eInfomation:
		{
			if(mbd->infoico.isEmpty())
			{
				icon->setImagePath(":/image/theme/messagebox_infomation.png");
			}
			else
			{
				icon->setImagePath(mbd->questionico);
			}
			icon->setFixSizeByPixmapSize();
		}
		break;
	case eWarning:
		{
			if(mbd->warnico.isEmpty())
			{
				icon->setImagePath(":/image/theme/messagebox_warning.png");
			}
			else
			{
				icon->setImagePath(mbd->warnico);
			}
			icon->setFixSizeByPixmapSize();
		}
		break;
	}
	
	if(breset)
	{
		resetFrame();
	}
}

void KMessageBox::setCriticalSymbol( const QString& ico )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->criticalico = ico;
}

void KMessageBox::setWarningSymbol( const QString& ico )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->warnico = ico;
}

void KMessageBox::setQuestionSymbol( const QString& ico )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->questionico = ico;
}

void KMessageBox::setInfomationSymbol( const QString& ico )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->infoico = ico;
}

void KMessageBox::setButtonImage( const QString& image )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->btnimage = image;
}

void KMessageBox::setCloseButtonImage( const QString& image )
{
	MsgBoxData *mbd = getMsgBoxData();
	mbd->btnclose = image;
}
