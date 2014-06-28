#pragma once

#include "kwidget.h"

class KToolBarPrivate;

class KXFW_API KToolBar : public KWidget
{
    Q_OBJECT
	Q_DECLARE_PRIVATE(KToolBar)
public:
    explicit KToolBar(QGraphicsItem *parent = NULL);
    ~KToolBar();

	Q_INVOKABLE KWidget *findButton(const QString& objectName);
	Q_INVOKABLE void showButton(const QString& objectName);
	Q_INVOKABLE void hideButton(const QString& objectName);
	Q_INVOKABLE void connectSignalToSlot(const QObject* wobj);
protected:
	virtual void childEvent(QChildEvent *event);
	virtual void construct();
private slots:
	void on_child_visibleChanged();
private:
	void trimRightItemSpacing();
};