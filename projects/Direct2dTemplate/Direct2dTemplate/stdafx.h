#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN 

#include "common_stdafx.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#define xassert(x)	_ASSERT(x)


using namespace std::placeholders;

#define TRACE // 