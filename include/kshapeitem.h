#pragma once;

#include "kwidget.h"

class KShapeItemPrivate;

class KXFW_API KShapeItem : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KShapeItem)
public:
	enum Shape
	{
		Rect,
		RoundRect,
		Ellipse,
		Pie,
		Chord
	};

	Q_ENUMS(Shape)

public:
	Q_PROPERTY(Qt::PenStyle penStyle READ penStyle WRITE setPenStyle)
	Q_PROPERTY(Qt::PenCapStyle capStyle READ capStyle WRITE setPenCapStyle)
	Q_PROPERTY(Qt::PenJoinStyle joinStyle READ joinStyle WRITE setPenJoinStyle)
	Q_PROPERTY(Qt::BrushStyle brushStyle READ brushStyle WRITE setBrushStyle)
	Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
	Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor)
	Q_PROPERTY(QMargins shapeMargins READ shapeMargins WRITE setShapeMargins)
	Q_PROPERTY(bool smooth READ smooth WRITE setSmooth)
	Q_PROPERTY(int roundPixel READ roundPixel WRITE setRoundPixel)
	Q_PROPERTY(int penWidth READ penWidth WRITE setPenWidth)
	Q_PROPERTY(Shape shape READ drawShape WRITE setDrawShape)

	Q_PROPERTY(QSize fan READ fanAngle WRITE setFanAngle)
	Q_PROPERTY(QString texture READ textureBrush WRITE setTextureBrush)
	Q_PROPERTY(QtGradient gradient READ gradientBrush WRITE setGradientBrush)

public:
	explicit KShapeItem(QGraphicsItem *parent=NULL);
	~KShapeItem();

	Qt::PenStyle penStyle();
	void setPenStyle(Qt::PenStyle ps);
	
	Qt::PenCapStyle capStyle();
	void setPenCapStyle(Qt::PenCapStyle cs);
	
	Qt::PenJoinStyle joinStyle();
	void setPenJoinStyle(Qt::PenJoinStyle js);
	
	Qt::BrushStyle brushStyle();
	void setBrushStyle(Qt::BrushStyle bs);
	
	QColor penColor() const;
	void setPenColor(const QColor& clr);
	
	QColor brushColor() const;
	void setBrushColor(const QColor& clr);
	
	QMargins shapeMargins() const;
	void setShapeMargins(QMargins& m);
	
	bool smooth();
	void setSmooth(bool on);

	int roundPixel();
	void setRoundPixel(int pixels);
	
	int penWidth();
	void setPenWidth(int w);

	Shape drawShape() const;
	void setDrawShape( Shape s);

	QSize fanAngle() const;
	void setFanAngle(const QSize& s);

	QString textureBrush() const;
	void setTextureBrush(const QString& t);
	
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Pad
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Repeat
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|angle:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	

	QtGradient gradientBrush() const;
	void  setGradientBrush(const QtGradient& gd);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
};