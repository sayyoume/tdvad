// tdVad.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "common_audio/vad/include/webrtc_vad.h"
#include <stdio.h>
#include <stdlib.h>
#include "simple_vad.h"
#include "period_format.h"
#include "file_cut.h"
#include <map>
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>

using namespace std;
#define  DEFMODE 4

// 16000 采样率 10ms，  大小 = 160 * 16bits/8 = 320字节 ,
//
int run(FILE* fp, VadInst* vad, struct cut_info* cut);

int add_period_activity(struct periods* per, int is_active, int is_last);

//int add_cut_file_activity(struct cut_info *cut, int is_active, int is_last);
SimpleVad	 m_simpVad;
PeroidFormat m_peroidFormat;
FileCut		 m_fileCut;
int main1() {

	const char filename[] = "D:\\td_work\\tdvad\\pcm\\16k_1.pcm"; // 读取的文件
	const char output_filename_prefix[] = "D:\\td_work\\tdvad\\pcm\\16k_111.pcm"; // 保存的文件名
	const char output_dir[] = "D:\\td_work\\tdvad\\pcm\\output_pcm"; // 保存的目录
	FILE* fp = nullptr;
	fopen_s(&fp,filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "%s does not exist\n", filename);
		return 3;
	}
	VadInst* vad = m_simpVad.simple_vad_create();
	//simple_vad* vad = simple_vad_create();
	if (vad == NULL) {
		return 4;
	}
	FILE* fp2 = nullptr;
	fopen_s(&fp2,filename, "rb");
	struct cut_info* cut = m_fileCut.cut_info_create(fp2);
	snprintf(cut->output_filename_prefix, sizeof(cut->output_filename_prefix), "%s",
		output_filename_prefix);
	snprintf(cut->output_file_dir, sizeof(cut->output_file_dir), "%s",
		output_dir);
	int res = run(fp, vad, cut);

	fclose(fp);
	fclose(fp2);
	m_simpVad.simple_vad_free(vad);
	m_fileCut.cut_info_free(cut);
	printf("PROGRAM FINISH\n");
	return res;
}

int run(FILE* fp, VadInst* vad, struct cut_info* cut) {

	int16_t data[FRAME_SIZE];
	int res = 0;
	struct periods* per = m_peroidFormat.periods_create();

	while (res == 0) {
		res = m_simpVad.read_int16_bytes(fp, data);
		if (res <= 1) {
			int is_last = (res == 1);
			int is_active = m_simpVad.process_vad(vad, data);
			add_period_activity(per, is_active, is_last);
			int vad_file_res = m_fileCut.cut_add_vad_activity(cut, is_active, is_last);
			if (vad_file_res < 0) {
				printf("file write success %s\n", cut->result_filename);
			}
		}
		else if (ferror(fp)) {
			printf("read failed  ferror result  : %d\n", ferror(fp));
		}

	}
	m_peroidFormat.periods_free(per);

	if (res != 1) {
		fprintf(stderr, "read file error %d\n", res);
		return res;
	}
	return 0;
}


int add_period_activity(struct periods* per, int is_active, int is_last) {
	static int old_is_active = 0;
	static int count = 0;
	int res_add = m_peroidFormat.period_add_vad_activity(per, is_active, is_last);
	if (res_add != 0) {
		return res_add;
	}
	if (is_active != old_is_active) {
		// printf("%s,%d \n", old_is_active ? "A" : "I", count);
		// I,1  表示之前的1个FRAME是 INACTIVE的；
		// I,1 A,10 表示之前的1个FRAME是 INACTIVE的；第2-10个FRAME是ACTIVE的
		// periods_print(per);
		old_is_active = is_active;
	}
	count += 1;
	if (is_last) {
		m_peroidFormat.periods_print(per);
		printf("total frames %d\n", count);
	}
}

std::string ws2s(const std::wstring& wide)
{
	int wide_length = static_cast<int>(wide.length());
	if (wide_length == 0)
		return std::string();

	// Compute the length of the buffer we'll need.
	int charcount = WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide_length,
		NULL, 0, NULL, NULL);
	if (charcount == 0)
		return std::string();

	std::string mb;
	mb.resize(charcount);
	WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide_length,
		&mb[0], charcount, NULL, NULL);

	return mb;
}

std::wstring s2ws(const std::string& mb)
{
	if (mb.empty())
		return std::wstring();

	int mb_length = static_cast<int>(mb.length());
	// Compute the length of the buffer.
	int charcount = MultiByteToWideChar(CP_UTF8, 0,
		mb.data(), mb_length, NULL, 0);
	if (charcount == 0)
		return std::wstring();

	std::wstring wide;
	wide.resize(charcount);
	MultiByteToWideChar(CP_UTF8, 0, mb.data(), mb_length, &wide[0], charcount);

	return wide;
}


std::wstring getCurrentPath() {
	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	return szPath;
}

std::wstring getConfigString(std::wstring wsSection, std::wstring key)
{
	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	std::wstring wsPath = szPath;
	wsPath.append(L"\\config.ini");


	wchar_t wsIp[256] = { 0 };
	GetPrivateProfileString(wsSection.c_str(), key.c_str(), L"", wsIp, sizeof(wsIp), wsPath.c_str());
	return wsIp;
}

typedef struct sAudioData
{
	char* buffer;
	int  iSize;
}SAUDIODATA;


void pcm_2_mp3(std::string sSrcName,std::string sDstName)
{
	std::wstring wsSrc = s2ws(sSrcName);
	std::wstring wsDst = s2ws(sDstName);

	std::wstring wsFFmpeg = getCurrentPath() + L"\\ffmpeg.exe";

	wchar_t wsparam[MAX_PATH];
	swprintf_s(wsparam, L"-y -f s16be -ac 1 -ar 16000 -acodec pcm_s16le -i \"%s\"  \"%s\"", wsSrc.c_str(), wsDst.c_str());

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = wsFFmpeg.c_str();
	ShExecInfo.lpParameters = wsparam;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void mp3_2_pcm(std::wstring sSrcName, std::wstring sDstName)
{
	std::wstring wsFFmpeg = getCurrentPath() + L"\\ffmpeg.exe";
	wchar_t wsparam[MAX_PATH];
	swprintf_s(wsparam, L"-y -i \"%s\"  -f s16be -ac 1 -ar 16000 -acodec pcm_s16le    \"%s\"", sSrcName.c_str(), sDstName.c_str());

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = wsFFmpeg.c_str();
	ShExecInfo.lpParameters = wsparam;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

int main()
{
	std::vector<std::string> vecPcmName;
	std::vector<SAUDIODATA*> vec;
	VadInst* vadInst = WebRtcVad_Create();
	WebRtcVad_Init(vadInst);
	WebRtcVad_set_mode(vadInst, DEFMODE);
	std::wstring sSrcPath = getConfigString(L"CONFIG",L"path");

	int n_no_pot = sSrcPath.find_last_of(L".");
	std::wstring ws_pcm_no_pot_name = sSrcPath.substr(0, n_no_pot);
	
	std::wstring ws_pcm_name = ws_pcm_no_pot_name;
	ws_pcm_name.append(L".pcm");
	mp3_2_pcm(sSrcPath, ws_pcm_name);


	//path
	int nnpos = ws_pcm_no_pot_name.find_last_of(L"\\");
	std::wstring tmp = ws_pcm_no_pot_name.substr(nnpos + 1, ws_pcm_no_pot_name.length());

	int npos = sSrcPath.find_last_of(L"\\");
	std::wstring wspath = sSrcPath.substr(0,npos+1);
	wspath.append(tmp);
	wspath.append(L"_item\\");
	CreateDirectory(wspath.c_str(), nullptr);

	
	wspath.append(tmp);
	wspath.append(L"_%d.pcm");
	
	FILE* fp = nullptr;
	_wfopen_s(&fp, ws_pcm_name.c_str(), L"rb");
	
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		long fileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		
		while (!feof(fp))
		{
			
			char* buff = new char[320];
			memset(buff, 0, 320);
			int nRead = fread(buff, sizeof(char), 320, fp);
			SAUDIODATA* data = new SAUDIODATA;
			data->buffer = new char[nRead];
			memset(data->buffer, 0, nRead);
			memcpy(data->buffer, buff, nRead);
			data->iSize = nRead;

			vec.push_back(data);
		}
	}
	fclose(fp);


	bool bActive = false;
	bool bOldActive = false;
	int ii = 0;
	std::vector<SAUDIODATA*> vectmp;
	for (auto it : vec)
	{
		
		if (bActive != bOldActive && bActive == false) {
			std::string spath = ws2s(wspath);
			char dst[245]{};
			sprintf_s(dst, spath.c_str(), ++ii);
			FILE* fdsttemp = nullptr;
			fopen_s(&fdsttemp, dst, "wb");

			for (auto it : vectmp)
			{
				SAUDIODATA* data = it;
				fwrite(data->buffer, sizeof(char), data->iSize, fdsttemp);
			}
			fclose(fdsttemp);
			vectmp.clear();
			vecPcmName.push_back(dst);
		}
		
		bOldActive = bActive;

		SAUDIODATA* data = it;
		short* shortBUff = new short[data->iSize / 2];
		memset(shortBUff, 0, data->iSize / 2);
		memcpy(shortBUff, data->buffer, data->iSize);

		int status = WebRtcVad_Process(vadInst, SAMPLE_RATE, shortBUff, data->iSize /2);
		if (status == -1)
		{
			for (auto it : vecPcmName)
			{
				int npos = it.find_last_of(".");
				std::string dst = it.substr(0, npos);
				dst.append(".mp3");			
				pcm_2_mp3(it, dst);
				
				DeleteFileA(it.c_str());

			}

			DeleteFile(ws_pcm_name.c_str());
			return 0;
		}
		if (status == 1) { //有语音

			SAUDIODATA* datat = new SAUDIODATA;
			datat->buffer = new char[data->iSize];
			memset(datat->buffer, 0, data->iSize);
			memcpy(datat->buffer, data->buffer, data->iSize);
			datat->iSize = data->iSize;

			vectmp.push_back(datat);
			bActive = true;
			
		}
		if (status == 0) {//静音
			bActive = false;
		}
	
	}
	
	WebRtcVad_Free(vadInst);



	


}
