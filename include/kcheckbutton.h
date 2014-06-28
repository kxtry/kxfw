#pragma once;

#include "kabstractbutton.h"

class KFrameItem;
class KLabel;
class KCheckButtonPrivate;

class KXFW_API KCheckButton : public KAbstractButton
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KCheckButton)
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(int imageWidth READ imageWidth WRITE setImageWidth)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(bool autoAdjust READ autoAdjust WRITE setAutoAdjust)
	Q_PROPERTY(QMargins textMargin READ textMargins WRITE setTextMargins)
	Q_PROPERTY(bool vertical READ vertical WRITE setVertical)
	Q_PROPERTY(bool reverse READ reverseOrder WRITE setReverseOrder)
	Q_PROPERTY(QObject *label READ label)
	Q_PROPERTY(QObject *frame READ frame)
public:
	explicit KCheckButton(QGraphicsItem *parent = NULL);
	~KCheckButton();

	QString text() const;
	void setText(const QString& title);

	int imageWidth() const;
	void setImageWidth(int s);

	QString imagePath() const;
	void setImagePath(const QString& file);

	QMargins textMargins() const;
	void setTextMargins(const QMargins& margin);

	bool autoAdjust();
	void setAutoAdjust(bool bAdjust);

	bool vertical();
	void setVertical(bool on);

	bool reverseOrder();
	void setReverseOrder(bool on);

	QObject *label() const;
	QObject *frame() const;
private:
	void updateLayout();
	void updateState(ButtonStates states);
};