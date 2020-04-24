#pragma once

#ifdef MATHLIB_EXPORTS
#define API_DECLSPEC  __declspec(dllexport)
#else
#define API_DECLSPEC  __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	API_DECLSPEC int  Add(int a, int b);

#ifdef __cplusplus
}
#endif