#pragma once
#include <windows.h>
#include <tchar.h>


#include "UIlib.h"

#include <memory>
#include <mutex>
#include <vector>

#include <future>

#include "../tdcommon.h"
#include "Control\UIDateTime.h"

using namespace std;
using namespace DuiLib;



class MainDlg : public DuiLib::WindowImplBase
{
public:
	MainDlg();
	~MainDlg();

	void InitWindow();
	LPCTSTR GetWindowClassName() const { return _T("TDEDMAIN_COMIITID"); }
	virtual DuiLib::CDuiString GetSkinFolder(){ return _T("skin"); }
	virtual DuiLib::CDuiString GetSkinFile(){ return _T("main\\skin.xml"); }
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass) override;
	
	void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
	LRESULT  OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)override;

private:
	bool OnSubItemMouseEvent(void* wparam);
	bool OnSubItemReportMouseEvent(void* wparam);
	DuiLib::CContainerUI *CreateSoftItem();
	
	//屏蔽基础键 esc等
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled) override;
	

private:


	//设置登录用户名
	void SetLoginUser(const std::wstring& userName, const std::wstring& userPassword);
	//重置用户密码
	void SetCurPassword(const std::wstring newPassword);
	void UserLoginStatus(const bool& loginSucess);
	//void getExpireTime();//获取账号到期时间

	//void SetExpireTimeCallBack(const std::function<void(const std::wstring&, const std::wstring&)>& setExpireTime);
	//void SetExpireTime(const std::wstring& currentTime, const std::wstring& expireTime);

	void testDemo();
	
private:
	
	DuiLib::CDialogBuilder m_builder;
	DuiLib::CVerticalLayoutUI *m_pTaskItem = NULL;
	DuiLib::CButtonUI* _pButtonAddTask;


	DuiLib::CLabelUI*  _pLabelCount; //任务总数
	int    nMax_; //执行任务中最大数量
	int	   nSuccentTotal_;  //每天成功任务数
	std::wstring m_UserName;

	DuiLib::CDateTimeUI* m_pStartTime;
	DuiLib::CDateTimeUI* m_pEndTime;
private:

	//TdKioServer m_kioserver;
	//TdCameraCapture m_cameracapture;

	void ParseConfig();
	void threadReport();
	bool reportData(std::string sData);

	void reportDetail(int offset);
	void reportWorkNum(int offset);
private:
	bool m_bMenuFirst;
	DUI_DECLARE_MESSAGE_MAP()
	
};

