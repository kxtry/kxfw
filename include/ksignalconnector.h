#pragma once;

#include "khelper.h"

class KSignalConnectorPrivate;
class KXFW_API KSignalConnector : public KHelper
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KSignalConnector)
	Q_PROPERTY(QString signal READ signal WRITE setSignal)
	Q_PROPERTY(QString slot READ slot WRITE setSlot)
	Q_PROPERTY(QString receiver READ receiver WRITE setReceiver)
public:
	explicit KSignalConnector(QObject *parent=NULL);
	~KSignalConnector();
	
	QString signal() const;
	void setSignal(const QString& f);

	QString slot() const;
	void setSlot(const QString& f);

	QString receiver() const;
	void setReceiver(const QString& obj);
protected:
	bool doExecute();
};