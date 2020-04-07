#include "pch.h"
#include "MyMath.h"
#include<boost/container/list.hpp>

int MyMath::Add(int a, int b)
{
	using namespace boost::container;

	auto *l = new list<int>();

	l->push_back(1);

	delete l;

	return a + b;
}
