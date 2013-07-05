#ifndef _COMMON_H
#define _COMMON_H

#include "stdafx.h"

//#define PBS_SMOOTHREVERSE 0x10
//#define PBS_MARQUEE 0x8

#define MSG_INFO (WM_USER + 100)
#define MSG_PRINT (WM_USER + 101)
#define MSG_SAVE (WM_USER + 102)
#define MSG_SET_INDEX (WM_USER + 103) 
#define MSG_CLOSE (WM_USER + 104)
//
 // CStringA to CStringW
 //
 CStringW CStrA2CStrW(const CStringA &cstrSrcA);

  //
 // CStringW to CStringA
 //
 CStringA CStrW2CStrA(const CStringW &cstrSrcW);


#endif