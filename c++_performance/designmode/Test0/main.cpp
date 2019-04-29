#include "Dog.h"
#include "Bird.h"
#include <memory>
template<class... T>
class MultipleVisitor : public Visitor_Token, public Visitor<T>...
{
};
using MyVisitor = MultipleVisitor<Dog, Bird>;
using MyVisitor1 = MultipleVisitor<Bird>;
class Visitor_Impl : public MyVisitor
{
public:
	void Visit(Dog* p) override
	{
		p->Swim();
	}
	void Visit(Bird* p) override
	{
		p->Fly();
	}
};
class Visitor_Impl1 : public MyVisitor1
{
public:
	void Visit(Bird* p) override
	{
		p->Fly();
	}
};
int main()
{
	Animal *animal = new Dog;
	Visitor_Token *token = new Visitor_Impl;
	animal->Accept(token);
	delete(animal);
	delete(token);

	animal = new Bird;
	token = new Visitor_Impl;
	animal->Accept(token);
	delete(animal);
	delete(token);

	animal = new Bird;
	token = new Visitor_Impl1;
	animal->Accept(token);
	delete(animal);
	delete(token);

	return 0;
}