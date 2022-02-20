#include "userLoginView.h"
#include "..\base\md5\MD5.h"
#include "..\base\data.h"
//#include "..\curl\easy.h"
#include "..\curl\curl.h"
#include "..\base\StringConvertion.h"
#include "..\base\json\reader.h"
DUI_BEGIN_MESSAGE_MAP(CUserLoginView, WindowImplBase)
DUI_ON_CLICK_CTRNAME(_T("btn_rememberme"), OnClickRememberMe)
DUI_ON_CLICK_CTRNAME(_T("btn_forgetpassword"), OnClickForgetPassword)
DUI_ON_CLICK_CTRNAME(_T("btn_loginshow"), OnClickShow)
DUI_END_MESSAGE_MAP()

CUserLoginView::CUserLoginView() : m_bRememberMe(false)
, m_pFrame(nullptr)
, m_bShowList(false)
{
}

CUserLoginView::~CUserLoginView()
{
}

void CUserLoginView::InitWindow()
{
	m_pUserNameEdit = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(_T("loginname")));
	if (m_pUserNameEdit)
		m_pUserNameEdit->OnNotify += DuiLib::MakeDelegate(this, &CUserLoginView::OnEditUserNameGetFocus);
	//m_pUserNameEdit->OnEvent += DuiLib::MakeDelegate(this, &CUserLoginView::OnEditUserNameMouseEvent);

	m_pPosswordEdit = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(_T("login_userpassword")));
	if (m_pPosswordEdit)
		m_pPosswordEdit->OnNotify += DuiLib::MakeDelegate(this, &CUserLoginView::OnEditPasswordGetFocus);
	m_pPosswordEdit->SetPasswordMode(true);

	//m_pLayoutColour = m_PaintManager.FindControl(L"loginwarningver");
	m_pLayoutErrorWarring = m_PaintManager.FindControl(L"loginwarninghor");
	m_pLayoutErrorWarring->SetVisible(false);
	m_pUserNameOutLine = m_PaintManager.FindControl(L"outlineborder");
	m_pErrorWarringInf = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl(_T("describtion")));

	m_pCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"btn_rememberme"));
	if (m_pCheckBox != nullptr)
	{
		//std::wstring szUser = SGREGOPT()->RegReadSZ(HKEY_USERS, REG_RATCHET_LOGIN_PATH, REG_AUTO_LOGIN_FLAG);
		//m_bRememberMe = (szUser.length() > 0) ? true : false;
		//m_pCheckBox->SetCheck(m_bRememberMe);
	}
	//如果自动登录就自动设置用户名和密码
	if (m_bRememberMe)
	{
		m_pPosswordEdit->SetAttribute(L"bkimage", L"");
		PopTheListView(true);
	}
}

bool CUserLoginView::OnEditPasswordGetFocus(void *param)
{
	DuiLib::TNotifyUI* pMsg = (DuiLib::TNotifyUI*)param;
	if (pMsg->sType == DUI_MSGTYPE_SETFOCUS)
	{
		//m_pPosswordEdit->SetAttribute(L"bkimage", L"");
		
		m_pLayoutErrorWarring->SetVisible(false);
		//m_pPosswordEdit->SetAttribute(L"bkcolor", L"0xff33323e");
		//m_pPosswordEdit->Invalidate();
		return false;
	}
	if (pMsg->sType != DUI_MSGTYPE_TEXTCHANGED)
		return false;
	return true;
}

bool CUserLoginView::OnEditUserNameGetFocus(void *param)
{
	DuiLib::TNotifyUI* pMsg = (DuiLib::TNotifyUI*)param;
	if (pMsg->sType == DUI_MSGTYPE_SETFOCUS)
	{
		/*	if (m_pPosswordEdit->GetText() == _T(""))
				m_pPosswordEdit->SetAttribute(L"bkimage", L"file=\'login\\inputpassword.png\' dest=\'4,6,83,22\'");*/
			
		m_pLayoutErrorWarring->SetVisible(false);
		//m_pLayoutColour->SetAttribute(L"bkcolor", L"0xffffffff");
		//弹出下拉框
		//PopTheListView();
	}
	if (pMsg->sType != DUI_MSGTYPE_TEXTCHANGED)
		return false;
	return true;
}

bool CUserLoginView::OnEditUserNameMouseEvent(void *param)
{
	DuiLib::TEventUI* pEvent = (DuiLib::TEventUI*)param;
	if (pEvent == NULL) return false;

	return true;
}

void CUserLoginView::PopTheListView(bool bInit)
{
	if (!m_pFrame)
	{
		//m_pFrame = new CLoginUserListView();
		/*std::wstring sTitle = L"";
		HWND hwnd = m_pFrame->Create(m_hWnd, sTitle.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME, 0, 0, 242, 30);
		m_pFrame->SetUserNamePasswordCallBack(std::bind(&CUserLoginView::SetUserNameAndPassword, this, placeholders::_1, placeholders::_2));
		m_pFrame->SetDefaultNameAndPwd();*/
	}

	RECT userNamePos = m_pUserNameOutLine->GetPos();
	POINT it_pos;
	it_pos.x = userNamePos.left;
	it_pos.y = userNamePos.bottom;
	::ClientToScreen(m_hWnd, &it_pos);
	//m_bShowList = m_pFrame->SetListPos(it_pos.x, it_pos.y);
	//如果是第一次初始时
	if (bInit)
	{
		//m_pFrame->ShowWindow(false);
	}
	else
	{
		//m_pFrame->ShowWindow(m_bShowList);
	}
}

DuiLib::CControlUI * CUserLoginView::CreateControl(LPCTSTR pstrClass)
{
	return  nullptr;// m_callBack.CreateControl(pstrClass);
}

LRESULT CUserLoginView::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_ESCAPE: {
			bHandled = true;
			break;
		}
		case VK_RETURN: {
			loginIn();
		}
			bHandled = true;
			break;
		default:
			break;
		}
	}
	return FALSE;
}


void CUserLoginView::loginIn()
{
	if (m_pUserNameEdit->GetText() == L"")
	{
		m_pErrorWarringInf->SetText(L"用户名不可以为空");
		m_pLayoutErrorWarring->SetVisible(true);
		//m_pLayoutColour->SetAttribute(L"bkcolor", L"0xffff6666");
		return;
	}

	if (m_pPosswordEdit->GetText() == L"")
	{
		m_pErrorWarringInf->SetText(L"密码不可以为空");
		m_pPosswordEdit->SetText(_T(""));
		m_pLayoutErrorWarring->SetVisible(true);
		//m_pLayoutColour->SetAttribute(L"bkcolor", L"0xffff6666");
		return;
	}

	std::wstring username = m_pUserNameEdit->GetText().GetData();
	std::wstring password = m_pPosswordEdit->GetText().GetData();

	bool bLogin = UserLoginRequest(username, password);
	if (bLogin)
	{
		
		TDData::Instance()._sUserName = username;
		//::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		Close(IDYES);
	}
	else
	{
		m_pLayoutErrorWarring->SetVisible(true);
		m_pErrorWarringInf->SetText(L"请检查用户名或密码是否正确");
		//m_pLayoutColour->SetAttribute(L"bkcolor", L"0xffff6666");
		return;
	}

	if (m_bRememberMe)
		//将用户名密码存入注册表中（后期应该只将用户名存入注册表，密码根据用户名去服务器请求）
	{
		//std::wstring szName = m_pUserNameEdit->GetText();
		//std::wstring szPwd = m_pPosswordEdit->GetText();
		//SGREGOPT()->RegWriteSZ(HKEY_USERS, REG_RATCHET_USER_PATH, szName, szPwd);
		//SGREGOPT()->RegWriteSZ(HKEY_USERS, REG_RATCHET_LOGIN_PATH, REG_AUTO_LOGIN_FLAG, szName);
	}
	else
	{
		//SGREGOPT()->RegWriteSZ(HKEY_USERS, REG_RATCHET_LOGIN_PATH, REG_AUTO_LOGIN_FLAG, L"");
	}
	m_pSetLoginUserName(username, password);
	m_pSetLoginStatus(true);

}



void CUserLoginView::Notify(DuiLib::TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_PaintManager.FindControl(L"closelogin_btn"))
		{
			m_pSetLoginStatus(false);
			Close(IDOK);
			::PostQuitMessage(0L);
			
			return;
		}
		else if (msg.pSender == m_PaintManager.FindControl(L"btn_loginok"))
		{
			loginIn();
		}
	}
	//else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
	//{
	//	if (msg.pSender == m_PaintManager.FindControl(L"login_userpassword"))
	//	{
	//		/*if (m_pPosswordEdit->GetText() == _T(""))
	//			m_pPosswordEdit->SetAttribute(L"bkimage", L"file=\'RatChet\\login\\inputpassword.png\' dest=\'4,6,83,22\'");*/
	//	}
	//	
	//}
	//
	WindowImplBase::Notify(msg);
}

LRESULT CUserLoginView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return S_OK;
}

LRESULT CUserLoginView::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	LRESULT lRes = 0;

	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:
		bHandled = TRUE;
		break;
	case WM_MOVING:
	{
		//if (m_pFrame)
			//m_pFrame->ShowWindow(false);
		break;
	}
	default:
		bHandled = FALSE;
	}
	return 0;
}

void CUserLoginView::OnClickRememberMe(TNotifyUI& msg)
{
	//获取的是单击之前的状态
	if (m_pCheckBox->GetCheck())
	{
		//清除注册表中对应的用户名和密码
		m_bRememberMe = false;
	}
	else
	{
		//将用户名和密码写入注册表
		m_bRememberMe = true;
	}
}

void CUserLoginView::SetUserNameAndPassword(std::wstring& userName, std::wstring& passWord)
{
	m_pUserNameEdit->SetText(userName.c_str());
	m_pPosswordEdit->SetText(passWord.c_str());
	m_PaintManager.FindControl(L"btn_loginok")->SetFocus();
}

void CUserLoginView::OnClickForgetPassword(TNotifyUI& msg)
{
	MessageBox(NULL, L"请联系管理员", L"操作", MB_OK);
	//DuilibEx::CUgMessagebox::Msgbox(m_hWnd, L"请联系管理员", _T("操作提示"), DuilibEx::DUI_MB_OK, DuilibEx::DUI_MSGBOX_FIRST);
	return;
	/*std::wstring userName = m_pUserNameEdit->GetText();
	if (userName.empty())
	{
		DuilibEx::CUgMessagebox::Msgbox(m_hWnd, L"请输入用户名", _T("操作提示"), DuilibEx::DUI_MB_OK, DuilibEx::DUI_MSGBOX_FIRST);
		return;
	}
		
	CForgetPasswordView* pFrame = new CForgetPasswordView(userName);
	if (pFrame == NULL) return;
	std::wstring sTitle = L"忘记密码";
	pFrame->Create(m_hWnd, sTitle.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME, 0, 0, 450, 300);
	pFrame->CenterWindow();
	pFrame->ShowModal();
	::ShowWindow(*pFrame, SW_SHOW);*/
}
void CUserLoginView::OnClickShow(TNotifyUI & msg)
{
	//m_pPosswordEdit->SetPassWord(!m_pPosswordEdit->IsPassWord());
	m_pPosswordEdit->SetPasswordMode(!m_pPosswordEdit->IsPasswordMode());
}
//
//int CUserLoginView::PostSet(appServerInterface type, const Json::Value& jsonRequest, bool& loginStatus)
//{
//	std::string strResponse;
//
//	Json::FastWriter writer;
//	std::string strRequestSub = writer.write(jsonRequest);
//	SetAppServiceIPort();
//	LOG(INFO) << "CUserLoginView::PostSet " << std::endl;
//	return AppService::instance()->postToServiceForLogin(type, strRequestSub, -1, strResponse, loginStatus);
//}

int CUserLoginView::CheckLoginState(const std::wstring& userName, bool& loginStatus)
{
	/*Json::Value jsonBody;

	jsonBody["username"] = SGSTROPT()->Wstr2Str(userName, CPUTF8);

	return PostSet(LAWCLIENT_GETLOGINSTATEBYUSERNAME, jsonBody, loginStatus);*/
	return 0;
}

void CUserLoginView::Logout(const std::wstring& userName)
{
	/*bool loginStatus = false;
	Json::Value jsonBody;
	jsonBody["username"] = SGSTROPT()->Wstr2Str(userName, CPUTF8);
	PostSet(LAWCLIENT_EXITLOGIN, jsonBody, loginStatus);*/
}
static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	//std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	//if (NULL == str || NULL == buffer)
	//{
		//return -1;
	//}

	const char* pData = (char*)buffer;
	//str->append(pData, size * nmemb);

	//std::string sData;
	//sData.assign(pData, size*nmemb);

	//char*
	//strcpy(sData.c_str(), pData);


	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(pData, root))
		return 0;

	std::string sCode =root["Code"].asString();
	TDData::Instance().sCode = sCode;
	return nmemb;
}
bool CUserLoginView::UserLoginRequest(const std::wstring& userName, const std::wstring& passWord)
{
	return true;// return DbManager::Instance().loginUser(TDData::Instance().ws2s(userName), TDData::Instance().ws2s(passWord));
}

void CUserLoginView::SetLoginUserNameCallBack(const std::function<void(const std::wstring&, const std::wstring&)>& setLoginUserName, const std::function<void(const bool&)>& setLoginStatus)
{
	m_pSetLoginUserName = setLoginUserName;
	m_pSetLoginStatus = setLoginStatus;
}

void CUserLoginView::SetAppServiceIPort()
{
	/*std::wstring wstrIP = Rconfig::Instance().getLoginIP();
	std::string strIP = SGSTROPT()->Wstr2Str(wstrIP);

	int nPort = Rconfig::Instance().getLoginPort();
	AppService::instance()->setAppServiceServer(strIP, (unsigned short)nPort);*/
}
