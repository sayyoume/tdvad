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
	std::wstring getCurrentSystemTime();//获取系统时间
	std::wstring getCurrentSystemTimeYear();//获取系统时间
	std::wstring  getCurrentTime();		//获取系统时间不带年月日
	void SaveFile(std::wstring& sSaveFileName, HWND hwnd);
	bool  IsDir(std::wstring& sPath);						//判断一个路径是否是文件夹
	void  GetSelectFile(std::wstring& s, HWND hwnd);		//打开文件
	void  GetSelectFileDirect(std::wstring&s, HWND hwnd);	//打开文件夹
	HICON fileIcon(std::string extention);					//获取文件图标
	std::string fileType(std::string extention);			//获取文件类型
	HICON folderIcon();										//获取文件夹图标
	std::string folderType();								//获取文件夹类型
	std::wstring GetCurrentPath();
	std::wstring GetDbPath();
	std::wstring GetAppData();
	HICON getFileIcon(std::wstring sFilePath);
	std::string createUUID();
	std::wstring  m_Path;
	std::string  sCode;
	std::wstring _sProxyUrl; //获取代理ip得url
	std::wstring _sUserName; //用户账号
	std::wstring _sEndTime; //账号到期时间
	std::wstring m_sIniPath;
	
	std::map<int, RECT> mapScreen;

	std::string m_station1;
	std::string m_station2;
	std::string m_station3;
	std::string m_station4;
	//调试信息开关
	bool m_IsOpenDebug;

};
