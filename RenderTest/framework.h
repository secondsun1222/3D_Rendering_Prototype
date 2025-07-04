// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN
// windows
#include <windows.h>
// c runtime headers
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <string>
#include <vector>

// GDI+
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


#define SINGLETON(type) public:\
							static type* GetInstance()\
							{\
								static type instance;\
								return &instance;\
							}\
						private:\
							type();\
							~type();
#define INSTANCE(type) type::GetInstance()