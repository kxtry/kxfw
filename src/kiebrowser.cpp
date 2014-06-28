/************************************************************************/
/*                                                                      */
/*    版本更新或帮助文档网址：http://www.kxtry.com                      */
/*                                                                      */
/************************************************************************/
#include "stdafx.h"
#include "magiciebrowser.h"

#include "magicwidget_p.h"
#include "xmluifactory.h"

#include "iebrowser.h"

#include <QAxWidget>
#include <MsHTML.h>
#include <atlbase.h>
#include <ExDispid.h>
#include <MsHtmHst.h>
#include <QAxScriptManager>
#include <qaxtypes.h>

MAGIC_WIDGET_CREATOR_GLOBAL_STATIC(MagicIEBrowser)

class MagicIEBrowserPrivate : public MagicWidgetPrivate
{
	Q_DECLARE_PUBLIC(MagicIEBrowser)
public:
	IEBrowser *iebrowser;
	QString urlpath;
	bool bAutoGo;
	QTimer timer;
	IEExternalFunctionHandler *handler;
public:
	MagicIEBrowserPrivate()
		: iebrowser(NULL)
		, bAutoGo(false)
		, handler(NULL)
	{

	}

	~MagicIEBrowserPrivate()
	{

	}

	void setExternalFunctionHandler( IEExternalFunctionHandler *hdl )
	{
		handler = hdl;
		if(iebrowser)
		{
			iebrowser->setExternalFunctionHandler(hdl);
		}
	}

	void init()
	{
		if(iebrowser)
			return;
		Q_Q(MagicIEBrowser);

		iebrowser = new IEBrowser(q->view());
		if(!urlpath.isEmpty() && bAutoGo)
		{
			iebrowser->navigate(urlpath);
		}

		if(handler)
		{
			iebrowser->setExternalFunctionHandler(handler);
		}

		bool ok1 = QObject::connect(iebrowser, SIGNAL(TitleChange(QString)), q, SIGNAL(titleChange(QString)));
		bool ok2 = QObject::connect(iebrowser, SIGNAL(ProgressChange(int,int)), q, SIGNAL(progressChange(int,int)));
		bool ok3 = QObject::connect(iebrowser, SIGNAL(CommandStateChange(int,bool)), q, SIGNAL(commandStateChange(int,bool)));
		bool ok4 = QObject::connect(iebrowser, SIGNAL(BeforeNavigate(QString,int,QString,QVariant&,QString,bool&)), q, SIGNAL(beforeNavigate(QString,int,QString,QVariant&,QString,bool&)));
		bool ok5 = QObject::connect(iebrowser, SIGNAL(NavigateComplete(QString)), q, SIGNAL(navigateComplete(QString)));
		bool ok6 = QObject::connect(iebrowser, SIGNAL(signal(QString,int,void*)), q, SLOT(on_slot(QString,int,void*)));

		return;
		QObject::connect(&timer, SIGNAL(timeout()), q, SLOT(on_timeout()), Qt::UniqueConnection);
		timer.setSingleShot(true);
		timer.start(5000);
	}

	int navigate(const QString& txt)
	{
		if(iebrowser == NULL)
			return -1;
		QVariant val = iebrowser->navigate(txt);
		iebrowser->setSilent(true);
		return val.toInt();
	}

	int callFunction(const QString & funname,
		const QVariant &var0 = QVariant(), 
		const QVariant &var1 = QVariant(), 
		const QVariant &var2 = QVariant(), 
		const QVariant &var3 = QVariant(), 
		const QVariant &var4 = QVariant(), 
		const QVariant &var5 = QVariant(), 
		const QVariant &var6 = QVariant(), 
		const QVariant &var7 = QVariant(), 
		const QVariant &var8 = QVariant())
	{
		if(iebrowser == NULL)
			return -1;
		return iebrowser->callFunction(funname, var0, var1, var2, var3, var4, var5, var6, var7, var8);
	}

	int callFunction(const QString & funname, QList<QVariant> &args)
	{
		if(iebrowser == NULL)
			return -1;
		return iebrowser->callFunction(funname, args);
	}

	void setVisible(bool bVisible)
	{
		if(iebrowser == NULL)
			return;
		iebrowser->setVisible(bVisible);
	}

	void resize(int w, int h)
	{
		if(iebrowser == NULL)
			return;
		Q_Q(MagicIEBrowser);
		QRectF rt = q->rectToView();
		iebrowser->setGeometry(rt.toRect());
	}
};


MagicIEBrowser::MagicIEBrowser( QGraphicsItem *parent/*=NULL*/ )
: MagicWidget(*new MagicIEBrowserPrivate(), parent)
{
	setFlags(QGraphicsItem::ItemHasNoContents);
	Q_D(MagicIEBrowser);
}

MagicIEBrowser::~MagicIEBrowser()
{

}

void MagicIEBrowser::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(MagicIEBrowser);
	if(d->iebrowser == NULL)
	{
		d->init();
	}

	QSizeF s = event->newSize();
	d->resize(s.width(), s.height());
}

void MagicIEBrowser::showEvent( QShowEvent *event )
{
	Q_D(MagicIEBrowser);
	d->setVisible(true);
}

void MagicIEBrowser::hideEvent( QHideEvent *event )
{
	Q_D(MagicIEBrowser);
	d->setVisible(false);
}

QString MagicIEBrowser::url() const
{
	Q_D(const MagicIEBrowser);
	return d->urlpath;
}

void MagicIEBrowser::setUrl( const QString& url )
{
	Q_D(MagicIEBrowser);
	if(d->urlpath == url)
		return;
	d->urlpath = url;
}

bool MagicIEBrowser::autoGo()
{
	Q_D(MagicIEBrowser);
	return d->bAutoGo;
}

void MagicIEBrowser::setAutoGo( bool bGo )
{
	Q_D(MagicIEBrowser);
	if(d->bAutoGo == bGo)
		return;
	d->bAutoGo = bGo;
	if(bGo && !d->urlpath.isEmpty())
	{
		d->navigate(d->urlpath);
	}
}

void MagicIEBrowser::on_slot( const QString &name, int argc, void *argv )
{
	VARIANTARG *params = (VARIANTARG*)argv;
	qDebug() << name;
	if (name.startsWith("BeforeNavigate2(")) 
	{
		IDispatch *pDisp = params[argc-1].pdispVal;
		VARIANTARG URL = *params[argc-2].pvarVal;
		VARIANTARG Flags = *params[argc-3].pvarVal;
		VARIANTARG TargetFrameName = *params[argc-4].pvarVal;
		VARIANTARG PostData = *params[argc-5].pvarVal;
		VARIANTARG Headers = *params[argc-6].pvarVal;
		VARIANT_BOOL *Cancel = params[argc-7].pboolVal;
	}
}

void MagicIEBrowser::on_timeout()
{
	Q_D(MagicIEBrowser);

//	QList<QVariant> var;
//	d->callFuntion("abcdef()", var);

	callFunction("abcdef");
}

int MagicIEBrowser::callFunction( const QString & funname, const QVariant &var0 /*= QVariant()*/, const QVariant &var1 /*= QVariant()*/, const QVariant &var2 /*= QVariant()*/, const QVariant &var3 /*= QVariant()*/, const QVariant &var4 /*= QVariant()*/, const QVariant &var5 /*= QVariant()*/, const QVariant &var6 /*= QVariant()*/, const QVariant &var7 /*= QVariant()*/, const QVariant &var8 /*= QVariant()*/ )
{
	Q_D(MagicIEBrowser);
	return d->callFunction(funname, var0, var1, var2, var3, var4, var5, var6, var7, var8);
}

int MagicIEBrowser::callFunction( const QString & funname, QList<QVariant> &args )
{
	Q_D(MagicIEBrowser);
	return d->callFunction(funname, args);	
}

void MagicIEBrowser::setExternalFunctionHandler( IEExternalFunctionHandler *handler )
{
	Q_D(MagicIEBrowser);
	d->setExternalFunctionHandler(handler);
}

void MagicIEBrowser::navigate( const QString& url )
{
	Q_D(MagicIEBrowser);
	d->urlpath = url;
	d->navigate(url);
}

IEBrowser * MagicIEBrowser::getIEBrowser()
{
	Q_D(MagicIEBrowser);
	return d->iebrowser;
}
