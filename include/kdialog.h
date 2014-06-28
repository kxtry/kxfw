#pragma once;

#include "kpopupwindow.h"

class KDialogBasePrivate;


class KXFW_API KDialogBase : public KPopupWindow
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KDialogBase)
public:
	enum ERetCode 
	{
		Accepted = QDialog::Accepted,
		Rejected = QDialog::Rejected,
	};
public:
	explicit KDialogBase(QWidget *parent=NULL);
	~KDialogBase();
	int doModal(KWidget *center = NULL, bool bActive=true, bool bAutoDelete=true);
	void endDialog(int code);
	int result() const;
	bool accepted() const;
	bool rejected() const;
public slots:
	virtual void setAccepted();
	virtual void setRejected();
	virtual void done( int result );
signals:
	void dialogShow();
protected slots:
	void exit();
protected:
	virtual void closeEvent( QCloseEvent *event );
};



class KXFW_API KDialog : public KDialogBase
{
	Q_OBJECT
public:
	Q_PROPERTY(QObject *titlebar READ titlebar)
public:
	explicit KDialog(QWidget *parent=NULL);
	~KDialog();

	QObject *titlebar() const;

public:
	KWidget * rootWidget();
};