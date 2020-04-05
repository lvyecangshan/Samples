#include "pch.h"
#include "MathLib2.h"
#include "MyMath.h"

using namespace MathLib2;

int MyMathWrapper2::Add(int a, int b)
{
	MyMath m;

	return m.Add(a, b);
}
