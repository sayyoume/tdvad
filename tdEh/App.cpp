#include "view/MainDlg.h"
#include "resource.h"
#include "preaddlib.h"
#include "curl/curl.h"
HWND g_hWnd = NULL;

#include <crtdbg.h>
#include <stdlib.h>  
#include "base/data.h"
#include <shlwapi.h>
#include "base/minidump.h"
#include <memory>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

int numScreen = 0;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	static BOOL first = TRUE;   //��־

	//������ʾ����Ϣ
	MONITORINFO monitorinfo;
	monitorinfo.cbSize = sizeof(MONITORINFO);

	//�����ʾ����Ϣ������Ϣ���浽monitorinfo��
	GetMonitorInfo(hMonitor, &monitorinfo);

	//����⵽����
	if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
	{
		if (first)  //��һ�μ�⵽����
		{
			first = FALSE;
			numScreen = 1;
			TDData::Instance().mapScreen[0] = monitorinfo.rcMonitor;
			//����ʾ���ķֱ�����Ϣ���浽rect
			//rect[0] = monitorinfo.rcMonitor;
			return TRUE;

		}
		else //�ڶ��μ�⵽����,˵�����еļ��������Ѿ������һ�飬�ʿ���ֹͣ�����
		{
			first = TRUE;    //��־��λ
			return FALSE;    //�������
		}
	}
	TDData::Instance().mapScreen[numScreen] = monitorinfo.rcMonitor;
	//rect[numScreen] = monitorinfo.rcMonitor;
	numScreen++;
	return TRUE;
}
time_t StringToDatetime(string str)
{
	tm tm_;
	int year, month, day, hour, minute, second;
	sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;

	time_t t_ = mktime(&tm_); //�Ѿ�����8��ʱ��
	return t_; //��ʱ��
}
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"{C0E690BB-A69B-42A1-8CD7-CE664232C35459}");
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		CloseHandle(hMutex);
		hMutex = NULL;
		return -1;
	}

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpstrCmdLine);
	
	//��ʼ��dump
	InitMinDump();
	

	TDData::Instance().m_Path = TDData::Instance().GetCurrentPath();

	TDData::Instance().m_sIniPath = TDData::Instance().m_Path + L"\\config.ini";

	::CoInitialize(NULL);

	ULONG_PTR pGDIToken = NULL;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&pGDIToken, &gdiplusStartupInput, NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	
	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	TDData::Instance().m_Path = szPath;


	std::shared_ptr<MainDlg>  spFrame = make_shared<MainDlg>();

	if (spFrame == nullptr) return 0;

	spFrame->Create(NULL, L"TDEG", WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, 0, 0, 1080, 720);
	spFrame->SetIcon(IDI_ICON1);
	/*if (pFrame->GetProductionType() == 0)
		pFrame->SetIcon(IDI_ICON1);
	else
		pFrame->SetIcon(IDI_ICON2);*/

	spFrame->ShowWindow(true);
	spFrame->CenterWindow();


	_CrtDumpMemoryLeaks();
	DuiLib::CPaintManagerUI::MessageLoop();


	::CoUninitialize();
	GdiplusShutdown(pGDIToken);
	
	return 0;
}
