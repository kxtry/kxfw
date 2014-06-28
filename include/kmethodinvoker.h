#pragma once;

#include "khelper.h"

class KMethodInvokerPrivate;

class KXFW_API KMethodInvoker : public KHelper
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KMethodInvoker)
public:
	explicit KMethodInvoker(QObject *parent=NULL);
	~KMethodInvoker();
};