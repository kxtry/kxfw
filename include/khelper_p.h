#pragma once;

class KHelper;

class KXFW_API KHelperPrivate
{
	Q_DECLARE_PUBLIC(KHelper)
public:
	QString targetPath;
	QPointer<QObject> objTarget;
	QPointer<QObject> objRoot;
public:
	KHelperPrivate();
	virtual ~KHelperPrivate();

	bool doExecute();
	QObject *viewObject();

protected:
	KHelper *q_ptr;
};