#pragma once;

#include "kdialog.h"
class KWidget;
class KPushButton;

#define MSGBOX_ID0			(1) //IDOK
#define MSGBOX_ID1			(2)	//IDCANCEL
#define MSGBOX_ID2			(3)	//IDABORT



class KXFW_API KMessageBox : public KDialog
{
	Q_OBJECT
public:
	explicit KMessageBox(QWidget *parent=NULL);
	~KMessageBox();

	/*
	1.button0 alway is default button.
	*/
	static void setMaskMargins(const QMargins& margins);
	static void setMaskImage(const QString& image);
	static void setFrameImage(const QString& frame);
	static void setBackgroundColor(const QColor& clr);
	static void setButtonImage(const QString& image);
	static void setCloseButtonImage(const QString& image);

	static void setCriticalSymbol(const QString& ico);
	static void setWarningSymbol(const QString& ico);
	static void setQuestionSymbol(const QString& ico);
	static void setInfomationSymbol(const QString& ico);

	
	/*
	1.leftSeconds：> 0代表倒计时，< 0:代表是不使用倒时。
	*/
	static int critical(KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text = QString(), const QString& button2Text = QString(), int leftSeconds=-1);
	static int warning(KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text = QString(), const QString& button2Text = QString(), int leftSeconds=-1);
	static int question(KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text = QString(), const QString& button2Text = QString(), int leftSeconds=-1);
	static int information(KWidget *center, const QString &title, const QString& text, const QString& button0Text, const QString& button1Text = QString(), const QString& button2Text = QString(), int leftSeconds=-1);

protected:
	virtual void onEnterPressed(Qt::KeyboardModifiers moidifier);
	virtual void onEscPressed(Qt::KeyboardModifiers moidifier);
	virtual void onCloseClick();
private:
	void initMessageBox(const QString &title, const QString& text, const QString& button0Text, const QString& button1Text = QString(), const QString& button2Text = QString(), int leftSeconds=-1);
	void initSkin();
private slots:
	void on_elapse_timeout();
	void button0Clicked();
	void button1Clicked();
	void button2Clicked();
private:
	enum MsgBoxType
	{
		eCritical,
		eWarning,
		eQuestion,
		eInfomation
	};
	MsgBoxType m_typeMsgBox;

	int m_leftSeconds;
	QTimer m_timer;
	QString m_button0Text;
	QPointer<KPushButton> m_button0;
};