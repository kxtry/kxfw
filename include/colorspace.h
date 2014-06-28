#pragma once

namespace ColorSpace
{

// rgb 2 xyz
//----------------------------------------------------------------------------------------
void rgb2xyz(double* x, double* y, double* z, ushort r, ushort g, ushort b);

// xyz 2 rgb
//----------------------------------------------------------------------------------------
void xyz2rgb(ushort* r, ushort* g, ushort* b, double x, double y, double z);

// xyz 2 lab
//----------------------------------------------------------------------------------------
void xyz2lab(double* l, double* a, double* b, double x, double y, double z);

// lab 2 xyz
//----------------------------------------------------------------------------------------
void lab2xyz(double* x, double* y, double* z, double l, double a, double b);

// rgb 2 lab
//----------------------------------------------------------------------------------------
void rgb2lab(double* l, double* a, double* bex, ushort r, ushort g, ushort b);

// lab 2 rgb
//----------------------------------------------------------------------------------------
void lab2rgb(ushort* r, ushort* g, ushort* b, double l, double a, double bex);

};// end of ColorSpace

namespace KHSL
{
	/*
	HSL:的整数取值范围是：Hue:0~360,S:0~100,L:0~100
	*/
	void Rgb2Hsl(QRgb colorrgb,float *h,float *s,float *l);
	void Hsl2Rgb(float hh,float ss,float ll,WORD a,QRgb *colorrgb);
	QImage CreateColorTable(int width, int height, float s);
	void UpdateColorTable(QImage& img, float s);
	QImage CreateColorBar(int width, int height, float h, float l);
	void UpdateColorBar(QImage& img, float h, float l);
	QRgb CalMergeColor(QRgb Source, float Powl, float SelectH, float SelectS, float SelectL, int MainH, int MainS, int MainL);
	QRgb MergeClr(QRgb SrcClr, QRgb chgClr, int MainH = -1, int MainS = -1, int MainL = - 1);
	QImage MergeImage(QImage srcImg, QRgb chgClr, int MainH = -1, int MainS = -1, int MainL = - 1);
}

namespace KRGB
{
	QRgb MergeClr(QRgb SrcClr, QRgb chgClr);
	QImage MergeImage(QImage srcImg, QRgb chgClr);
}