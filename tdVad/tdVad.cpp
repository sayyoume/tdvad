// tdVad.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "common_audio/vad/include/webrtc_vad.h"

int main()
{
	VadInst* vadInst = WebRtcVad_Create();
	WebRtcVad_Init(vadInst);
	WebRtcVad_set_mode(vadInst, 2);


	const char filename[] = "pcm/16k_1.pcm"; // 读取的文件
	FILE* fp = nullptr;
	fopen_s(&fp,filename, "rb");

	char* buff = nullptr;

	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		long fileSize = ftell(fp);
		rewind(fp);

		int nCount = 0;
		while (nCount >= fileSize)
		{
			buff = (char*)malloc(320);

			int nRead = fread(buff, sizeof(char), 320, fp);
			nCount += nRead;
		}

	}


	WebRtcVad_Free(vadInst);
}
