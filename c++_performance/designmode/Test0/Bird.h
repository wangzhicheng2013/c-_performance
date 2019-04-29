#pragma once
#include "Visitable.h"
#include <iostream>
using namespace std;
class Bird : public Visitable<Bird>
{
public:
	int Move() override
	{
		return 2;
	}
	void Fly()
	{
		cout << "I am a bird, I can fly." << endl;
	}
};