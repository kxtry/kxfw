#include "stdafx.h"
#include "kresource.h"
#include "kjson.h"
#include "ktheme.h"
#include "xmlParser.h"
#include "kutility.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	KWindow *window = KXmlUI::windowFromFile(":/layout/Resources/uishower.xml");
	window->show();
	return a.exec();
}