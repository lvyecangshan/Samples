#pragma once

#ifdef MATHLIB_EXPORTS
#define API_DECLSPEC  __declspec(dllexport)
#else
#define API_DECLSPEC  __declspec(dllimport)
#endif

namespace MathLib2 {
	class API_DECLSPEC MyMathWrapper2
	{
	public:
		int   Add(int a, int b);
	};
}


