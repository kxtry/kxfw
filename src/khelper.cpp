#include "stdafx.h"
#include "khelper.h"
#include "kwidget.h"

#include "khelper_p.h"
#include "kutility.h"
#include "kxmlui.h"

KX_HELPER_CREATOR_GLOBAL_STATIC(KHelper)

KHelperPrivate::KHelperPrivate()
{
	
}

KHelperPrivate::~KHelperPrivate()
{

}

bool KHelperPrivate::doExecute()
{
	objTarget = KUtility::findObjectByPath(objRoot, targetPath);
	return objTarget ? true : false;
}

QObject *KHelperPrivate::viewObject()
{
	if(objRoot == NULL)
		return NULL;
	if(objRoot->isWidgetType())
	{
		return objRoot;
	}

	KWidget *widget = qobject_cast<KWidget*>(objRoot);
	if(widget == NULL)
		return NULL;

	return widget->view();
}

KHelper::KHelper( QObject *parent/*=NULL*/ )
: QObject(parent)
, d_ptr(new KHelperPrivate)
{
	d_ptr->q_ptr = this;
}

KHelper::KHelper( KHelperPrivate &dd, QObject *parent )
: QObject(parent) 
, d_ptr(&dd)
{
	d_ptr->q_ptr = this;
}

KHelper::~KHelper()
{
	delete d_ptr;
}

QString KHelper::targetPath() const
{
	Q_D(const KHelper);
	return d->targetPath;
}

void KHelper::setTargetPath( const QString& path )
{
	Q_D(KHelper);
	d->targetPath = path;
}

QObject * KHelper::targetObject()
{
	Q_D(KHelper);
	return d->objTarget;
}

void KHelper::setRootObject( QObject *root )
{
	Q_D(KHelper);
	d->objRoot = root;
}

bool KHelper::execute()
{
	return doExecute();
}

bool KHelper::doExecute()
{
	Q_D(KHelper);
	return d->doExecute();
}

QObject * KHelper::viewObject()
{
	Q_D(KHelper);
	return d->viewObject();
}