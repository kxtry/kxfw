#include "stdafx.h"
#include "uishower.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(DropObjectWidget);

DropObjectWidget::DropObjectWidget( QGraphicsItem *parent /*= NULL*/ )
: KWidget(parent)
{
	setAcceptDrops(true);
}

void DropObjectWidget::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
	if (event->mimeData()->hasFormat("text/uri-list")) 
	{         
		event->acceptProposedAction();     
	}
}

void DropObjectWidget::dropEvent( QGraphicsSceneDragDropEvent * event )
{
	QList<QUrl> urls = event->mimeData()->urls();     
	if (urls.isEmpty()) 
	{        
		return;      
	}      
	QString fileName = urls.first().toLocalFile();     
	if (fileName.isEmpty()) 
	{       
		return;
	}       

	if (fileName.lastIndexOf(".xml", -1, Qt::CaseInsensitive) != -1 )
	{
		KRadioButton *btn = findChild<KRadioButton*>("btnContentCreate");
		//获取XML所在的目录，并加入到环境变量中。
/*
		QString path = fileName;
		int pos = path.lastIndexOf("/");
		path.resize(pos);
		QStringList myPath = QCoreApplication::libraryPaths();
		qDebug() << myPath;
		QCoreApplication::addLibraryPath(path);
		SetCurrentDirectoryA(path.toStdString().c_str());*/
		if (btn && btn->checked())
		{
			processShowXMLContent(fileName);
		}
		else
		{
			processShowXMLWnd(fileName);
		}
	}
	else
	{
		processImgFile(fileName);
	}
}

void DropObjectWidget::processImgFile( const QString &img )
{

}

void DropObjectWidget::processShowXMLWnd( const QString &xml )
{
	KWindow *wnd = qobject_cast<KWindow*>(KXmlUI::windowFromFile(xml));
	if (wnd)
	{
		wnd->show();
	}
}

void DropObjectWidget::processShowXMLContent( const QString &xml )
{
	KWindow *wnd = qobject_cast<KWindow*>(KXmlUI::windowFromFile(":/layout/Resources/uicontainer.xml"));
	KXmlUI::widgetFromFile(xml, wnd->rootWidget());
	wnd->show();
}
