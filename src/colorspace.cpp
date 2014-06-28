#include "stdafx.h"
#include "colorspace.h"

/** reference to :
* http://en.wikipedia.org/wiki/Lab_color_space#CIE_XYZ_to_CIE_L.2Aa.2Ab.2A_.28CIELAB.29_and_CIELAB_to_CIE_XYZ_conversions 
* http://www.easyrgb.com/index.php?X=MATH
*/
namespace ColorSpace{
// rgb 2 xyz
//----------------------------------------------------------------------------------------
static double rgb2xyz_correct(double cl) {
	if ( cl > 0.04045 ) cl = ::qPow( ( ( cl + 0.055 ) / 1.055 ) , 2.4);
	else  cl = cl / 12.92;

	return cl * 100.0;
}

static ushort clampui(ushort var, ushort min, ushort max)
{
	if(var > max) return max;
	if(var < min) return min;
	return var;
}

void rgb2xyz(double* x, double* y, double* z, ushort r, ushort g, ushort b){
	r = clampui(r, 0, 255);
	g = clampui(g, 0, 255);
	b = clampui(b, 0, 255);

	double varr = ( 1.0 * r / 255 );        //R from 0 to 255
	double varg = ( 1.0 * g / 255 );        //G from 0 to 255
	double varb = ( 1.0 * b / 255 );        //B from 0 to 255

	varr = rgb2xyz_correct(varr);
	varg = rgb2xyz_correct(varg);
	varb = rgb2xyz_correct(varb);

	//Observer. = 2бу, Illuminant = D65
	*x = varr * 0.4124 + varg * 0.3576 + varb * 0.1805;
	*y = varr * 0.2126 + varg * 0.7152 + varb * 0.0722;
	*z = varr * 0.0193 + varg * 0.1192 + varb * 0.9505;
}

// xyz 2 rgb
//----------------------------------------------------------------------------------------
static double xyz2rgb_correct(double cl) {
	if ( cl > 0.0031308 ) cl = 1.055 * ::qPow(cl, 0.41667) - 0.055;
	else  cl = cl * 12.92;

	return cl;
}

static double clampf(double var, double min, double max)
{
	if(var > max) return max;
	if(var < min) return min;
	return var;
}

void xyz2rgb(ushort* r, ushort* g, ushort* b, double x, double y, double z)
{
	x = clampf(x, 0.0, 95.047);
	y = clampf(y, 0.0, 100.000);
	z = clampf(z, 0.0, 108.883);

	double varx = x / 100.0;        //X from 0 to  95.047      (Observer = 2бу, Illuminant = D65)
	double vary = y / 100.0;       //Y from 0 to 100.000
	double varz = z / 100.0;       //Z from 0 to 108.883

	double varr = varx *  3.2406 + vary * -1.5372 + varz * -0.4986;
	double varg = varx * -0.9689 + vary *  1.8758 + varz *  0.0415;
	double varb = varx *  0.0557 + vary * -0.2040 + varz *  1.0570;

	varr = xyz2rgb_correct(varr);
	varg = xyz2rgb_correct(varg);
	varb = xyz2rgb_correct(varb);
	
	*r = (ushort)(varr * 255.0);
	*g = (ushort)(varg * 255.0);
	*b = (ushort)(varb * 255.0);
}

// xyz 2 lab
//----------------------------------------------------------------------------------------
static double xyz2lab_correct(double cl) {
	if (cl > 0.008856) cl = ::qPow(cl, 0.33333);
	else cl = 7.787 * cl + 0.13793;
	return cl;
}

void xyz2lab(double* l, double* a, double* b, double x, double y, double z){
	double varx = x / 95.047;		//ref_X = Illuminant= D65 95.047
	double vary = y / 100.00;		//ref_Y = D65 100.00
	double varz = z / 108.88;        //ref_Z =  D65 108.88

	varx = xyz2lab_correct(varx);
	vary = xyz2lab_correct(vary);
	varz = xyz2lab_correct(varz);

	*l = ( 116.0 * vary ) - 16.0;
	*a = 500.0 * ( varx - vary );
	*b = 200.0 * ( vary - varz );
}

// lab 2 xyz
//----------------------------------------------------------------------------------------
static double lab2xyz_correct(double cl) {
	if (::qPow(cl, 3.0) > 0.008856) cl = ::qPow(cl, 3.0);
	else cl = (cl - 0.13793)/7.787;
	return cl;
}

void lab2xyz(double* x, double* y, double* z, double l, double a, double b)
{
	double vary = ( l + 16.0 ) / 116.0;
	double varx = a / 500.0 + vary;
	double varz = vary - b / 200.0;

	vary = lab2xyz_correct(vary);
	varx = lab2xyz_correct(varx);
	varz = lab2xyz_correct(varz);

	//  Observer= 2бу, Illuminant= D65
	*x = 95.047 * varx;
	*y = 100.000 * vary;
	*z = 108.883 * varz;
}

// rgb 2 lab
//----------------------------------------------------------------------------------------
void rgb2lab(double* l, double* a, double* bex, ushort r, ushort g, ushort b)
{
	double x, y, z;
	rgb2xyz(&x, &y, &z, r, g, b);
	xyz2lab(l, a, bex, x, y, z);
}

// lab 2 rgb
//----------------------------------------------------------------------------------------
void lab2rgb(ushort* r, ushort* g, ushort* b, double l, double a, double bex)
{
	double x, y, z;
	lab2xyz(&x, &y, &z, l, a, bex);
	xyz2rgb(r, g, b, x, y, z);
}

};// end of ColorSpace

namespace KHSL
{

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))  

#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1))) 

#define GetRedValue(rgb)		(LOBYTE(rgb))
#define GetGreenValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBlueValue(rgb)		(LOBYTE((rgb)>>16))
#define GetAlphaValue(rgb)      (LOBYTE((rgb)>>24))

inline DWORD RGB2BGR( DWORD rgb )
{
	return (GetAlphaValue(rgb)<<24) | (GetRedValue(rgb)<<16) | (GetGreenValue(rgb)<<8) | GetBlueValue(rgb);
}

inline DWORD BGR2RGB( DWORD bgr )
{
	return (GetAlphaValue(bgr)<<24) | (GetRedValue(bgr)<<16) | (GetGreenValue(bgr)<<8) | GetBlueValue(bgr);
}

void RGBF2HSL(float r, float g, float b, float *h,float *s,float *l);
void HSL2RGBF(float hh,float ss,float ll,WORD a, float &r, float &g, float &b);
void RGB2HSL(QRgb colorrgb,float *h,float *s,float *l);
void HSL2RGB(float hh,float ss,float ll,WORD a,QRgb *colorrgb);

static float CalPowl( float SourceL)
{
	float powl = 0;
	if ( SourceL - 50.0f > 0 )
		powl = pow( SourceL - 50.0f, 0.88f );
	else
		powl = -pow( 50.0f - SourceL, 0.88f );
	return powl;
}

void RGB2HSL( QRgb colorrgb,float *h,float *s,float *l )
{
	float r = GetRedValue(colorrgb) / 255.0f;
	float g = GetGreenValue( colorrgb ) / 255.0f;
	float b = GetBlueValue( colorrgb ) / 255.0f;
	RGBF2HSL( r, g, b, h, s, l );
}

void HSL2RGB( float hh,float ss,float ll,WORD a, QRgb *colorrgb )
{
	float R, G, B;
	HSL2RGBF( hh, ss, ll, a, R, G, B );
	R = R*255.0f;   

	G = G*255.0f;   

	B = B*255.0f;   
	*colorrgb = ( static_cast<BYTE>(a)<<24 ) |
		( static_cast<BYTE>((R>255)? 255 : ((R<0)?0 : R))) |
		( static_cast<BYTE>((G>255)? 255 : ((G<0)?0 : G))<<8 ) |
		( static_cast<BYTE>((B>255)? 255 : ((B<0)?0 : B))<<16 );
}

void RGBF2HSL( float r, float g, float b, float *h,float *s,float *l )
{
	double w = 0.00001f;


	float maxVal = max3v(r, g, b);
	float minVal = min3v(r, g, b);

	if( h != NULL )
	{
		float hh  = 0;
		if(fabs(maxVal - minVal) <= w)
		{
			hh = 0;
		}

		else if(fabs(maxVal - r) <= w && g >= b )
		{
			hh =  60.0f*(g-b)/(maxVal-minVal); 
		}
		else if(fabs(maxVal - r) <= w && g < b)
		{
			hh = 60.0f * (g - b) / (maxVal - minVal) + 360.0f;
		}

		else if(fabs(maxVal - g) <= w)
		{
			hh = 60.0f*(b-r)/(maxVal-minVal) + 120.0f;   
		}

		else if(fabs(maxVal - b) <= w)
		{
			hh = 60.0f*(r-g)/(maxVal-minVal) + 240.0f;
		}
		*h = (hh>360)? 360 : ((hh<0)?0:hh);    
	}
	if( l != NULL)
	{
		float ll = 0;
		ll = (maxVal+minVal)/2.0f;

		*l = ((ll>1)? 1 : ((ll<0)?0:ll))*100;   
		if( s != NULL )
		{
			float ss = 0;
			if(ll <= w || fabs(maxVal - minVal) <= w )
			{
				ss = 0;
			}

			else if( 0 < ll && ll <= 0.5f)
			{
				ss = (maxVal-minVal)/(maxVal+minVal);   
			}

			else if( ll > 0.5f)
			{
				ss = (maxVal-minVal)/(2 - (maxVal+minVal));
			}

			*s = ((ss>1)? 1 : ((ss<0)?0:(ss)))*100; 
		}
	}
}

void HSL2RGBF( float hh,float ss,float ll,WORD a, float &r, float &g, float &b )
{
	float h = hh;                  // h must be [0, 360]   

	float s = ss / 100.f; // s must be [0, 1]   

	float l = ll / 100.f;      // l must be [0, 1]   




	//if(ss == 0)   

	//{   

	//	// achromatic color (gray scale)   

	//	r = g = b = l*255.f;   

	//}   

	//else  

	{   

		float q = (l<0.5f)?(l * (1.0f+s)):(l+s - (l*s));   

		float p = (2.0f * l) - q;   



		float Hk = h/360.0f;   

		float T[3];   

		T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0   

		T[1] = Hk;              // Tb   

		T[2] = Hk - 0.3333333f; // Tg   



		for(int i=0; i<3; i++)   

		{   

			if(T[i] < 0) T[i] += 1.0f;   

			if(T[i] > 1) T[i] -= 1.0f;   



			if((T[i]*6) < 1)   

			{   

				T[i] = p + ((q-p)*6.0f*T[i]);   

			}   

			else if((T[i]*2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5   

			{   

				T[i] = q;   

			}   

			else if((T[i]*3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)   

			{   

				T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;   

			}   

			else T[i] = p;   

		}   



		r = T[0];   

		g = T[1];   

		b = T[2];   

	}   

}


void Rgb2Hsl( QRgb colorrgb,float *h,float *s,float *l )
{
	RGB2HSL(colorrgb, h, s, l);
}

void Hsl2Rgb( float hh,float ss,float ll,WORD a,QRgb *colorrgb )
{
	HSL2RGB(hh, ss, ll, a, colorrgb);
}

QRgb CalMergeColor( QRgb Source, float Powl, float SelectH, float SelectS, float SelectL, int MainH, int MainS, int MainL )
{
	QRgb DColor;
	float l;
	float s;
	float h;
	
/*
	if(!(Source & 0xFF000000))
		return Source;
*/

	Rgb2Hsl( Source, &h, &s, &l );
	float DL, DS, DH;
	if( l >= 100.0f || l <= 0.0f )
	{
		DColor = Source;
		return DColor;
	}
	if ( MainS == -1 && MainH == -1 && MainL == -1 )
	{
		DL = Powl + l;
		DS = SelectS;
		DH = SelectH;
	}
	else
	{
		DL = Powl + l;
		DS = SelectS + ( s - MainS );
		DH = SelectH + ( h - MainH );
	}
	Hsl2Rgb( DH, DS, DL, (Source)>>24, &DColor );

	return DColor;
}

QRgb MergeClr( QRgb SrcClr, QRgb chgClr, int MainH /*= -1*/, int MainS /*= -1*/, int MainL /*= - 1 */ )
{
	float Hselect, Sselect, Lselect;
	Rgb2Hsl( chgClr, &Hselect, &Sselect, &Lselect );
	float powl = CalPowl(Lselect);

	return CalMergeColor( SrcClr, powl, Hselect, Sselect, Lselect, MainH, MainS, MainL );
}

QImage MergeImage( QImage srcImg, QRgb chgClr, int MainH /*= -1*/, int MainS /*= -1*/, int MainL /*= - 1*/ )
{
	if(srcImg.isNull())
		return srcImg;
	QImage dstImg = srcImg.convertToFormat(QImage::Format_ARGB32);
	float Hselect, Sselect, Lselect;
	Rgb2Hsl( chgClr, &Hselect, &Sselect, &Lselect );
	float powl = CalPowl( Lselect );
	for(int h = 0; h < dstImg.height(); h++)
	{
		for(int w = 0; w < dstImg.width(); w++)
		{
			QRgb srcclr = dstImg.pixel(w, h);
			QRgb dstclr = CalMergeColor(srcclr, powl, Hselect, Sselect, Lselect, MainH, MainS, MainL);
			dstImg.setPixel(w, h, dstclr);
		}
	}
	return dstImg;
}

QImage CreateColorTable( int width, int height, float s )
{
	QImage hslImage(width, height, QImage::Format_RGB32);
	UpdateColorTable(hslImage, s);
	return hslImage;
}

void UpdateColorTable( QImage& img, float s )
{
	int width = img.width();
	int height = img.height();
	UINT *pPixel = (UINT*)img.bits();
	UINT stride = img.bytesPerLine() * 8 / img.depth();
	QRgb ref = 0;
	float yP = (float)height / 100.0f;
	float xP = (float)width / 360.0f;
	for ( int j = 0; j < height; ++j )
	{
		UINT *pLine = pPixel + j * stride;
		for ( int i = 0; i < width; ++i )
		{
			HSL2RGB( ((float)i)/xP, s, ((float)j)/yP, 0, &ref );
			pLine[ i ] = ref | 0xFF000000;
		}
	}
}

QImage CreateColorBar( int width, int height, float h, float l )
{
	QImage hslImage(width, height, QImage::Format_RGB32);
	UpdateColorBar(hslImage, h, l);
	return hslImage;
}

void UpdateColorBar( QImage& img, float h, float l )
{
	int width = img.width();
	int height = img.height();
	UINT *pPixel = (UINT*)img.bits();
	UINT stride = img.bytesPerLine() * 8 / img.depth();
	float xP = (float)width / 100.0f;
	QRgb ref;
	for ( int i = 0; i < width; ++i )
	{
		HSL2RGB( h, ((float)i)/xP, l, 0, &ref );
		for ( int j = 0; j < height; ++j )
		{
			pPixel[ j * stride + i ] = ref | 0xFF000000;
		}
	}
}

}


namespace KRGB
{

float CalPowl( float SourceL)
{
	float powl = 0;
	if ( SourceL - 255.0f > 0 )
		powl = pow( SourceL - 255.0f, 0.88f );
	else
		powl = -pow( 255.0f - SourceL, 0.88f );
	return powl;
}

void grayscale(const QImage &image, QImage &dest, const QRect& rect = QRect())
{
	QRect destRect = rect;
	QRect srcRect = rect;
	if (rect.isNull()) {
		srcRect = dest.rect();
		destRect = dest.rect();
	}
	if (&image != &dest) {
		destRect.moveTo(QPoint(0, 0));
	}

	unsigned int *data = (unsigned int *)image.bits();
	unsigned int *outData = (unsigned int *)dest.bits();

	if (dest.size() == image.size() && image.rect() == srcRect) {
		// a bit faster loop for grayscaling everything
		int pixels = dest.width() * dest.height();
		for (int i = 0; i < pixels; ++i) {
			int val = qGray(data[i]);
			outData[i] = qRgba(val, val, val, qAlpha(data[i]));
		}
	} else {
		int yd = destRect.top();
		for (int y = srcRect.top(); y <= srcRect.bottom() && y < image.height(); y++) {
			data = (unsigned int*)image.scanLine(y);
			outData = (unsigned int*)dest.scanLine(yd++);
			int xd = destRect.left();
			for (int x = srcRect.left(); x <= srcRect.right() && x < image.width(); x++) {
				int val = qGray(data[x]);
				outData[xd++] = qRgba(val, val, val, qAlpha(data[x]));
			}
		}
	}
}

QRgb MergeClr( QRgb SrcClr, QRgb chgClr )
{
	int gray = qGray(SrcClr);
	return SrcClr;
}

QImage MergeImage( QImage srcImg, QRgb chgClr )
{
	QImage dstImg = srcImg;
	QPainter destPainter(&dstImg);
	grayscale(srcImg, dstImg, srcImg.rect());
	destPainter.setCompositionMode(QPainter::CompositionMode_Screen);
	QColor clr(chgClr);
	//int a = qGray(chgClr);
	//int a2 = 255 * (1.0 - (a - 128)/128);
	//clr.setAlpha(qAbs<int>(a2));
	destPainter.fillRect(srcImg.rect(), clr);
	destPainter.end();
	return dstImg;
}

}
