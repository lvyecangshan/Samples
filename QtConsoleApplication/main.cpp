#include <QtCore/QCoreApplication>
//#include "MathLib1.h"
#include "MathLib2.h"
#include "MathLib3.h"

using namespace System;


//using namespace MathLib1;
using namespace MathLib2;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	int result;
	//MyMathWrapper1 m1;
	MyMathWrapper2 m2;

	//result = m1.Add(1, 2);

	Console::WriteLine(m2.Add(1, 2));

	Console::WriteLine(Add(1, 2));

	return a.exec();
}
