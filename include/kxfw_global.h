#pragma once;


/*C Libarary*/

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

// Windows 头文件:
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>

/* Qt library */
#include <QtCore>
#include <QtGui>
#include <QtGlobal>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QMargins>
#include <QPointer>
#include <QColor>
#include <QIcon>
#include <QLatin1String>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QPair>
#include <QVariant>
#include <QPixmap>
#include <QEvent>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QFontMetrics>
#include <QFontMetricsF>
#include <QGraphicsLinearLayout>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QTextFormat>
#include <QTextOption>
#include <QTextCursor>
#include <QFontDatabase>
#include <QFrame>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QDebug>


#pragma comment(lib, "shell32.lib")

#define KX_SAFE_DELETE(p) {if (p != NULL) delete (p); p = NULL; }

#define KX_SAFE_GDIOBJ_DELETEOBJECT(obj) { if(obj) { ::DeleteObject(obj) ;obj =NULL;} }

#define KX_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#pragma warning(error: 4715)

#ifdef KXFW_LIB
# define KXFW_API Q_DECL_EXPORT
#else
# define KXFW_API Q_DECL_IMPORT
#endif


/* 本全局只适宜放置SDK的头文件，如果放KXXX的头文件，可能会产生编译	*/
/* 问题，需要谨慎。														*/


/* private */
#include "kdatadef.h"

/*
在一些特殊条下，例如IE或Flash的script回调里直接调用deleteLater，由于是该回调不是通过Qt直接调用的，
故导致其deleteLater所依赖的loopLevel恒为1，从而导致对象无法正确被释放，故需使用以下队列方式解决。
*/
inline void qDeleteLater(QObject *obj)
{
	QMetaObject::invokeMethod(obj, "deleteLater", Qt::QueuedConnection);
}
