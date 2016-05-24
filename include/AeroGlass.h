#ifndef _AEROGLASS_H_
#define _AEROGLASS_H_

#include <dwmapi.h>
class KXFW_API CAeroGlass
{
public:
	static BOOL Create();
	static void Destroy();
	static HRESULT DwmDefWindowProc(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
	static HRESULT DwmEnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);
	static HRESULT DwmEnableComposition(UINT uCompositionAction);
	static HRESULT DwmEnableMMCSS(BOOL fEnableMMCSS);
	static HRESULT DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset);
	static HRESULT DwmGetColorizationColor(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
	static HRESULT DwmGetCompositionTimingInfo(HWND hwnd, DWM_TIMING_INFO *pTimingInfo);
	static HRESULT DwmGetWindowAttribute(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
	static HRESULT DwmIsCompositionEnabled(BOOL *pfEnabled);
	static HRESULT DwmModifyPreviousDxFrameDuration(HWND hwnd, INT cRefreshes, BOOL fRelative);
	static HRESULT DwmQueryThumbnailSourceSize(HTHUMBNAIL hThumbnail, PSIZE pSize);
	static HRESULT DwmRegisterThumbnail(HWND hwndDestination, HWND *hwndSource, PHTHUMBNAIL phThumbnailId);
	static HRESULT DwmSetDxFrameDuration(HWND hwnd, INT cRefreshes);
	static HRESULT DwmSetPresentParameters(HWND hwnd, DWM_PRESENT_PARAMETERS *pPresentParams);
	static HRESULT DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
	static HRESULT DwmUnregisterThumbnail(HTHUMBNAIL hThumbnailId);
	static HRESULT DwmUpdateThumbnailProperties(HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES *ptnProperties);
private:
	static HMODULE m_hModuleAero;
};
#endif