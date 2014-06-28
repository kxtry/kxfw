#pragma once;
#include "kwidget.h"

class KIconPrivate;

class KXFW_API KIcon : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KIcon)
public:
	enum Mode 
	{ 
		Normal = 0, 
		Disabled, 
		Active, 
		Selected
	};
	enum State 
	{ 
		On = 0, 
		Off
	};
	Q_ENUMS(State)
	Q_ENUMS(Mode)
public:
	Q_PROPERTY(QString iconPath READ iconPath WRITE setIconPath)
	Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
	Q_PROPERTY(Mode mode READ mode WRITE setMode)
	Q_PROPERTY(State state READ state WRITE setState)
	Q_PROPERTY(Qt::Alignment iconAlignment READ iconAlignment WRITE setIconAlignment)
public:
	explicit KIcon(QGraphicsItem *parent = NULL);
	~KIcon();

	QIcon icon() const;
	void setIcon(const QIcon& ico);

	QString iconPath() const;
	void setIconPath(const QString& file);

	KIcon::Mode mode() const;
	void setMode(KIcon::Mode m);

	KIcon::State state() const;
	void setState(KIcon::State s);

	QSize actualSize(const QSize & size=QSize(), KIcon::Mode mode = Normal, KIcon::State state = Off ) const;

	Qt::Alignment iconAlignment() const;
	void setIconAlignment(const Qt::Alignment align);
private:
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
};