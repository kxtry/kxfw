#include "stdafx.h"
#include "kxmlui.h"
#include "kresource.h"
#include "khelper.h"
#include "kwidget.h"
#include "kwindow.h"
#include "kxfw_version.h"
#include "xmlParser.h"

#include <QDateTime>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


#define INVOKE_FUNCTION_PROPERTY	("function")
#define INVOKE_ARGUMENT_PROPERTY	("arguments")
#define INVOKE_CONNECT_PROPERTY		("connect")

#define OBJECT_ARGUMENT_PROPERTY	("objectArgument")

#define EVENT_OBJECT_PROPERTY		("object")
#define EVENT_SIGNAL_PROPERTY		("signal")
#define EVENT_SLOT_PROPERTY			("slot")
#define EVENT_CONNECT_PROPERTY		("connect")

#define DATA_FUNCTION_PROPERTY		("function")
#define DATA_MODEL_PROPERTY			("model")
#define DATA_CONNECT_PROPERTY		("connect")

typedef QVariant (*PFUNObjectPropertyCreator)(const QString &value);
typedef QHash<int, PFUNObjectPropertyCreator> KPropertyHash;

typedef QHash<QLatin1String, KHelperCreatorBase*> KHelperHash;
typedef QHash<QLatin1String, KWidgetCreatorBase*> KWidgetHash;
typedef QHash<QLatin1String, KWindowCreatorBase*> KWindowHash;

struct SkinData
{
	QMetaObject *metaobj;//安全检查
	QMap<QByteArray,QVariant> skinvar;//当skinvar有值时，代表所有skinraw已经转换成skinvar了。
	QMap<QByteArray,QByteArray> skinraw;//刚开始时，由于类型无法及时转换成QVariant估直接保存了XML中值。
};

typedef QHash<QString,SkinData> KSkinHash;

Q_GLOBAL_STATIC(KHelperHash, _helperHash);
Q_GLOBAL_STATIC(KWidgetHash, _widgetHash);
Q_GLOBAL_STATIC(KWindowHash, _windowHash)
Q_GLOBAL_STATIC(KPropertyHash, _propertyHash)
Q_GLOBAL_STATIC(KSkinHash, _skinHash)

KHelperHash *helperHash()
{
	return _helperHash();
}

KWidgetHash *widgetHash()
{
	return _widgetHash();
}

KWindowHash *windowHash()
{
	return _windowHash();
}

KPropertyHash *propertyHash()
{
	return _propertyHash();
}

KSkinHash *skinHash()
{
	return _skinHash();
}

KHelperCreatorBase::KHelperCreatorBase( const char* objectClassName ) : m_className(objectClassName)
{
	helperHash()->insert(m_className, this);
}

KHelperCreatorBase::~KHelperCreatorBase()
{
	helperHash()->remove(m_className);
}

const char* KHelperCreatorBase::className() const
{
	return m_className.latin1();
}

KWidgetCreatorBase::KWidgetCreatorBase( const char* widgetClassName ) : m_className(widgetClassName)
{
	widgetHash()->insert(m_className, this);
}

KWidgetCreatorBase::~KWidgetCreatorBase()
{
	widgetHash()->remove(m_className);
}

const char* KWidgetCreatorBase::className() const
{
	return m_className.latin1();
}

KWindowCreatorBase::KWindowCreatorBase( const char* windowClassName ) : m_className(windowClassName)
{
	windowHash()->insert(m_className, this);
}

KWindowCreatorBase::~KWindowCreatorBase()
{
	windowHash()->remove(m_className);
}

const char* KWindowCreatorBase::className() const
{
	return m_className.latin1();
}


Q_GLOBAL_STATIC(KMethodInvokerOrg, _methodInvoker)
KMethodInvokerOrg *methodInvoker()
{
	return _methodInvoker();
}


template <typename type>
class PropertyConvertor
{
public:
	PropertyConvertor(const char* name, PFUNObjectPropertyCreator fun)
	{
		type_id = QVariant::nameToType(name);
		if(type_id == QVariant::UserType)
		{
			type_id = QMetaType::type(name);
		}
		else if(type_id == QVariant::Invalid)
		{
			type_id = qRegisterMetaType<type>(name);
		}
		propertyHash()->insert(type_id, fun);
	}

	~PropertyConvertor()
	{
		propertyHash()->remove(type_id);
	}

private:
	int type_id;
};


#define KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(type, fun)	\
	static PropertyConvertor<type> k_property_convertor_global_static##type(#type, fun);


/*                    ArgumentConvert                                   */

QGenericArgument variant2Argument( const char* typeName, int type_id, QVariant& v )
{
	void *dataPtr = v.data();
	if(type_id == QMetaType::QObjectStar || type_id == QMetaType::QWidgetStar || type_id == QMetaType::VoidStar)
	{
		dataPtr = v.data();
	}
	else if(type_id == QMetaType::QVariant)
	{
		dataPtr = &v;
	}
	else if(type_id < (int)QVariant::UserType && !v.convert(QVariant::Type(type_id)))
	{
		/*转换失败。*/
		return QGenericArgument();
	}
	return QGenericArgument(typeName, dataPtr);
}

/*                     PropertyConvert		                            */

static QVariant string2SizeF(const QString& value);
static QVariant string2Size(const QString& value);
static QVariant stringToFont(const QString& value);
static QVariant string2PointF(const QString& value);
static QVariant string2Point(const QString& value);
static QVariant stringToMargin(const QString& value);
static QVariant string2Color(const QString& value);
static QVariant string2Anchor(const QString& value);
static QVariant string2Rect(const QString& value);
static QVariant string2RectF(const QString& value);
static QVariant stringToQtGradient(const QString& value);
static QVariant stringToConicalQtGradient(const QString& value);
static QVariant stringToLinearQtGradient(const QString& value);
static QVariant stringToRadialQtGradient(const QString& value);
static QVariant stringToBrush(const QString& value);


static QVariant string2PointF(const QString& value)
{
	QStringList s = value.split(',');
	QPointF siz;
	if (s.size() >= 2)
	{
		siz.setX(s.at(0).toFloat());
		siz.setY(s.at(1).toFloat());
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad QPointF format had inputed, check against. orginal string:%1").arg(value).toStdString().c_str());
	}

	return siz;
}

static QVariant string2Point(const QString& value)
{
	return string2PointF(value).toPoint();
}

static QVariant string2SizeF(const QString& value)
{
	QStringList s = value.split(',');
	QSizeF siz;
	if (s.size() >= 2)
	{
		siz.setWidth(s.at(0).toFloat());
		siz.setHeight(s.at(1).toFloat());
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad QSizeF format had inputed, check against. orginal string:%1").arg(value).toStdString().c_str());
	}

	return siz;
}

static QVariant string2Size(const QString& value)
{
	return string2SizeF(value).toSize();
}

static QVariant stringToFont(const QString& value)
{
	//return qStringToFont(value);
	QFont f;
	f.fromString(value);
	return f;
}

static QVariant stringToMargin(const QString& value)
{
	QStringList s = value.split(',');
	if (s.size() >= 4)
	{
		QMargins r(s.at(0).toFloat(), s.at(1).toFloat(), s.at(2).toFloat(), s.at(3).toFloat());
		return QVariant::fromValue(r);
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad QMargins format had inputed, check against. orginal string:%1").arg(value).toStdString().c_str());
		return QVariant::fromValue(QMargins()); //unreachable
	}
}

static QVariant stringToRadialQtGradient(const QString& val)
{
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Pad
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Repeat
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|angle:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	if(val.indexOf("Radius") >= 0 && val.indexOf("Center") >= 0)
	{
		QStringList sl = val.split('|');
		QtGradient radial;
		radial.t = QGradient::RadialGradient;
		for(int i = 0; i < sl.count(); i++)
		{
			QStringList scl = sl.at(i).split(':');
			if(scl.at(0).compare("Center") == 0)
			{
				QVariant pt = string2PointF(scl.at(1));
				radial.d.r.centerx = pt.toPointF().x();
				radial.d.r.centery = pt.toPointF().y();
			}
			else if(scl.at(0).compare("Radius") == 0)
			{
				radial.d.r.radius = scl.at(1).toFloat();
			}
			else if(scl.at(0).compare("Color") == 0)
			{
				qreal pos = scl.at(1).toFloat();
				QVariant clr = string2Color(scl.at(2));
				radial.gs.push_back(QPair<qreal,QColor>(pos, clr.value<QColor>()));
			}
			else if(scl.at(0).compare("Spread") == 0)
			{
				if(scl.at(1) == "Pad")
				{
					radial.spread = QGradient::PadSpread;
				}
				else if(scl.at(1) == "Repeat")
				{
					radial.spread = QGradient::RepeatSpread;
				}
				else if(scl.at(1) == "Reflect")
				{
					radial.spread = QGradient::ReflectSpread;
				}
			}
		}
		return QVariant::fromValue(radial);
	}
	return QVariant::fromValue(QtGradient());
}


static QVariant stringToConicalQtGradient(const QString& val)
{
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Pad
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Repeat
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|angle:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	if(val.indexOf("Angle") >= 0 && val.indexOf("Center") >= 0)
	{
		QStringList sl = val.split('|');
		QtGradient radial;
		radial.t = QGradient::ConicalGradient;
		for(int i = 0; i < sl.count(); i++)
		{
			QStringList scl = sl.at(i).split(':');
			if(scl.at(0).compare("Center") == 0)
			{
				QVariant pt = string2PointF(scl.at(1));
				radial.d.c.centerx = pt.toPointF().x();
				radial.d.c.centery = pt.toPointF().y();
			}
			else if(scl.at(0).compare("Angle") == 0)
			{
				radial.d.c.angle = scl.at(1).toFloat();
			}
			else if(scl.at(0).compare("Color") == 0)
			{
				qreal pos = scl.at(1).toFloat();
				QVariant clr = string2Color(scl.at(2));
				radial.gs.push_back(QPair<qreal,QColor>(pos, clr.value<QColor>()));
			}
			else if(scl.at(0).compare("Spread") == 0)
			{
				if(scl.at(1) == "Pad")
				{
					radial.spread = QGradient::PadSpread;
				}
				else if(scl.at(1) == "Repeat")
				{
					radial.spread = QGradient::RepeatSpread;
				}
				else if(scl.at(1) == "Reflect")
				{
					radial.spread = QGradient::ReflectSpread;
				}
			}
		}
		return QVariant::fromValue(radial);
	}

	return QVariant::fromValue(QtGradient());
}

static QVariant stringToLinearQtGradient(const QString& val)
{
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Pad
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Repeat
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|angle:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	if(val.indexOf("Start") >= 0 && val.indexOf("Final") >= 0)
	{
		QStringList sl = val.split('|');
		QtGradient radial;
		radial.t = QGradient::LinearGradient;
		for(int i = 0; i < sl.count(); i++)
		{
			QStringList scl = sl.at(i).split(':');
			if(scl.at(0).compare("Start") == 0)
			{
				QVariant pt = string2PointF(scl.at(1));
				radial.d.l.startx = pt.toPointF().x();
				radial.d.l.starty = pt.toPointF().y();
			}
			else if(scl.at(0).compare("Final") == 0)
			{
				QVariant pt = string2PointF(scl.at(1));
				radial.d.l.finalx = pt.toPointF().x();
				radial.d.l.finaly = pt.toPointF().y();
			}
			else if(scl.at(0).compare("Color") == 0)
			{
				qreal pos = scl.at(1).toFloat();
				QVariant clr = string2Color(scl.at(2));
				radial.gs.push_back(QPair<qreal,QColor>(pos, clr.value<QColor>()));
			}
			else if(scl.at(0).compare("Spread") == 0)
			{
				if(scl.at(1) == "Pad")
				{
					radial.spread = QGradient::PadSpread;
				}
				else if(scl.at(1) == "Repeat")
				{
					radial.spread = QGradient::RepeatSpread;
				}
				else if(scl.at(1) == "Reflect")
				{
					radial.spread = QGradient::ReflectSpread;
				}
			}
		}
		return QVariant::fromValue(radial);
	}

	return QVariant::fromValue(QtGradient());
}

static QVariant stringToQtGradient(const QString& val)
{
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Pad
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Repeat
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|angle:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	if(val.indexOf("Radius") >= 0)
	{
		return stringToRadialQtGradient(val);
	}
	else if(val.indexOf("Angle") >= 0)
	{
		return stringToConicalQtGradient(val);
	}
	else if(val.indexOf("Final") >= 0)
	{
		return stringToLinearQtGradient(val);
	}

	return QVariant::fromValue(QtGradient());
}

static QVariant stringToBrush(const QString& val)
{
	QBrush brush;
	return QVariant::fromValue(brush);
}

static QVariant translateString(const QString& value)
{
	return QVariant::fromValue(QObject::tr(value.toUtf8()));
}

static QVariant string2Color(const QString& value)
{
	QColor color;
	if(value.at(0) == '#')
	{
		if(value.length() > 7)
		{
			QByteArray val = value.toAscii();
			QByteArray name = val.mid(0, 7);
			QByteArray alpha = val.mid(7, 2);
			color.setNamedColor(name.data());
			int a = alpha.toInt(NULL, 16);
			color.setAlpha(a);
		}
		else
		{
			color.setNamedColor(value);
		}
		return color;
	}
	QStringList s = value.split(',');
	if (s.size() >= 3)
	{
		int red = s[0].toInt();
		int green = s[1].toInt();
		int blue = s[2].toInt();
		int alpha = 255;
		if (s.size() >= 4)
		{
			alpha = s[3].toInt();
		}
		color.setRgb(red, green, blue, alpha);
		return color;
	}
	else if(s.size() > 1)
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("bad QColor format had inputed, check against. orginal string:%1").arg(value).toStdString().c_str());
	}
	else
	{
		color.setNamedColor(value);
	}

	return color;
}

QVariant string2RectF( const QString &value )
{
	QStringList vals = value.split(',');
	if(vals.count() != 4)
		return QVariant();
	QRectF rt;
	rt.setRect(vals.at(0).toFloat(),vals.at(1).toFloat(),vals.at(2).toFloat(),vals.at(3).toFloat());
	return QVariant(rt);
}

QVariant string2Rect( const QString &value )
{
	return string2RectF(value).toRect();
}

QVariant setDefaultValue( const QString &value )
{
	return value;
}

//arguments="(int)3,(int)3"
void argumentsFromText( QList<QByteArray>& argvs, QList<int>& lstType, QList<QVariant>& lstVars, QList<QGenericArgument> &lstArgv )
{
	int count = 0;
	for(int i = 0; i < argvs.count(); i++, count++)
	{
		QByteArray& item = argvs[i];
		if(item.isEmpty())
		{
			break;
		}
		int left = item.lastIndexOf('(');
		Q_ASSERT_X(left >= 0, __FUNCTION__, "can't find the left bracket");
		item[left] = '\0';
		char *typeName = item.data();
		int right = item.lastIndexOf(')');
		Q_ASSERT_X(right >= 0, __FUNCTION__, "can't find the right bracket");
		item[right] = '\0';
		char *argvTxt = typeName + left + 1;
		int type_id = QVariant::nameToType(typeName);
		if(type_id == QVariant::UserType)
		{
			type_id = QMetaType::type(typeName);
		}
		lstType.push_back(type_id);
		PFUNObjectPropertyCreator creator = propertyHash()->value(type_id);
		if(creator == NULL)
		{
			creator = setDefaultValue;
		}
		lstVars.push_back(creator(argvTxt));
		lstArgv.push_back(variant2Argument(typeName, type_id, lstVars.back()));
	}

	for(int i = count; i < 10; i++)
	{
		lstArgv.push_back(QGenericArgument());
	}
}


Q_DECLARE_METATYPE(QMargins);
Q_DECLARE_METATYPE(QtGradient);

KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QSizeF, string2SizeF);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QSize, string2Size);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QFont, stringToFont);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QPointF, string2PointF);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QPoint, string2Point);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QMargins, stringToMargin);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QColor, string2Color);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QRect, string2Rect);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QRectF, string2RectF);
KX_PROPERTY_CONVERTOR_GLOBAL_STATIC(QtGradient, stringToQtGradient);




/*                     KXmlUI                                        */

bool KXmlUI::widgetFromFile( const char* file, KWidget *parent )
{
	XMLNode xml;
	if (!KResource::loadXml(file, xml))
	{
		Q_ASSERT_X(false, __FUNCTION__, "xml file error!");
		return false;
	}
	methodInvoker()->clearAll();
	bool bOk = createUITreeFromXml(xml, parent);
	if(bOk)
	{
		executeUIHelperFromXml(xml, parent);
	}
	methodInvoker()->execute();
	return bOk;
}

bool KXmlUI::widgetFromFile( const QString& file, KWidget *parent /*= NULL*/ )
{
	return widgetFromFile(file.toStdString().data(), parent);
}

bool KXmlUI::widgetFromMemory( const QByteArray& memory, KWidget *parent /*= NULL */ )
{
	XMLNode xml;
	XMLResults err;
	xml = XMLNode::parseString(memory.data(), NULL, &err);
	if(err.error == eXMLErrorNone)
		return false;
	methodInvoker()->clearAll();
	bool bOk = createUITreeFromXml(xml, parent);
	if(bOk)
	{
		executeUIHelperFromXml(xml, parent);
	}
	methodInvoker()->execute();
	return bOk;
}

KWindow* KXmlUI::windowFromFile( const char* file, QWidget *parent )
{
	XMLNode xml;
	if (!KResource::loadXml(file, xml))
	{
		Q_ASSERT_X(false, __FUNCTION__, "xml file error!");
		return false;
	}
	methodInvoker()->clearAll();
	KWindow *window = createUITreeFromXml(xml, parent);
	if(window)
	{
		executeUIHelperFromXml(xml, window);
	}
	methodInvoker()->execute();
	return window;
}

KWindow* KXmlUI::windowFromFile( const QString& file, QWidget *parent /*= NULL*/ )
{
	return windowFromFile(file.toStdString().data(), parent);
}

KWindow* KXmlUI::windowFromMemory( const QByteArray& memory, QWidget *parent /*= NULL*/ )
{
	XMLNode xml;
	XMLResults err;
	xml = XMLNode::parseString(memory.data(), NULL, &err);
	if(err.error == eXMLErrorNone)
		return NULL;
	methodInvoker()->clearAll();
	KWindow *window = createUITreeFromXml(xml, parent);
	if(window)
	{
		executeUIHelperFromXml(xml, window);
	}
	methodInvoker()->execute();
	return window;
}

bool KXmlUI::createUITreeFromXml( XMLNode& xml, KWidget *parent )
{
	XMLNode child = xml.getChildNodeByPath("uitree");

	if (child.isEmpty())
	{
		Q_ASSERT_X(false, __FUNCTION__, "has no ui child.");
		return false;
	}

	int count = child.nChildNode();
	for (int index = 0; index < count; ++index)
	{
		createChild(child.getChildNode(index), parent);
	}
	return true;
}

KWindow* KXmlUI::createUITreeFromXml( XMLNode& xml, QWidget *parent )
{
	XMLNode child = xml.getChildNodeByPath("uitree");

	if (child.isEmpty())
	{
		Q_ASSERT_X(false, __FUNCTION__, "has no uitree child.");
		return false;
	}

	int count = child.nChildNode();
	Q_ASSERT_X(count == 1, __FUNCTION__, "more than one child.");
	XMLNode topNode = child.getChildNode(0);
	const char *className = topNode.getName();
	KWindow *window = createWindow(className, parent);
	if(window == NULL)
		return NULL;
	setProperties(topNode, window);
	KWidget *rootWidget = window->rootWidget();
	count = topNode.nChildNode();
	for (int index = 0; index < count; ++index)
	{
		createChild(topNode.getChildNode(index), rootWidget);
	}
	window->construct();
	return window;
}

bool KXmlUI::createChild( XMLNode& child, KWidget *parent )
{
	const char *className = child.getName();
	KWidget *widget = createWidget(className, parent);
	if(widget == NULL)
		return false;
	parent->addItem(widget);
	setProperties(child, widget);
	int count = child.nChildNode();
	KWidget *pWidget = widget->layoutWidget();
	for (int index = 0; index < count; ++index)
	{
		createElements(pWidget, child.getChildNode(index));
	}
	widget->construct();
	return true;
}

void KXmlUI::setProperties( XMLNode& child, QObject* item )
{
	QMap<QByteArray, QObject*> itemMap;
	int count = child.nAttribute();
	for (int index = 0; index < count; ++index)
	{
		QByteArray attrName(child.getAttributeName(index));
		if (attrName.isEmpty())
		{
			Q_ASSERT_X(false, __FUNCTION__, "attr name is null");
			continue;
		}
		const char *attrValue = child.getAttributeValue(index);
		int dotIndex = attrName.lastIndexOf('.');
		if(dotIndex == -1)
		{
			setProperty(item, attrName, attrValue);
		}
		else
		{
			/*查找出内置对象*/
			QList<QByteArray> extAtrs = attrName.split('.');
			attrName[dotIndex] = '\0';
			char *objName = attrName.data();
			char *objAttr = objName + dotIndex + 1;
			QObject *objExt = itemMap.value(objName);
			if(objExt == NULL)
			{
				QObject *itemOwner = item;
				for(int i = 0; i < extAtrs.count() - 1; i++)
				{
					QVariant varExt = itemOwner->property(extAtrs[i].data());
					objExt = varExt.value<QObject*>();
					Q_ASSERT_X(objExt, __FUNCTION__, QString("no such property:%1.%2").arg(objName).arg(objAttr).toStdString().c_str());
					if(objExt == NULL)
						break;
					itemOwner = objExt;
				}
			}
			if(objExt == NULL)
				continue;
			setProperty(objExt, objAttr, attrValue);
			itemMap[objName] = objExt;
		}
	}
}

QVariant KXmlUI::setProperty( QObject *item, const char* attrName, const char* attrValue )
{
	if(item == NULL)
		return QVariant();
	QString objName = item->objectName();
	int idx = item->metaObject()->indexOfProperty(attrName);
	if(idx == -1)
	{
		if(strcmp(attrName, OBJECT_ARGUMENT_PROPERTY) == 0)
		{
			methodInvoker()->addObject(objName, item);
			return QVariant();
		}
		//当作附加属性记录下来。
		QVariant val(attrValue);
		item->setProperty(attrName, val);
		return val;
	}
	Q_ASSERT_X(strcmp(attrName, OBJECT_ARGUMENT_PROPERTY), __FUNCTION__, QString("%1 has the same attribute named of %2").arg(objName).arg(attrName).toStdString().c_str());
	QMetaProperty prop = item->metaObject()->property(idx);
	int type_id = prop.type();
	if(type_id == QVariant::UserType)
	{
		type_id = prop.userType();
	}
	PFUNObjectPropertyCreator creator = propertyHash()->value(type_id);
	if(creator == NULL)
	{
		creator = setDefaultValue;
	}

	QVariant val = creator(attrValue);
	bool b = item->setProperty(attrName, val);
	Q_ASSERT_X(b, __FUNCTION__, QString("setProperty %1 failed for bad value. orginal string:%2").arg(attrName).arg(attrValue).toStdString().c_str());
	return val;
}

KWindow * KXmlUI::createWindow( const char* className, QWidget *parent )
{
	KWindowHash::iterator iter = windowHash()->find(QLatin1String(className));
	if(iter == windowHash()->end())
		return NULL;
	KWindowCreatorBase *windowCreator = iter.value();
	return windowCreator->create(parent);
}

KWidget * KXmlUI::createWidget( const char *className, KWidget *parent )
{
	KWidgetHash::iterator iter = widgetHash()->find(QLatin1String(className));
	if(iter == widgetHash()->end())
		return NULL;
	KWidgetCreatorBase *widgetCreator = iter.value();
	return widgetCreator->create(parent);
}

bool KXmlUI::setDefaultDataProperty( QObject *widget, XMLNode child )
{
	int count = child.nChildNode();
	QList<QVariant> mapList;
	for (int i = 0; i < count; ++i)
	{
		int attrCount = child.getChildNode(i).nAttribute();
		QMap<QString, QVariant> m;
		const XMLNode elem = child.getChildNode(i);
		for (int j = 0; j < attrCount; j++)
		{
			m.insert(elem.getAttributeName(j), elem.getAttributeValue(j));
		}
		mapList.push_back(m);
	}
	return widget->setProperty("data", mapList);
}

void KXmlUI::createElements( KWidget *item, XMLNode& node )
{
	QByteArray tagName(node.getName());
	int index = tagName.indexOf(':');
	if(index == -1)
	{
		createChild(node, item);
	}
	else
	{
		const char *szType = tagName.data() + index + 1;
		Q_ASSERT_X(szType[0] != ':', __FUNCTION__, QString("it's : not ::[%1]").arg(tagName.data()).toStdString().c_str());
		if(strcmp(szType, "data") == 0)
		{
			setData(item, node);
		}
		else if(strcmp(szType, "event") == 0)
		{
			connectSlot(item, node);
		}
		else if(strcmp(szType, "invoke") == 0)
		{
			invokeMethod(item, node);
		}
	}
	
}

void KXmlUI::connectSlot( QObject *obj, XMLNode& node )
{
	QString sender(node.getAttribute(EVENT_OBJECT_PROPERTY));
	QByteArray signal("2");
	QByteArray slot("1");
	Qt::ConnectionType connType = Qt::AutoConnection;
	signal.append(node.getAttribute(EVENT_SIGNAL_PROPERTY));
	slot.append(node.getAttribute(EVENT_SLOT_PROPERTY));
	const char* attrValue = node.getAttribute(EVENT_CONNECT_PROPERTY);
	if(attrValue == NULL)
	{
		connType = Qt::AutoConnection;
	}
	if(strcmp(attrValue, "auto") == 0)
	{
		connType = Qt::AutoConnection;
	}
	else if(strcmp(attrValue, "direct") == 0)
	{
		connType = Qt::DirectConnection;
	}
	else if(strcmp(attrValue, "queue") == 0)
	{
		connType = Qt::QueuedConnection;
	}
	else 
	{
		connType = Qt::AutoConnection;
	}
	methodInvoker()->addReceiver(obj, sender, signal, slot, connType);
}

void KXmlUI::invokeMethod( QObject *obj, XMLNode& node )
{
	QByteArray invoke;
	QByteArray argv;
	Qt::ConnectionType connType = Qt::AutoConnection;
	invoke.append(node.getAttribute(INVOKE_FUNCTION_PROPERTY));
	argv.append(node.getAttribute(INVOKE_ARGUMENT_PROPERTY));
	const char* connValue = node.getAttribute(INVOKE_CONNECT_PROPERTY);
	if(connValue == NULL)
	{
		connType = Qt::AutoConnection;
	}
	else if(strcmp(connValue, "auto") == 0)
	{
		connType = Qt::AutoConnection;
	}
	else if(strcmp(connValue, "direct") == 0)
	{
		connType = Qt::DirectConnection;
	}
	else if(strcmp(connValue, "queue") == 0)
	{
		connType = Qt::QueuedConnection;
	}
	else 
	{
		connType = Qt::AutoConnection;
	}
	methodInvoker()->addMethod(obj, invoke, argv, connType);
}

void KXmlUI::setData( KWidget *obj, XMLNode& node )
{
	const char *invoke = node.getAttribute(DATA_FUNCTION_PROPERTY);
	const char *model = node.getAttribute(DATA_MODEL_PROPERTY);
	Qt::ConnectionType connType = Qt::AutoConnection;
	const char* connValue = node.getAttribute(DATA_CONNECT_PROPERTY);
	if(invoke == NULL)
	{
		invoke = "setData";
	}
	if(connValue == NULL)
	{
		connType = Qt::AutoConnection;
	}
	else if(strcmp(connValue, "auto") == 0)
	{
		connType = Qt::AutoConnection;
	}
	else if(strcmp(connValue, "direct") == 0)
	{
		connType = Qt::DirectConnection;
	}
	else if(strcmp(connValue, "queue") == 0)
	{
		connType = Qt::QueuedConnection;
	}
	else 
	{
		connType = Qt::AutoConnection;
	}
	if(model == NULL || strcmp(model, "xml") == 0)
	{
		QMetaObject::invokeMethod(obj, invoke, connType, Q_ARG(const XMLNode*, &node));
	}
	else if(strcmp(model, "variant") == 0)
	{
		Q_ASSERT_X(node.nChildNode(), __FUNCTION__, "it failed to set data for bad child");
		XMLNode &dataNode = node.getChildNode(0);
		const char *dataType = dataNode.getName();
		QList<QVariant> tree;
		buildTreeData(tree, node);
		QMetaObject::invokeMethod(obj, invoke, connType, Q_ARG(QVariant, QVariant(tree)));
	}
}

bool KXmlUI::buildTreeData( QList<QVariant>& list, XMLNode& node )
{
	int count = node.nChildNode();
	if (count == 0)
	{
		return false;
	}

	for (int i = 0; i < count; ++i)
	{
		XMLNode &child = node.getChildNode(i);
		int attrCount = child.nAttribute();
		QMap<QString, QVariant> tag;
		QMap<QString, QVariant> m;
		for (int j = 0; j < attrCount; j++)
		{
			m.insert(child.getAttributeName(j), child.getAttributeValue(j));
		}
		tag.insert(child.getName(), m);
		QList<QVariant> childList;
		if (buildTreeData(childList, child))
		{
			m.insert("children", childList);
		}

		list.append(tag);
	}
	return true;
}

bool KXmlUI::executeUIHelperFromXml( XMLNode& xml, QObject *parent )
{
	XMLNode child = xml.getChildNodeByPath("uihelper");

	if (child.isEmpty())
	{
		return false;
	}

	int count = child.nChildNode();
	for (int index = 0; index < count; ++index)
	{
		executeHelper(child.getChildNode(index), parent, parent);
	}
	return true;
}

bool KXmlUI::executeHelper( XMLNode& child, QObject *parent, QObject *root )
{
	const char *className = child.getName();
	KHelper *helper = createHelper(className, parent);
	if(helper == NULL)
		return false;
	setProperties(child, helper);
	helper->setRootObject(root);
	if(helper->execute())
	{
		int count = child.nChildNode();
		for (int index = 0; index < count; ++index)
		{
			executeHelper(child.getChildNode(index), helper, helper->targetObject());
		}
	}

	delete helper;
	return true;
}

KHelper* KXmlUI::createHelper( const char *className, QObject *parent )
{
	KHelperHash::iterator iter = helperHash()->find(QLatin1String(className));
	if(iter == helperHash()->end())
		return NULL;
	KHelperCreatorBase *helperCreator = iter.value();
	return helperCreator->create(parent);
}


/*
优先使用当前XML文件中的公共私有同名属性，然后再使用全局公共私有属性。
这要求先返回当前公共属性成员QMap<QString,QString>,然后再用当前复盖相同的。

使用QMap<QByteArray,QByteArray> 可能效率会更高。
虽然通过该方法使内存增大了，但把公共相同的东西，放到同一处查询，使得XML文件更小，最终效率也提升了。维护也变得更容易。
*/

bool KXmlUI::initSkin( const char* file )
{
	XMLNode xml;
	if (!KResource::loadXml(file, xml))
	{
		Q_ASSERT_X(false, __FUNCTION__, "xml file error!");
		return false;
	}

	return initSkin(xml);
}

bool KXmlUI::initSkin( const QByteArray& memory )
{
	XMLNode xml;
	XMLResults err;

	xml = XMLNode::parseString(memory.data(), NULL, &err);
	if(err.error == eXMLErrorNone)
		return false;

	return initSkin(xml);
}

bool KXmlUI::initSkin( XMLNode& root )
{
	XMLNode object = root.getChildNodeByPath("uiskin");

	if (object.isEmpty())
	{
		Q_ASSERT_X(false, __FUNCTION__, "has no skin object.");
		return false;
	}
	skinHash()->clear();
	return createSkinPool(object);
}

void KXmlUI::setSkin( QObject *obj, const QString& name )
{
	KSkinHash *skin = skinHash();
	KSkinHash::Iterator iter = skin->find(name);
	if(iter == skin->end())
		return;
	SkinData &sd = iter.value();
	QMetaObject *mo = (QMetaObject*)obj->metaObject();
	while(mo != sd.metaobj)
	{
		mo = (QMetaObject*)mo->superClass();
		if(mo == NULL)
		{
			/*说明不是该类也不是派生关系。*/
			return;
		}
	}
	//说明是该类或派生。
	QMap<QByteArray, QObject*> itemMap;
	if(!sd.skinvar.isEmpty())
	{
		for(QMap<QByteArray,QVariant>::Iterator iter = sd.skinvar.begin(); iter != sd.skinvar.end(); iter++)
		{
			QByteArray attrName = iter.key();
			if(attrName.isEmpty())
			{
				Q_ASSERT_X(false, __FUNCTION__, "attr name is null");
				continue;
			}
			QVariant attrValue = iter.value();
			int dotIndex = attrName.lastIndexOf('.');
			if(dotIndex == -1)
			{
				obj->setProperty(attrName, attrValue);
			}
			else
			{
				/*查找出内置对象*/
				QList<QByteArray> extAtrs = attrName.split('.');
				attrName[dotIndex] = '\0';
				char *objName = attrName.data();
				char *objAttr = objName + dotIndex + 1;
				QObject *objExt = itemMap.value(objName);
				if(objExt == NULL)
				{
					QObject *itemOwner = obj;
					for(int i = 0; i < extAtrs.count() - 1; i++)
					{
						QVariant varExt = itemOwner->property(extAtrs[i].data());
						objExt = varExt.value<QObject*>();
						Q_ASSERT_X(objExt, __FUNCTION__, QString("bad property:%1.%2").arg(objName).arg(objAttr).toStdString().c_str());
						if(objExt == NULL)
							break;
						itemOwner = objExt;
					}
				}
				if(objExt == NULL)
					continue;
				bool b = objExt->setProperty(objAttr, attrValue);
				itemMap[objName] = objExt;
			}
		}
	}
	else
	{
		for(QMap<QByteArray,QByteArray>::Iterator iter = sd.skinraw.begin(); iter != sd.skinraw.end(); iter++)
		{
			QByteArray attrName = iter.key();
			QByteArray attrNameOrg = attrName;
			if(attrName.isEmpty())
			{
				Q_ASSERT_X(false, __FUNCTION__, "attr name is null");
				continue;
			}
			QByteArray attrValue = iter.value();
			int dotIndex = attrName.lastIndexOf('.');
			if(dotIndex == -1)
			{
				QVariant val = setProperty(obj, attrName, attrValue.data());
				if(val.isValid())
				{
					sd.skinvar.insert(attrNameOrg, val);
				}
			}
			else
			{
				/*查找出内置对象*/
				QList<QByteArray> extAtrs = attrName.split('.');
				attrName[dotIndex] = '\0';
				char *objName = attrName.data();
				char *objAttr = objName + dotIndex + 1;
				QObject *objExt = itemMap.value(objName);
				if(objExt == NULL)
				{
					QObject *itemOwner = obj;
					for(int i = 0; i < extAtrs.count() - 1; i++)
					{
						QVariant varExt = itemOwner->property(extAtrs[i].data());
						objExt = varExt.value<QObject*>();
						Q_ASSERT_X(objExt, __FUNCTION__, QString("bad property:%1.%2").arg(objName).arg(objAttr).toStdString().c_str());
						if(objExt == NULL)
							break;
						itemOwner = objExt;
					}
				}
				if(objExt == NULL)
					continue;
				QVariant val = setProperty(objExt, objAttr, attrValue.data());
				if(val.isValid())
				{
					sd.skinvar.insert(attrNameOrg, val);
				}
				itemMap[objName] = objExt;
			}
		}
		sd.skinraw.clear();
	}
}

bool KXmlUI::createSkinPool( XMLNode& xml )
{
	KSkinHash *skin = skinHash();

	int count = xml.nChildNode();
	for (int index = 0; index < count; index++)
	{
		XMLNode node = xml.getChildNode(index);
		int attrCount = node.nAttribute();
		const char *className = node.getName();
		QMetaObject *metaobj = getMetaObject(className);
		if(metaobj == NULL)
			continue;
		SkinData sd;
		sd.metaobj = metaobj;
		QString skinName;
		for (int iattr = 0; iattr < attrCount; iattr++)
		{
			QByteArray attrName(node.getAttributeName(iattr));
			if (attrName.isEmpty())
			{
				Q_ASSERT_X(false, __FUNCTION__, "attr name is null");
				continue;
			}
			const char *attrValue = node.getAttributeValue(iattr);
			if(attrName == "name")
			{
				skinName = attrValue;
			}
			else
			{
				sd.skinraw.insert(attrName, attrValue);
			}
		}
		if(!skinName.isEmpty())
		{
			skin->insert(skinName, sd);
		}
	}	
	return true;
}

QMetaObject * KXmlUI::getMetaObject( const char* className )
{
	KWidgetHash::iterator iter = widgetHash()->find(QLatin1String(className));
	if(iter == widgetHash()->end())
	{
		KWindowHash::Iterator iter = windowHash()->find(QLatin1String(className));
		if(iter == windowHash()->end())
			return NULL;
		KWindowCreatorBase *windowCreator = iter.value();
		return (QMetaObject*)windowCreator->metaObject();
	}
	KWidgetCreatorBase *widgetCreator = iter.value();
	return (QMetaObject*)widgetCreator->metaObject();
}

bool KXmlUI::setPropertyEx( QObject *obj, const char* name, const char* value )
{
	QByteArray attrName = name;
	QByteArray attrValue = value;
	int dotIndex = attrName.lastIndexOf('.');
	if(dotIndex == -1)
	{
		QVariant val = setProperty(obj, attrName, attrValue.data());
	}
	else
	{
		/*查找出内置对象*/
		QList<QByteArray> extAtrs = attrName.split('.');
		attrName[dotIndex] = '\0';
		char *objName = attrName.data();
		char *objAttr = objName + dotIndex + 1;
		QObject *objExt = NULL;
		if(objExt == NULL)
		{
			QObject *itemOwner = obj;
			for(int i = 0; i < extAtrs.count() - 1; i++)
			{
				QVariant varExt = itemOwner->property(extAtrs[i].data());
				objExt = varExt.value<QObject*>();
				Q_ASSERT_X(objExt, __FUNCTION__, QString("bad property:%1.%2").arg(objName).arg(objAttr).toStdString().c_str());
				if(objExt == NULL)
					break;
				itemOwner = objExt;
			}
		}
		if(objExt == NULL)
			return false;
		QVariant val = setProperty(objExt, objAttr, attrValue.data());
	}
	return true;
}

bool KXmlUI::setPropertyEx( QObject *obj, const char* name, const QVariant& val )
{
	QByteArray attrName = name;
	int dotIndex = attrName.lastIndexOf('.');
	if(dotIndex == -1)
	{
		return obj->setProperty(name, val);
	}
	else
	{
		/*查找出内置对象*/
		QList<QByteArray> extAtrs = attrName.split('.');
		attrName[dotIndex] = '\0';
		char *objName = attrName.data();
		char *objAttr = objName + dotIndex + 1;
		QObject *objExt = NULL;
		if(objExt == NULL)
		{
			QObject *itemOwner = obj;
			for(int i = 0; i < extAtrs.count() - 1; i++)
			{
				QVariant varExt = itemOwner->property(extAtrs[i].data());
				objExt = varExt.value<QObject*>();
				Q_ASSERT_X(objExt, __FUNCTION__, QString("bad property:%1.%2").arg(objName).arg(objAttr).toStdString().c_str());
				if(objExt == NULL)
					break;
				itemOwner = objExt;
			}
		}
		if(objExt == NULL)
			return false;
		return objExt->setProperty(objAttr, val);
	}
	return true;
}



/*                  KMethodInvokerOrg                             */

KMethodInvokerOrg::KMethodInvokerOrg()
{

}

KMethodInvokerOrg::~KMethodInvokerOrg()
{

}


void KMethodInvokerOrg::addReceiver( QObject* obj, const QString& sender, const QByteArray& signal, const QByteArray& slot, Qt::ConnectionType itype )
{
	EventReceiver eRecv;
	eRecv.itype = itype;
	eRecv.obj = obj;
	eRecv.sender = sender;
	eRecv.slot = slot;
	eRecv.signal = signal;
	m_lstRecv.push_back(eRecv);
}

void KMethodInvokerOrg::execute()
{
	for(int i = 0; i < m_lstRecv.count(); i++)
	{
		EventReceiver &er = m_lstRecv[i];
		QObject *objSender = m_hashObjects.value(er.sender);
		if(objSender == NULL)
		{
			continue;
		}
		QObject::connect(objSender, er.signal.data(), er.obj, er.slot.data(), er.itype);
	}
	for(int i = 0; i < m_lstMethod.count(); i++)
	{
		MethodInvoker &mi = m_lstMethod[i];
		QList<QByteArray> argvs = mi.argv.split(',');
		QList<QGenericArgument> lstArgvs;
		QList<QVariant> lstVars;
		QList<int> lstType;
		lstArgvs.reserve(10);
		argumentsFromText(argvs, lstType, lstVars, lstArgvs);
		for(int j = 0; j < lstType.count(); j++)
		{
			int type_id = lstType[j];
			if(type_id == QMetaType::QObjectStar || type_id == QMetaType::QWidgetStar)
			{
				QGenericArgument &argv = lstArgvs[j];
				QVariant &val = lstVars[j];
				QObject *objval = m_hashObjects.value(val.toString());
				lstVars[j] = QVariant::fromValue<QObject*>(objval);
				lstArgvs[j] = QGenericArgument(argv.name(), lstVars[j].data());
			}
		}
		QMetaObject::invokeMethod(mi.obj, mi.invoke, mi.itype, lstArgvs[0],lstArgvs[1],lstArgvs[2],lstArgvs[3],lstArgvs[4],lstArgvs[5],lstArgvs[6],lstArgvs[7],lstArgvs[8],lstArgvs[9]);
	}
}

void KMethodInvokerOrg::clearAll()
{
	m_hashObjects.clear();
	m_lstRecv.clear();
}

void KMethodInvokerOrg::addObject( const QString& objName, QObject *obj )
{
	m_hashObjects.insert(objName, obj);
}

void KMethodInvokerOrg::addMethod( QObject *obj, const QByteArray& invoke, const QByteArray& argv, Qt::ConnectionType itype )
{
	MethodInvoker mi;
	mi.itype = itype;
	mi.invoke = invoke;
	mi.argv = argv;
	mi.obj = obj;
	m_lstMethod.push_back(mi);
}