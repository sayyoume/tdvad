#include "MainDlg.h"

#include <functional>
#include "..\curl\curl.h"
#include "..\base\StringConvertion.h"
#include <iosfwd>
#include "..\base\json\reader.h"
#include "..\base\json\writer.h"
#include <fstream>
#include "..\base\data.h"
#include <Shlwapi.h>
#include "userLoginView.h"

#define  TEST 0
using namespace std;
using namespace std::placeholders;
//请求软件到期时间的回调
std::function<void(const std::wstring&, const std::wstring&)> _pExpireTime;



#define HOUSE 0
#define MIN 10

#define MAXTASK 12
#define SHOWINDOW 1  //1显示 0隐藏

MainDlg::MainDlg()
{
	m_bMenuFirst = false;
}

MainDlg::~MainDlg()
{
	curl_global_cleanup();
}

DUI_BEGIN_MESSAGE_MAP(MainDlg, WindowImplBase)
DUI_END_MESSAGE_MAP()


void MainDlg::ParseConfig()
{

	TDData::Instance().m_station1 = TDData::Instance().getConfigStringA("station", "station1");
	TDData::Instance().m_station2 = TDData::Instance().getConfigStringA("station", "station2");
	TDData::Instance().m_station3 = TDData::Instance().getConfigStringA("station", "station3");
	TDData::Instance().m_station4 = TDData::Instance().getConfigStringA("station", "station4");
	
}



static size_t OnCallBackAllData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	char* pData = (char*)buffer;
	return nmemb;
}

bool MainDlg::reportData(std::string sData)
{
	std::wstring sUrl = TDData::Instance().getConfigString(L"SQLSERVER", L"url");
	bool bRet = true;
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		struct curl_slist *headers = NULL;


		headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnCallBackAllData);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sData.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			bRet = false;
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return bRet;
}



void MainDlg::InitWindow()
{
	//读取配置文件信息
	ParseConfig();

	//设置pdf默认路径
	std::wstring wsIniPdfPath = TDData::Instance().getConfigString(L"config", L"pdfpath");
	if (wsIniPdfPath.empty()) {
		std::wstring pdfPath = TDData::Instance().GetCurrentPath() + L"\\TestReport";
		CreateDirectory(pdfPath.c_str(), NULL);
		TDData::Instance().writeConfigString(L"config", L"pdfpath", pdfPath.c_str());
	}


#ifdef _DEBUG
	//TDData::Instance()._sUserName = L"18910231632";
#else
	CUserLoginView* pFrame = new CUserLoginView();
	std::wstring sTitle = L"用户登录";

	HWND hwnd = nullptr;	//Parent 句柄为null，否则无法居中
	hwnd = pFrame->Create(nullptr, sTitle.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME, 0, 0, 360, 450);
	pFrame->SetLoginUserNameCallBack(std::bind(&MainDlg::SetLoginUser, this, _1, _2), std::bind(&MainDlg::UserLoginStatus, this, _1));
	pFrame->CenterWindow();
	if (pFrame->ShowModal() != IDYES) {
		::PostQuitMessage(0L);
		return;
	}
#endif // DEBUG+

	//testDemo();

	curl_global_init(CURL_GLOBAL_ALL);
	
}

std::wstring StringReplace(const std::wstring& str, const std::wstring& from,
	const std::wstring& to) {
	std::wstring result = str;
	std::wstring::size_type pos = 0;
	std::wstring::size_type from_len = from.length();
	std::wstring::size_type to_len = to.length();
	do {
		pos = result.find(from, pos);
		if (pos != std::wstring::npos) {
			result.replace(pos, from_len, to);
			pos += to_len;
		}
	} while (pos != std::wstring::npos);
	return result;
}



LRESULT MainDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM, bool & bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{

		switch (wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			bHandled = true;
			break;
		default:
			break;
		}
	}if (uMsg == WM_NCLBUTTONDBLCLK) {
		if (IsZoomed(m_hWnd)) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		bHandled = true;
	}

	return FALSE;
}





void MainDlg::SetLoginUser(const std::wstring & userName, const std::wstring & userPassword)
{
	m_UserName = userName;
}
void MainDlg::SetCurPassword(const std::wstring newPassword)
{
}
void MainDlg::UserLoginStatus(const bool & loginSucess)
{
}

//static size_t OnLastTimeData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
//{
//
//	char* pData = (char*)buffer;
//	
//	Json::Reader reader;
//	Json::Value root;
//	if (!reader.parse(pData, root))
//		return 0;
//
//	std::string sCode = root["Code"].asString();
//	std::wstring sEndTime = TDData::Instance().s2ws(root["Data"][0]["endtime"].asString());
//	std::wstring sCUrrentTime = TDData::Instance().s2ws(root["Data"][0]["currenttime"].asString());
//
//	_pExpireTime(sCUrrentTime,sEndTime);
//	return nmemb;
//}

//获取软件到期时间
//void MainDlg::getExpireTime()
//{
//	CURL *curl;
//	CURLcode res;
//	curl = curl_easy_init();
//	if (curl) {
//
//		struct curl_slist* headerlist = NULL;
//		headerlist = curl_slist_append(headerlist, "Content-Type:application/x-www-form-urlencoded");
//		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
//
//		std::string sUN = TDData::Instance().ws2s(TDData::Instance()._sUserName);
//		char buff[MAX_PATH]{};
//		sprintf_s(buff, "http://blueseo.h4321.com/getaccount?username=%s", sUN.c_str());
//
//		curl_easy_setopt(curl, CURLOPT_URL, buff);
//		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buff);
//		//curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,sizeof(buff));
//		curl_easy_setopt(curl, CURLOPT_POST, 1);
//		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
//		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
//		//curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);    // 表单
//		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnLastTimeData);
//
//		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
//		res = curl_easy_perform(curl);
//
//		long res_code = 0;
//		res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
//
//		if (res != CURLE_OK) {
//			fprintf(stderr, "curl_easy_perform() failed: %s\n",
//				curl_easy_strerror(res));
//
//		}
//		curl_easy_cleanup(curl);
//		curl_slist_free_all(headerlist);
//	}
//}


DuiLib::CControlUI * MainDlg::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

void MainDlg::Notify(DuiLib::TNotifyUI & msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_PaintManager.FindControl(L"btn_detail_close"))
		{
			int msgboxID = ::MessageBox(NULL, L"确认要退出程序", L"CRSC", MB_YESNO);
			if (msgboxID == IDYES)
			{
				Close(IDCANCEL);
			}
		}
	}
	WindowImplBase::Notify(msg);
}

LRESULT MainDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	LRESULT lRes = 0;
	switch (uMsg) {
	case WM_TIMER:	lRes = OnTimer(uMsg, wParam, lParam, bHandled);				break;
	default:
		bHandled = FALSE;
		break;
	}
	return 0;
}

LRESULT MainDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT MainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return S_OK;
}

LRESULT MainDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this))
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("CElipsePhotoUI")) != 0)
			return HTCAPTION;
	}
	return HTCLIENT;
}

DuiLib::CContainerUI * MainDlg::CreateSoftItem()
{
	return nullptr;
	//if (m_builder.GetMarkup()->IsValid())
	//	return dynamic_cast<DuiLib::CHorizontalLayoutUI*>(m_builder.Create(&m_callBack, &m_PaintManager));

	//return dynamic_cast<DuiLib::CHorizontalLayoutUI*>(m_builder.Create(_T("main\\worktick_item.xml"), (UINT)0, &m_callBack, &m_PaintManager));
}



bool MainDlg::OnSubItemMouseEvent(void* wparam)
{
	DuiLib::TEventUI* pEvent = (DuiLib::TEventUI*)wparam;
	if (pEvent == NULL) return false;

	DuiLib::CHorizontalLayoutUI* pItem = dynamic_cast<DuiLib::CHorizontalLayoutUI*>(pEvent->pSender);
	if (pItem == NULL) return false;

	
	DuiLib::CControlUI *pItemTen = static_cast<DuiLib::CControlUI*>(pItem->FindSubControl(_T("item_first")));

	if (pEvent->Type == DuiLib::UIEVENT_MOUSEENTER)
	{
		pItemTen->SetBkColor(0xff39ca2);
		pItem->SetBkColor(0xff0f1630);
	}
	if (pEvent->Type == DuiLib::UIEVENT_MOUSELEAVE)
	{
		RECT rcItem = pItem->GetPos();
		if (!PtInRect(&rcItem, pEvent->ptMouse))
		{
			pItem->SetBkColor(0xff151e3e);
			pItemTen->SetBkColor(0xff151e3e);
		}
	}

	return true;
}

bool MainDlg::OnSubItemReportMouseEvent(void* wparam)
{
	DuiLib::TEventUI* pEvent = (DuiLib::TEventUI*)wparam;
	if (pEvent == NULL) return false;

	DuiLib::CHorizontalLayoutUI* pItem = dynamic_cast<DuiLib::CHorizontalLayoutUI*>(pEvent->pSender->GetParent());
	if (pItem == NULL) return false;


	DuiLib::CControlUI *pItemTen = static_cast<DuiLib::CControlUI*>(pItem->FindSubControl(_T("item_first")));

	if (pEvent->Type == DuiLib::UIEVENT_MOUSEENTER)
	{
		pItemTen->SetBkColor(0xff39ca2);
		pItem->SetBkColor(0xff0f1630);
	}
	if (pEvent->Type == DuiLib::UIEVENT_MOUSELEAVE)
	{
		RECT rcItem = pItem->GetPos();
		if (!PtInRect(&rcItem, pEvent->ptMouse))
		{
			pItem->SetBkColor(0xff151e3e);
			pItemTen->SetBkColor(0xff151e3e);
		}
	}

	return true;
}
