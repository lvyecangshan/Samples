#include "pch.h"
#include "MathLib3.h"

using namespace std;
using namespace Newtonsoft::Json;
using namespace System;

int Add(int a, int b)
{
	int result = a + b;

	Console::WriteLine(JsonConvert::SerializeObject(result));

	return result;
}