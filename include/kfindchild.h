#pragma once;

#include "khelper.h"

class KFindChildPrivate;
class KFindChild : public KHelper
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KFindChild)
	Q_PROPERTY(QString targetPath READ targetPath WRITE setTargetPath)
public:
	explicit KFindChild(QObject *parent=NULL);
	~KFindChild();

	QString targetPath() const;
	void setTargetPath(const QString& path);

private:
	bool doExecute();
};