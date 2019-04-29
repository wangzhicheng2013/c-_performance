#pragma once
template<class T>
class Visitor
{
public:
	virtual void Visit(T *) = 0;
};
class Visitor_Token
{
public:
	virtual ~Visitor_Token() = default;
};
