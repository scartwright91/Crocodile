#pragma once

#ifdef CC_PLATFORM_WINDOWS
#ifdef CC_BUILD_DLL
#define CROCODILE_API __declspec(dllexport)
#else
#define CROCODILE_API __declspec(dllimport)
#endif
#else
#define CROCODILE_API
#endif
