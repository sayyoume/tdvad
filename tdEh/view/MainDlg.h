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
	
	//���λ����� esc��
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled) override;
	

private:


	//���õ�¼�û���
	void SetLoginUser(const std::wstring& userName, const std::wstring& userPassword);
	//�����û�����
	void SetCurPassword(const std::wstring newPassword);
	void UserLoginStatus(const bool& loginSucess);
	//void getExpireTime();//��ȡ�˺ŵ���ʱ��

	//void SetExpireTimeCallBack(const std::function<void(const std::wstring&, const std::wstring&)>& setExpireTime);
	//void SetExpireTime(const std::wstring& currentTime, const std::wstring& expireTime);

	void testDemo();
	
private:
	
	DuiLib::CDialogBuilder m_builder;
	DuiLib::CVerticalLayoutUI *m_pTaskItem = NULL;
	DuiLib::CButtonUI* _pButtonAddTask;


	DuiLib::CLabelUI*  _pLabelCount; //��������
	int    nMax_; //ִ���������������
	int	   nSuccentTotal_;  //ÿ��ɹ�������
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

