#ifndef __PREADDLIB_H
#define __PREADDLIB_H


#ifdef _DEBUG
#pragma comment( lib, "DuiLibD.lib" )
#pragma comment( lib, "libcurld.lib" )
#else
#pragma comment( lib, "DuiLib.lib" )
#pragma comment( lib, "libcurl.lib" )
#endif


#pragma comment(lib,"Iphlpapi.lib")
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "wldap32.lib" )

#endif
