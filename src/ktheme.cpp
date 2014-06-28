#include "stdafx.h"
#include "ktheme.h"
#include "kresource.h"
#include "kutility.h"
#include "colorspace.h"
#include "kxmlui.h"

#include <float.h>

struct ThemeColorData 
{
	QRgb themeColor; /*主题颜色参考值*/
	QRgb textColor;/*在对应的主题色下，其对应的文字颜色值*/;
	QRgb levelColor[KTheme::LEVEL_COUNT]; 
};

ThemeColorData _themeColorData[9]=
{
	//0
	{
		qRgb(58,110,165),
		qRgb(255, 255, 255),
		{
			qRgb(1,1,1),
			qRgb(2,2,2),
			qRgb(3,3,3),
			qRgb(4,4,4),
			qRgb(0,0,0)
		}
	},
	//1
	{
		qRgb(331, 255, 168),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//2
	{
		qRgb(289, 255, 154),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//3
	{
		qRgb(41, 255, 149),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//4
	{
		qRgb(84, 180, 125),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//5
	{
		qRgb(189, 229, 148),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//6
	{
		qRgb(22, 197, 22),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//7
	{
		qRgb(22, 197, 22),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	},
	//8
	{
		qRgb(0, 0, 170),
		qRgb(255, 255, 255),
		{
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255),
			qRgb(255,255,255)
		},
	}
};

struct ThemeData
{
	QColor themeColor;//用户选中的颜色
	int themeIndex;//通过选中的颜色进行计算出相应的最近颜色值。
	
	ThemeData()
	{
		themeColor = _themeColorData[0].themeColor;
		themeIndex = 0;
	}
};

typedef QCache<QString,QPixmap> QKeyPixmap;

QString PixmapCache_pixmapCacheKey(const QString& fileName, bool bRaw);
bool PixmapCache_pixmapLoad(const QString& fileName, QPixmap& pixmap, bool bTheme, bool bCache);

bool PixmapCache_insert(QKeyPixmap *pPixmapCache, const QString& key, const QPixmap& pixmap);
void PixmapCache_remove(QKeyPixmap *pPixmapCache, const QString& key);
QPixmap PixmapCache_find(QKeyPixmap *pPixmapCache, const QString& key);
void PixmapCache_clear(QKeyPixmap *pPixmapCache);

Q_GLOBAL_STATIC(QKeyPixmap, keyRawPixmap)
Q_GLOBAL_STATIC(QKeyPixmap, keyThemePixmap)
Q_GLOBAL_STATIC(ThemeData, globalThemeData)

int KThemeChangeEvent::EventType = QEvent::registerEventType();


KThemeChangeEvent::KThemeChangeEvent()
:QEvent(QEvent::Type(EventType))
{

}

void broacastThemeChanged()
{
	QWidgetList topWidgets = qApp->topLevelWidgets();
	foreach (QWidget* widget, topWidgets)
	{
		qApp->postEvent( widget, new KThemeChangeEvent() );
	}
}


QPixmap KTheme::getPixmap( const QString& fileName, bool bTheme/*=true*/, bool bCache/*=true*/ )
{
	/*可以指定当前皮肤主题*/
	return KResource::loadPixmap(fileName, bTheme, bCache);
}

QImage KTheme::getImage( const QString& fileName )
{
	return KResource::loadImage(fileName);
}

QSize KTheme::virtualScreenSize()
{
	return KUtility::virtualScreenSize();
}

QRect KTheme::virtualScreenRect()
{
	return KUtility::virtualScreenRect();
}

QColor KTheme::themeColor()
{
	return globalThemeData()->themeColor;
}


void KTheme::setThemeIndex( int index )
{
	globalThemeData()->themeIndex = index;
	globalThemeData()->themeColor = _themeColorData[index].themeColor;
	broacastThemeChanged();
	resetTheme();
}

void KTheme::setThemeColor( const QColor& refclr )
{
	globalThemeData()->themeColor = refclr;
	globalThemeData()->themeIndex = calcNearestThemeIndex(refclr);
	broacastThemeChanged();
	resetTheme();
}

QColor KTheme::textColor()
{
	return _themeColorData[globalThemeData()->themeIndex].textColor;
}

QColor KTheme::levelColor( int level )
{
	if(level >= KTheme::LEVEL_COUNT || level < 0)
		return QColor();
	return _themeColorData[globalThemeData()->themeIndex].levelColor[level];
}

int KTheme::themeIndex()
{
	return globalThemeData()->themeIndex;
}

void KTheme::resetTheme()
{
	PixmapCache_clear(keyThemePixmap());
}

int KTheme::calcNearestThemeIndex( const QColor& refclr )
{
	double cie_l, cie_a, cie_b;
	ColorSpace::rgb2lab(&cie_l, &cie_a, &cie_b, refclr.red(), refclr.green(), refclr.blue());

	int nearestIndex = -1;
	double labDelta = DBL_MAX;

	for(int i = 0; i < KX_ARRAY_SIZE(_themeColorData); i++)
	{
		QColor baseColor = _themeColorData[i].themeColor;

		double cie_l2, cie_a2, cie_b2;
		ColorSpace::rgb2lab(&cie_l2, &cie_a2, &cie_b2, baseColor.red(), baseColor.green(), baseColor.blue());
		double delta = (cie_a2 - cie_a) * (cie_a2 - cie_a) + (cie_b2 - cie_b)  * (cie_b2 - cie_b);

		if(delta < labDelta)
		{
			labDelta = delta;
			nearestIndex = i;
		}
	}
	return nearestIndex;
}

QColor KTheme::convertToThemeColor( const QColor& clrref )
{
	QRgb srcclr = clrref.rgba();
	QRgb themeclr = themeColor().rgba();
	return KHSL::MergeClr(srcclr, themeclr);
}

QPixmap KTheme::convertToThemePixmap( const QPixmap& pixmapref )
{
	QImage srcimg = pixmapref.toImage();
	QImage dstimg = convertToThemeImage(srcimg);
	return QPixmap::fromImage(dstimg);
}

QImage KTheme::convertToThemeImage( const QImage& imgref )
{
	QRgb themeclr = themeColor().rgba();

	return KHSL::MergeImage(imgref, themeclr);
}

QBrush KTheme::convertToThemeBrush( const QBrush& brushref )
{
	switch(brushref.style())
	{
	case Qt::NoBrush:
		{
			return brushref;
		}
		break;
	case Qt::TexturePattern:
		{
			QImage img = convertToThemeImage(brushref.textureImage());
			return QBrush(img);
		}
		break;
	default:
		{
			QColor clr = brushref.color();
			if(clr.isValid())
			{
				QColor cvtColor = convertToThemeColor(clr);
				return QBrush(cvtColor);
			}
		}
		break;
	}
	return brushref;
}

QPixmap KTheme::getThemePixmap( const QString& fileName )
{
	return QPixmap();
}

QPixmap KTheme::getRawPixmap( const QString& fileName )
{
	return QPixmap();
}

void KTheme::setSkinName( QObject* obj, const QString& name )
{
	KXmlUI::setSkin(obj, name);
}

QString PixmapCache_pixmapCacheKey( const QString& fileName, bool bTheme )
{
	QFileInfo info(fileName);
	QString key = QString("kpixmap%1%2%3%4").arg(info.absoluteFilePath()).arg(info.lastModified().toTime_t()).arg(info.size()).arg(bTheme);
	return key;
}

bool PixmapCache_pixmapLoad( const QString& fileName, QPixmap& pixmap, bool bTheme, bool bCache /*= true*/ )
{
	if (fileName.isEmpty())
		return false;

	QString key = PixmapCache_pixmapCacheKey(fileName, bTheme);
	if(bTheme)
	{
		/*获取的是主题图片*/
		pixmap = PixmapCache_find(keyThemePixmap(), key);
		if(!pixmap.isNull())
			return true;
		if(PixmapCache_pixmapLoad(fileName, pixmap, false, true))
		{
			/*处理主题图片的颜色合并算法等，并放进图片缓存中。*/
			pixmap = KTheme::convertToThemePixmap(pixmap);
			if(bCache)
			{
				return PixmapCache_insert(keyThemePixmap(), key, pixmap);
			}
			return true;
		}
	}
	else
	{
		pixmap = PixmapCache_find(keyRawPixmap(), key);
		if(!pixmap.isNull())
			return true;
		QImage image = QImageReader(fileName, 0).read();
		if (image.isNull())
			return false;
		if (pixmap.convertFromImage(image)) 
		{
			if(bCache)
			{
				return PixmapCache_insert(keyRawPixmap(), key, pixmap);
			}
			return true;
		}
	}
	return false;
}

bool PixmapCache_insert(QKeyPixmap *pPixmapCache, const QString& key, const QPixmap& pixmap)
{
	return pPixmapCache->insert(key, new QPixmap(pixmap));
}

void PixmapCache_remove(QKeyPixmap *pPixmapCache, const QString& key)
{
	pPixmapCache->remove(key);
}

QPixmap PixmapCache_find(QKeyPixmap *pPixmapCache, const QString& key)
{
	QPixmap *obj = pPixmapCache->object(key);
	if(obj)
	{
		return QPixmap(*obj);		
	}
	return QPixmap();
}

void PixmapCache_clear(QKeyPixmap *pPixmapCache)
{
	pPixmapCache->clear();
}
