#pragma once;

#include "kbubbletip.h"

class KLabel;
class KTextItem;
class KTextBubbleTipPrivate;

class KXFW_API KTextBubbleTip : public KBubbleTip
{
	Q_OBJECT
public:
	explicit KTextBubbleTip(QWidget *parent=NULL);
	~KTextBubbleTip();

	QString text() const;
	void setText(const QString& txt);

	Qt::Orientation orientation();
	void setOrientation(Qt::Orientation o);

	qreal textWidth();
	void setTextWidth(qreal v);

	int elapseTime();
	void setElapseTime(int t);

	KLabel *labelObject();
	KTextItem *textObject();

signals:
	void clicked();

protected:
	void updateTextLayout();

protected slots:
	void on_clicked();
	void on_elapse_timeout();

protected:
	virtual void showEvent(QShowEvent *event);
	virtual void hideEvent(QShowEvent *event);

private:
	KTextBubbleTipPrivate *d;
};