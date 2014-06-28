#include "stdafx.h"
#include "kmethodinvoker.h"

#include "khelper_p.h"

class KMethodInvokerPrivate : public KHelperPrivate
{
	Q_DECLARE_PUBLIC(KMethodInvoker)
public:
	KMethodInvokerPrivate()
	{

	}

	~KMethodInvokerPrivate()
	{

	}
};

KMethodInvoker::KMethodInvoker( QObject *parent/*=NULL*/ )
: KHelper(parent)
{

}

KMethodInvoker::~KMethodInvoker()
{

}
