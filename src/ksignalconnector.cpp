#include "stdafx.h"
#include "ksignalconnector.h"

#include "khelper_p.h"
#include "kxmlui.h"

KX_HELPER_CREATOR_GLOBAL_STATIC(KSignalConnector)

class KSignalConnectorPrivate : public KHelperPrivate
{
	Q_DECLARE_PUBLIC(KSignalConnector)
public:
	QString signal;
	QString slot;
	QString receiver;
public:
	KSignalConnectorPrivate()
	{

	}

	~KSignalConnectorPrivate()
	{

	}
};

KSignalConnector::KSignalConnector( QObject *parent/*=NULL*/ )
: KHelper(* new KSignalConnectorPrivate(), parent)
{

}

KSignalConnector::~KSignalConnector()
{

}

QString KSignalConnector::signal() const
{
	Q_D(const KSignalConnector);
	return d->signal;
}

void KSignalConnector::setSignal( const QString& f )
{
	Q_D(KSignalConnector);
	d->signal = f;
}

QString KSignalConnector::slot() const
{
	Q_D(const KSignalConnector);
	return d->slot;
}

void KSignalConnector::setSlot( const QString& f )
{
	Q_D(KSignalConnector);
	d->slot = f;
}

QString KSignalConnector::receiver() const
{
	Q_D(const KSignalConnector);
	return d->receiver;
}

void KSignalConnector::setReceiver( const QString& obj )
{
	Q_D(KSignalConnector);
	d->receiver = obj;
}

bool KSignalConnector::doExecute()
{
	if(!__super::doExecute())
	{
		return false;
	}
	
	return true;
}
