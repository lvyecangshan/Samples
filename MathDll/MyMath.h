#pragma once

#ifdef MATHDLL_EXPORTS
#define API_DECLSPEC  __declspec(dllexport)
#else
#define API_DECLSPEC  __declspec(dllimport)
#endif

API_DECLSPEC  class MyMath
{
public:
	API_DECLSPEC int Add(int a, int b);
};

