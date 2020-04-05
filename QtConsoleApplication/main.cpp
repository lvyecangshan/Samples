#include <QtCore/QCoreApplication>
#include "MathLib2.h"

int main(int argc, char *argv[])
{
	using namespace MathLib2;

	QCoreApplication a(argc, argv);

	MyMathWrapper2 m;

	int result = m.Add(1, 2);

	return a.exec();
}
