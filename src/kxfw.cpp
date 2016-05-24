#include "stdafx.h"
#include "kxfw.h"
#include "xmlParser.h"
#include "kutility.h"
#include "AeroGlass.h"

void initResource()
{
	Q_INIT_RESOURCE(kxfw);
}

void cleanupResource()
{
	Q_CLEANUP_RESOURCE(kxfw);
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			QFont f("SimSun");
			f.setPixelSize(12);
			QApplication::setFont(f);
			CoInitialize( NULL);
			CAeroGlass::Create();
			XMLNode::setGlobalOptions(XMLNode::char_encoding_GB2312);
			qputenv("QT_ANCHORLAYOUT_NO_SIMPLIFICATION", "1");
			QString appdir = KUtility::osExeInstancePath();
			SetEnvironmentVariableA("appdir", appdir.toStdString().c_str());
			QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			CAeroGlass::Destroy();
			CoUninitialize();
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}