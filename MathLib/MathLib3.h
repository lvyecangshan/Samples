#pragma once

#ifdef MATHLIB_EXPORTS
#define API_DECLSPEC  __declspec(dllexport)
#else
#define API_DECLSPEC  __declspec(dllimport)
#endif

extern "C" API_DECLSPEC int Add(int a, int b);
