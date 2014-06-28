#pragma once;

#include "kwidget.h"

class KSequentImageItemPrivate;

class KXFW_API KSequentImageItem : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KSequentImageItem)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(QSize rowcolumn READ rowcolumn WRITE setRowColumn)
	Q_PROPERTY(bool playLoop READ playLoop WRITE setPlayLoop)
	Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart)
	Q_PROPERTY(int fps READ fps WRITE setFps)
	Q_PROPERTY(bool useDefault READ example WRITE playExample)
public:
	explicit KSequentImageItem(QGraphicsItem *parent=NULL);
	~KSequentImageItem();

	void setImagePath( const QString &imageName );
	QString imagePath() const;

	QSize rowcolumn() const;
	void setRowColumn(const QSize& rc);
	void setRowColumn(int row, int col);

	bool playLoop() const;
	void setPlayLoop( bool on );

	bool autoStart() const;
	void setAutoStart(bool on);

	int fps();
	void setFps(int f);

	bool example();
	void playExample(bool on);

	int frameCount();
	QSize frameSize();
public:
	Q_INVOKABLE void start();
	Q_INVOKABLE void stop();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void visibleEvent(bool bvis);

private slots:
	void on_timeout();

private:
	QRectF drawAnimationRect();
	QRectF sourcePixmapRect(int index);
};