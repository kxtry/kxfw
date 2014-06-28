#pragma once


#include "kwidget.h"

class KSplitWidgetExtPrivate;

class KXFW_API KSplitWidgetExt : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KSplitWidgetExt)
	Q_DECLARE_PRIVATE(KSplitWidgetExt)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(QString splitbarImage READ splitBarImage WRITE setSplitBarImage)
	Q_PROPERTY(int splitbarSize READ splitBarSize WRITE setSplitBarSize)
public:
	explicit KSplitWidgetExt(QGraphicsItem *parent = NULL);
	explicit KSplitWidgetExt(Qt::Orientation orientation, QGraphicsItem *parent = NULL);
	~KSplitWidgetExt();

	void setOrientation( Qt::Orientation orientation );
	Qt::Orientation orientation() const;

	void insertItem( KWidget* item, int index = -1 );
	void removeItem( KWidget* item );
	void removeItem( int index );

	KWidget *itemByIndex( int index ) const;
	int itemIndex( KWidget *item );

	bool setItemPrefferredSize( KWidget *item, qreal prefferredSize );
    bool getSpliterPreferredSize(int index , int& w, int& h);

	QString splitBarImage() const;
	void setSplitBarImage(const QString& img);

	int splitBarSize() const;
	void setSplitBarSize(int s);

    void relayout();

signals:
	void splited( int splitIndex, const QPointF& pos );

private slots:
	void onItemDestroyed(QObject *obj);

protected:
	void resizeEvent( QGraphicsSceneResizeEvent *event );
	void showEvent( QShowEvent *event );
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

private:
	void addItem( KWidget* item );
};