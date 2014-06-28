#pragma once;

#include "magicwidget.h"


class IEExternalFunctionHandler
{
public:
	//如果支持该函数，则返回非0数据，不支持则是小于0。
	virtual int functionId(const QString& name) = 0;
	virtual int invoke(int funId, DISPPARAMS *pDispParams, VARIANT *pVarResult) = 0;
};


class IEBrowser;
class MagicIEBrowserPrivate;
class MAGICFW_EXPORT MagicIEBrowser : public MagicWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(MagicIEBrowser)
public:
	Q_PROPERTY(QString url READ url WRITE setUrl)
	Q_PROPERTY(bool autoGo READ autoGo WRITE setAutoGo)
public:
	explicit MagicIEBrowser(QGraphicsItem *parent=NULL);
	~MagicIEBrowser();
	
	QString url() const;
	void setUrl(const QString& url);

	void navigate(const QString& url);
	IEBrowser *getIEBrowser();

	bool autoGo();
	void setAutoGo(bool bGo);

	void setExternalFunctionHandler(IEExternalFunctionHandler *handler);

	int callFunction(const QString & funname,
		const QVariant &var0 = QVariant(), 
		const QVariant &var1 = QVariant(), 
		const QVariant &var2 = QVariant(), 
		const QVariant &var3 = QVariant(), 
		const QVariant &var4 = QVariant(), 
		const QVariant &var5 = QVariant(), 
		const QVariant &var6 = QVariant(), 
		const QVariant &var7 = QVariant(), 
		const QVariant &var8 = QVariant());

	int callFunction(const QString & funname, QList<QVariant> &args);
signals:
	void titleChange(const QString &title);
	void progressChange(int a, int b);
	void commandStateChange(int cmd, bool on);
	void beforeNavigate(QString,int,QString,QVariant&,QString,bool&);
	void navigateComplete(QString);

private slots:
	void on_slot(const QString &name, int argc, void *argv);

	void on_timeout();
protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
};