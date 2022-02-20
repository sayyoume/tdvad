#pragma once

#include <string>

namespace safe
{
	class MD5
	{
	public:
		static bool md5(const char *buf, char *md5val, int bufSize = 0);
		static bool md5(const char *buf, wchar_t *md5val, int bufSize = 0);

		static bool md5File(const char *path, char *md5val);
		static bool md5File(const wchar_t *path, wchar_t *md5val);
		static bool md5File(const wchar_t *path, std::wstring& ret);

	private:
		//MD5ժҪֵ�ṹ��
		typedef struct MD5VAL_STRUCT
		{
			unsigned int a;
			unsigned int b;
			unsigned int c;
			unsigned int d;
		} MD5VAL;

		//�����ַ�����MD5ֵ(����ָ���������ɺ�������)
		static MD5VAL md5sum(char * str, unsigned int size=0);

		//MD5�ļ�ժҪ
		static MD5VAL md5sumFile(FILE * fpin);
		static unsigned int conv(unsigned int a);
	};
}