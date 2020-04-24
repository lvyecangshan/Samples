
#include"pch.h"
#include "MathLib1.h"
#include "..\MathDll\MyMath.h"

int MathLib1::MyMathWrapper1::Add(int a, int b)
{
	MyMath m;

	return m.Add(a, b);
}

