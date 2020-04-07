#include "pch.h"
#include "MathLib3.h"
#include<boost/container/list.hpp>
#include <iostream>

using namespace std;

int Add(int a, int b)
{
	using namespace boost::container;

	auto *l = new list<int>();

	l->push_back(1);

	cout << l->size()<<endl;

	delete l;

	return a + b;
}