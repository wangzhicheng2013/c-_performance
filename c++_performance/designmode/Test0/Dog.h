#pragma once
#include "Visitable.h"
#include <iostream>
using namespace std;
class Dog : public Visitable<Dog>
{
public:
	int Move() override
	{
		return 4;
	}
	void Swim()
	{
		cout << "I am a dog, I can swim." << endl;
	}
};

