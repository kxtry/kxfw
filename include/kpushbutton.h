#pragma once;

#include "kabstractbutton.h"

class KPushButtonPrivate;

class KXFW_API KPushButton : public KAbstractButton
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KPushButton)

	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
	Q_PROPERTY(bool bold READ textBold WRITE setTextBold)
	Q_PROPERTY(QString iconPath READ iconPath WRITE setIconPath)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(QPoint clickOffset READ clickOffset WRITE setClickOffset)
	Q_PROPERTY(int iconSize READ iconSize WRITE setIconSize)
	Q_PROPERTY(QMargins iconMargins READ iconMargins WRITE setIconMargins)
	Q_PROPERTY(QMargins textMargins READ textMargins WRITE setTextMargins)
	Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)
	Q_PROPERTY(Qt::Alignment iconAlignment READ iconAlignment WRITE setIconAlignment)
	Q_PROPERTY(QObject *buttonFrame READ buttonFrame)
	Q_PROPERTY(QObject *buttonIcon READ buttonIcon)
	Q_PROPERTY(QObject *buttonText READ buttonText)
public:
	explicit KPushButton(QGraphicsItem *parent = NULL);
	~KPushButton();

	QString text() const;
	void setText(const QString& text);

	QColor textColor() const;
	void setTextColor(const QColor& clr);

	int fontSize();
	void setFontSize(int fs);

	bool textBold();
	void setTextBold(bool on);

	QString iconPath() const;
	void setIconPath(const QString& file);

	QString imagePath() const;
	void setImagePath(const QString& file);

	Qt::Orientation orientation();
	void setOrientation(Qt::Orientation or);

	int iconSize() const;
	void setIconSize(int s);

	QMargins iconMargins() const;
	void setIconMargins(const QMargins& margin);

	QMargins textMargins() const;
	void setTextMargins(const QMargins& m);

	Qt::Alignment textAlignment();
	void setTextAlignment(Qt::Alignment align);

	Qt::Alignment iconAlignment();
	void setIconAlignment(Qt::Alignment icon);

	QPoint clickOffset() const;
	void setClickOffset(const QPoint& offset);

	void addItem(KWidget *item);
	void removeItem(KWidget *item);

	void setFont(const QFont& f);

	QObject *buttonFrame() const;
	QObject *buttonIcon() const;
	QObject *buttonText() const;
public:
	Q_INVOKABLE void setSizeToFrameImage();
	Q_INVOKABLE void setSizeToContent(const QMargins& m=QMargins());

private:
	void updateLayout();
	void updateState(ButtonStates states);
};