#ifndef __HH_TDCOMMON_H__
#define __HH_TDCOMMON_H__
#include <string>
#include <windows.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif



const int NFC1 = 1;
const int NFC2 = 2;
const int NFC3 = 3;
const int NFC4 = 4;
const int NFC5 = 5;

const int Server1 = 6;
const int Server2 = 7;
const int Server3 = 8;
const int Server4 = 9;

#define  DEF_KIOSERVE_CONNECT  WM_USER +20000
#define  DEF_KIOSERVE_DISCONNECT  WM_USER +20001



#endif
