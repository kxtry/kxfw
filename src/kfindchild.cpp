#include "stdafx.h"
#include "kfindchild.h"

#include "khelper_p.h"
#include "kutility.h"
#include "kxmlui.h"

KX_HELPER_CREATOR_GLOBAL_STATIC(KFindChild)

class KFindChildPrivate : public KHelperPrivate
{
	Q_DECLARE_PUBLIC(KFindChild)
public:
	QString targetPath;
public:
	KFindChildPrivate()
	{

	}

	~KFindChildPrivate()
	{

	}

	bool doExecute()
	{
		if(targetPath.isEmpty())
			return false;
		if(objRoot == NULL)
			return false;
		targetPath.remove('.\\');
		targetPath.remove('./');
		targetPath.remove(' ');
		targetPath.replace('\\', '/');
		QStringList targets = targetPath.split('/');
		if(targets.isEmpty())
			return false;
		QObject *tmpRoot = objRoot;
		QObject *tmpTarget = NULL;
		for(int i = 0; i < targets.count(); i++)
		{
			tmpTarget = KUtility::findObject(tmpRoot, targets.at(i));
			tmpRoot = tmpTarget;
		}
		objTarget = tmpTarget;
		return (objTarget==NULL ? false : true);
	}
};

KFindChild::KFindChild( QObject *parent/*=NULL*/ )
: KHelper(*new KFindChildPrivate(), parent)
{

}

KFindChild::~KFindChild()
{

}

QString KFindChild::targetPath() const
{
	Q_D(const KFindChild);
	return d->targetPath;
}

void KFindChild::setTargetPath( const QString& path )
{
	Q_D(KFindChild);
	d->targetPath = path;
}

bool KFindChild::doExecute()
{
	Q_D(KFindChild);
	return d->doExecute();
}

