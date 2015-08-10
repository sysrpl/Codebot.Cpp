#pragma once
#include <Codebot/Types.h>

namespace Codebot
{

#define PropType(Prop) decltype(Prop())
#define CreateProp(Class, Name) Name(this, &Class::Get##Name, &Class::Set##Name)
#define CreateConstProp(Class, Name) Name(this, &Class::Get##Name)

#define ByValue false
#define ByRef true

template <typename Container, typename Type, bool Get = ByValue>
class Property
{
public:
    typedef typename std::conditional<Get, Type&, Type>::type RetType;
	typedef RetType (Container::*GetPropFunc)() const;
	typedef void (Container::*SetPropFunc)(const Type& value);
private:
	int offset;
	GetPropFunc get;
	SetPropFunc set;

	Container* GetContainer() const
	{
		Container* c = 	(Container*)((int)this - offset);
		return c;
	}
public:
	Property(Container* c, GetPropFunc g, SetPropFunc s)
		: offset(0), get(g), set(s)
	{
		offset = (int)this - (int)c;
	}

	Property(Container* c, GetPropFunc g)
		: offset(0), get(g), set(null)
	{
		offset = (int)this - (int)c;
	}

	inline RetType GetProp() const
	{
		return (GetContainer()->*get)();
	}

	inline RetType SetProp(const Type& value)
	{
		if (set == null)
			ThrowNullReferenceException(ThisMethod, "set");
		(GetContainer()->*set)(value);
		return GetProp();
	}

	RetType operator ()() const
	{
		return GetProp();
	}

	RetType operator ()(const Type& value)
	{
		return SetProp(value);
	}

	operator RetType () const
	{
		return GetProp();
	}

	RetType operator = (const Type& value)
	{
		return SetProp(value);
	}

	Boolean operator == (const Type& value) const
	{
		return GetProp() == value;
	}

	Boolean operator != (const Type& value) const
	{
		return GetProp() != value;
	}

	Boolean operator < (const Type& value) const
	{
		return GetProp() < value;
	}

	Boolean operator <= (const Type& value) const
	{
		return GetProp() <= value;
	}

	Boolean operator > (const Type& value) const
	{
		return GetProp() > value;
	}

	Boolean operator >= (const Type& value) const
	{
		return GetProp() >= value;
	}

	Type operator + (const Type& value) const
	{
		return GetProp() + value;
	}

	Type operator += (const Type& value)
	{
		SetProp(GetProp() + value);
		return GetProp();
	}

	Type operator -= (const Type& value)
	{
		SetProp(GetProp() - value);
		return GetProp();
	}

	Type operator - (const Type& value) const
	{
		return GetProp() - value;
	}

	Type& operator ++ ()
	{
		Type& r = GetProp();
		r++;
		return r;
	}

	Type& operator -- ()
	{
		Type& r = GetProp() ;
		r--;
		return r;
	}

	Type operator - () const
	{
		return -GetProp();
	}

	Type operator + () const
	{
		return +GetProp();
	}

	Type operator & (const Type& value) const
	{
		return GetProp() & value;
	}

	Type operator | (const Type& value) const
	{
		return GetProp() | value;
	}

	Type operator ^ (const Type& value) const
	{
		return GetProp() ^ value;
	}

	Type operator ~ () const
	{
		return ~ GetProp();
	}

	Type operator && (const Type& value) const
	{
		return GetProp() && value;
	}

	Type operator || (const Type& value) const
	{
		return GetProp() || value;
	}

	Type operator ! () const
	{
		return ! GetProp();
	}
};

}
