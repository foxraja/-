// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#include <afx.h>
#include <ole2.h>
#include <initguid.h>
#include <windows.h>
#include <winsock2.h>
// 공유 라이브러리
#pragma comment (lib, "../Library/HSEL.lib")
#include "../Library/Shared/HSEL.h"
#pragma comment (lib, "../Library/Shared.lib")
#include "../Library/Shared/Shared.h"

#define rand()	rand_()		// 020707 YGI
extern int rand_();

#include "HigherLayers/DefaultHeader.h"

#include "GameSystem.h"


