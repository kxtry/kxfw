#pragma once;

struct QtGradient
{
	QGradient::Type t;
	union
	{
		struct 
		{
			qreal startx, starty;
			qreal finalx, finaly;
		} l;
		struct
		{
			qreal centerx,centery;
			qreal radius;
		} r;
		struct
		{
			qreal centerx,centery;
			qreal angle;
		} c;
	} d;
	QGradientStops gs;
	QGradient::Spread spread;
	QtGradient()
	{
		t = QGradient::NoGradient;
	}
};
