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
	short* buffer;
	int  iSize;
}SAUDIODATA;

int main()
{
	//for (int i =0 ;i<5;i++)
	//{
	//	int nRead1 = 160;
	//	sAudioData* data = new sAudioData;
	//	data->buffer = new short[nRead1];
	//}
	//

	std::vector<SAUDIODATA*> vec;

	VadInst* vadInst = WebRtcVad_Create();
	WebRtcVad_Init(vadInst);
	WebRtcVad_set_mode(vadInst, 2);


	const char filename[] = "D:\\td_work\\tdvad\\pcm\\new_mp3.mp3"; // 读取的文件
	FILE* fp = nullptr;
	fopen_s(&fp,filename, "rb");

	//short* buff = nullptr;

	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		long fileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		{
			char* buff = new char[fileSize];
			memset(buff, 0, fileSize);

			int nRead = fread(buff, sizeof(char), fileSize, fp);

			const char dst[] = "D:\\td_work\\tdvad\\pcm\\new_mp3_1111111.mp3"; // 读取的文件
			FILE* fpdst = nullptr;
			fopen_s(&fpdst, dst, "wb");
			fwrite(buff, sizeof(char), nRead, fpdst);
			fclose(fp);
			fclose(fpdst);
			int ii = 3;
		}
		{
			short* buff = new short[fileSize];
			memset(buff, 0, fileSize);

			int nRead = fread(buff, sizeof(short), fileSize, fp);

			const char dst[] = "D:\\td_work\\tdvad\\pcm\\new_mp3_1111111.mp3"; // 读取的文件
			FILE* fpdst = nullptr;
			fopen_s(&fpdst, dst, "wb");
			fwrite(buff, nRead, 1, fpdst);

			fclose(fpdst);
			int ii = 3;
		}
		//int nCount = 0;
		while (!feof(fp))
		{
			//short*  buff = (short*)malloc(160);
			short* buff = new short[160];
			memset(buff, 0, 160);

			int nRead = fread(buff, sizeof(short), 160, fp);
			//nCount += nRead;

			SAUDIODATA* data = new SAUDIODATA;
			data->buffer = new short[nRead];
			memset(data->buffer, 0, nRead);
			memcpy(data->buffer, buff, nRead);
			data->iSize = nRead;

			vec.push_back(data);


			//int status = WebRtcVad_Process(vadInst, SAMPLE_RATE, buff, nRead);
			//if (status == -1)
			//{
			//	printf("WebRtcVad_Process is error\n");
			//	return 0;
			//}
			//if (status == 1) {
			//	//fwrite(audioFrame, sizeof(short), read_size, fpOutput);
			//	//fflush(fpOutput);
			//}

		}

	}

	const char dst[] = "D:\\td_work\\tdvad\\pcm\\new_mp3_1111111.mp3"; // 读取的文件
	FILE* fpdst = nullptr;
	fopen_s(&fpdst, dst, "wb");

	for (auto it : vec)
	{
		SAUDIODATA* data = it;
		fwrite(data->buffer, data->iSize, 1, fpdst);
	}
	fclose(fpdst);
	fclose(fp);

	//WebRtcVad_Free(vadInst);
}
