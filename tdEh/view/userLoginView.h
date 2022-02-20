#pragma once
#include "UIlib.h"
#include "functional"
#include "Control\UIEdit.h"

using namespace DuiLib;
class CLoginUserListView;
class CUserLoginView : public DuiLib::WindowImplBase
{
public:
	CUserLoginView();
	~CUserLoginView();

public:
	void InitWindow();
	LPCTSTR GetWindowClassName() const { return _T("RatChetSetPassWord"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND hWnd) { delete this; };
	virtual DuiLib::CDuiString GetSkinFolder(){ return _T("skin"); }
	virtual DuiLib::CDuiString GetSkinFile(){ return _T("login\\loginwindow.xml"); }
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass) override;
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void Notify(DuiLib::TNotifyUI& msg);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	void PopTheListView(bool bInit = false);
	void SetAppServiceIPort();
	void SetUserNameAndPassword(std::wstring& userName, std::wstring& passWord);
	//int PostSet(appServerInterface type, const Json::Value& jsonRequest, bool& loginStatus);
	int CheckLoginState(const std::wstring& userName, bool& loginStatus);
	bool UserLoginRequest(const std::wstring& userName, const std::wstring& passWord);
	void SetLoginUserNameCallBack(const std::function<void(const std::wstring&, const std::wstring&)>& setLoginUserName, const std::function<void(const bool&)>& setLoginStatus);
	void Logout(const std::wstring& userName);
private:
	bool OnEditPasswordGetFocus(void *param);
	bool OnEditUserNameGetFocus(void *param);
	bool OnEditUserNameMouseEvent(void* wparam);		//鼠标响应
	void OnClickRememberMe(TNotifyUI& msg);
	void OnClickForgetPassword(TNotifyUI& msg);
	void OnClickShow(TNotifyUI& msg);
private:
	void loginIn();
private:
	bool m_bShowList;
	bool m_bRememberMe;
	DuiLib::CEditUI*	m_pPosswordEdit;//密码设置框
	DuiLib::CEditUI*	m_pUserNameEdit;//用户名
	DuiLib::CEditUI*    m_pErrorWarringInf;
	DuiLib::CControlUI* m_pLayoutErrorWarring;
	//DuiLib::CControlUI* m_pLayoutColour;
	DuiLib::CControlUI* m_pUserNameOutLine;
	CLoginUserListView* m_pFrame;
	CCheckBoxUI*  m_pCheckBox;
	std::function<void(const bool&)> m_pSetLoginStatus;
	std::function<void(const std::wstring&, const std::wstring&)> m_pSetLoginUserName;
public:


private:
	DUI_DECLARE_MESSAGE_MAP()
};
