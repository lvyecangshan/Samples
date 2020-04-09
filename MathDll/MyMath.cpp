#include "pch.h"
#include "MyMath.h"
#include<boost/container/list.hpp>
#include<iostream> 

int MyMath::Add(int a, int b)
{
	using namespace std;
	using namespace boost::container;

	cout.setf(ios_base::boolalpha);

	shared_ptr< list<int>> l = make_shared<  list<int>>();

	l->push_back(1);

	cout << "boost²âÊÔ½á¹û£º" << !l->empty() << endl;

	return a + b;
}
