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

using namespace std;

// 16000 采样率 10ms，  大小 = 160 * 16bits/8 = 320字节 ,
//
//int run(FILE* fp, VadInst* vad, struct cut_info* cut);
//
//int add_period_activity(struct periods* per, int is_active, int is_last);
//
////int add_cut_file_activity(struct cut_info *cut, int is_active, int is_last);
//SimpleVad	 m_simpVad;
//PeroidFormat m_peroidFormat;
//FileCut		 m_fileCut;
//int main() {
//
//	const char filename[] = "D:\\td_work\\tdvad\\pcm\\16k_1.pcm"; // 读取的文件
//	const char output_filename_prefix[] = "D:\\td_work\\tdvad\\pcm\\16k_111.pcm"; // 保存的文件名
//	const char output_dir[] = "D:\\td_work\\tdvad\\pcm\\output_pcm"; // 保存的目录
//	FILE* fp = nullptr;
//	fopen_s(&fp,filename, "rb");
//	if (fp == NULL) {
//		fprintf(stderr, "%s does not exist\n", filename);
//		return 3;
//	}
//	VadInst* vad = m_simpVad.simple_vad_create();
//	//simple_vad* vad = simple_vad_create();
//	if (vad == NULL) {
//		return 4;
//	}
//	FILE* fp2 = nullptr;
//	fopen_s(&fp2,filename, "rb");
//	struct cut_info* cut = m_fileCut.cut_info_create(fp2);
//	snprintf(cut->output_filename_prefix, sizeof(cut->output_filename_prefix), "%s",
//		output_filename_prefix);
//	snprintf(cut->output_file_dir, sizeof(cut->output_file_dir), "%s",
//		output_dir);
//	int res = run(fp, vad, cut);
//
//	fclose(fp);
//	fclose(fp2);
//	m_simpVad.simple_vad_free(vad);
//	m_fileCut.cut_info_free(cut);
//	printf("PROGRAM FINISH\n");
//	return res;
//}
//
//int run(FILE* fp, VadInst* vad, struct cut_info* cut) {
//
//	int16_t data[FRAME_SIZE];
//	int res = 0;
//	struct periods* per = m_peroidFormat.periods_create();
//
//	while (res == 0) {
//		res = m_simpVad.read_int16_bytes(fp, data);
//		if (res <= 1) {
//			int is_last = (res == 1);
//			int is_active = m_simpVad.process_vad(vad, data);
//			add_period_activity(per, is_active, is_last);
//			int vad_file_res = m_fileCut.cut_add_vad_activity(cut, is_active, is_last);
//			if (vad_file_res < 0) {
//				printf("file write success %s\n", cut->result_filename);
//			}
//		}
//		else if (ferror(fp)) {
//			printf("read failed  ferror result  : %d\n", ferror(fp));
//		}
//
//	}
//	m_peroidFormat.periods_free(per);
//
//	if (res != 1) {
//		fprintf(stderr, "read file error %d\n", res);
//		return res;
//	}
//	return 0;
//}
//
//
//int add_period_activity(struct periods* per, int is_active, int is_last) {
//	static int old_is_active = 0;
//	static int count = 0;
//	int res_add = m_peroidFormat.period_add_vad_activity(per, is_active, is_last);
//	if (res_add != 0) {
//		return res_add;
//	}
//	if (is_active != old_is_active) {
//		// printf("%s,%d \n", old_is_active ? "A" : "I", count);
//		// I,1  表示之前的1个FRAME是 INACTIVE的；
//		// I,1 A,10 表示之前的1个FRAME是 INACTIVE的；第2-10个FRAME是ACTIVE的
//		// periods_print(per);
//		old_is_active = is_active;
//	}
//	count += 1;
//	if (is_last) {
//		m_peroidFormat.periods_print(per);
//		printf("total frames %d\n", count);
//	}
//}

typedef struct sAudioData
{
	char* buffer;
	int  iSize;
}SAUDIODATA;

int main()
{

	std::vector<SAUDIODATA*> vec;

	VadInst* vadInst = WebRtcVad_Create();
	WebRtcVad_Init(vadInst);
	WebRtcVad_set_mode(vadInst, 2);
	
	std::wstring sFileName = L"D:\\td_work\\tdvad\\pcm\\new_mp3.mp3";
	FILE* fp = nullptr;
	_wfopen_s(&fp,sFileName.c_str(), L"rb");
	
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

	const char dst[] = "D:\\pdb\\new_mp3_1111111.mp3"; // 读取的文件
	FILE* fpdst = nullptr;
	fopen_s(&fpdst, dst, "wb");


	bool bActive = true;
	for (auto it : vec)
	{
		SAUDIODATA* data = it;
		short* shortBUff = new short[data->iSize / 2];
		memset(shortBUff, 0, data->iSize / 2);
		memcpy(shortBUff, data->buffer, data->iSize);

		int status = WebRtcVad_Process(vadInst, SAMPLE_RATE, shortBUff, data->iSize /2);
		if (status == -1)
		{
			printf("WebRtcVad_Process is error\n");
			return 0;
		}
		if (status == 1) { //有语音


			bActive = true;
			//fwrite(audioFrame, sizeof(short), read_size, fpOutput);
			//fflush(fpOutput);
		}
		if (status == 0) {//静音


			bActive = false;
		}

		fwrite(shortBUff, sizeof(short), data->iSize/2, fpdst);
	}
	fclose(fpdst);
	fclose(fp);

	WebRtcVad_Free(vadInst);
}
