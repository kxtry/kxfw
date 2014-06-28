#pragma once;

#include "ktreewidget.h"

class KCommonTreeWidgetPrivate;

class KXFW_API KCommonTreeWidget : public KTreeWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KCommonTreeWidget)
public:
	explicit KCommonTreeWidget(QGraphicsItem *parent=NULL);
	~KCommonTreeWidget();

	qint64 nodeRoot();
	qint64 addChild(qint64 nodeid, const QString& text);
	qint64 addChild(qint64 nodeid, const QString& text, const QVariant& other);
	qint64 addChild(qint64 nodeid, const QString& icon, const QString& text, const QVariant& other);
	qint64 addChild(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals);

	qint64 insertAfter(qint64 nodeid, const QString& text);
	qint64 insertAfter(qint64 nodeid, const QString& text, const QVariant& other);
	qint64 insertAfter(qint64 nodeid, const QString& icon, const QString& text, const QVariant& other);
	qint64 insertAfter(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals);

	qint64 insertBefore(qint64 nodeid, const QString& text);
	qint64 insertBefore(qint64 nodeid, const QString& text, const QVariant& other);
	qint64 insertBefore(qint64 nodeid, const QString& icon, const QString& text, const QVariant& other);
	qint64 insertBefore(qint64 nodeid, const QMap<Qt::ItemDataRole, QVariant>& vals);

	void removeNode(qint64 nodeid);
	void removeChilds(qint64 nodeid);
	void removeAll();

	int count() const;
	int childCount(qint64 nodeid) const;
};