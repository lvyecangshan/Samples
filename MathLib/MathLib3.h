#pragma once

#ifdef MATHLIB_EXPORTS
#define API_DECLSPEC extern "C" __declspec(dllexport)
#else
#define API_DECLSPEC extern "C" __declspec(dllimport)
#endif

API_DECLSPEC int  Add(int a, int b);
