#pragma once;

#include <QObject>

class KWidget;
class KHelperPrivate;

class KXFW_API KHelper : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KHelper)
	Q_PROPERTY(QString targetPath READ targetPath WRITE setTargetPath)

public:
	explicit KHelper(QObject *parent=NULL);
	virtual ~KHelper();
	
	QString targetPath() const;
	void setTargetPath(const QString& path);

	QObject *targetObject();
	QObject *viewObject();

	void setRootObject(QObject *root);

	bool execute();

protected:
	explicit KHelper(KHelperPrivate &dd, QObject *parent);
	virtual bool doExecute();
protected:
	KHelperPrivate * d_ptr;
};