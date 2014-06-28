#pragma once;

class DropObjectWidget : public KWidget
{
public:
	explicit DropObjectWidget(QGraphicsItem *parent = NULL);

protected:
	void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
	void dropEvent(QGraphicsSceneDragDropEvent * event);
	void processImgFile(const QString &img);
	void processShowXMLWnd(const QString &xml);
	void processShowXMLContent(const QString &xml);
};
