#pragma  once
#include <string>
#include<algorithm>  
#include<vector> 
#include <mutex>
#include<windows.h>
#include <map>
#include "../tdcommon.h"

#pragma comment(lib,"Shlwapi.lib")

class TDData
{
public:
	static TDData& Instance() { static TDData theRatData; return theRatData; }

	void setDebugViewInfo(std::wstring sInfo, std::wstring key = L"ttttt");
	std::string getConfigStringA(std::string wsSection, std::string key);
	std::wstring getConfigString(std::wstring wsSection,std::wstring key);
	int	getConfigInt(std::wstring wsSection, std::wstring key);

	void writeConfigString(std::wstring wsSection, std::wstring key, std::wstring value);
	void writeConfigInt(std::wstring wsSection, std::wstring key, int value);

	bool resultIsSuccess(std::wstring sData);
	std::string  ws2s(const std::wstring& wstr);
	std::wstring  s2ws(const std::string& str);

	std::string  tdws2s(const std::wstring& wide);
	std::wstring  tds2ws(const std::string& mb);

	std::wstring GetPathOrURLShortName(std::wstring strFullName);
	std::wstring getCurrentSystemTime();//��ȡϵͳʱ��
	std::wstring getCurrentSystemTimeYear();//��ȡϵͳʱ��
	std::wstring  getCurrentTime();		//��ȡϵͳʱ�䲻��������
	void SaveFile(std::wstring& sSaveFileName, HWND hwnd);
	bool  IsDir(std::wstring& sPath);						//�ж�һ��·���Ƿ����ļ���
	void  GetSelectFile(std::wstring& s, HWND hwnd);		//���ļ�
	void  GetSelectFileDirect(std::wstring&s, HWND hwnd);	//���ļ���
	HICON fileIcon(std::string extention);					//��ȡ�ļ�ͼ��
	std::string fileType(std::string extention);			//��ȡ�ļ�����
	HICON folderIcon();										//��ȡ�ļ���ͼ��
	std::string folderType();								//��ȡ�ļ�������
	std::wstring GetCurrentPath();
	std::wstring GetDbPath();
	std::wstring GetAppData();
	HICON getFileIcon(std::wstring sFilePath);
	std::string createUUID();
	std::wstring  m_Path;
	std::string  sCode;
	std::wstring _sProxyUrl; //��ȡ����ip��url
	std::wstring _sUserName; //�û��˺�
	std::wstring _sEndTime; //�˺ŵ���ʱ��
	std::wstring m_sIniPath;
	
	std::map<int, RECT> mapScreen;

	std::string m_station1;
	std::string m_station2;
	std::string m_station3;
	std::string m_station4;
	//������Ϣ����
	bool m_IsOpenDebug;

};
