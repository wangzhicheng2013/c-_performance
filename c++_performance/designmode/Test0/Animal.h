#pragma once
#include "Visitor.h"
class Animal
{
public:
	Animal()
	{
	}
	virtual ~Animal()
	{
	}
public:
	virtual int Move() = 0;
	virtual void Accept(Visitor_Token *) = 0;
};