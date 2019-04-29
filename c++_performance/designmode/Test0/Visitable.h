#pragma once
#include "Animal.h"
template <class T>
class Visitable : public Animal
{
public:
	void Accept(Visitor_Token *token) override
	{
		if (auto p = dynamic_cast<Visitor<T> *>(token); p)
		{
			p->Visit(static_cast<T *>(this));
		}
	}
};