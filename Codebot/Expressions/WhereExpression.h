#pragma once
#include <Codebot/IEnumerable.h>
#include <Codebot/Object.h>

namespace Codebot
{
namespace Expressions
{

// WhereExpression class

template <typename T>
class WhereExpression : public Object, public IEnumerator<T>
{
private:
	Ref<IEnumerator<T>> parent;
	Func<Boolean(const T&)> where;
	T* item;
public:
	WhereExpression(IEnumerator<T>* p, Func<Boolean(const T&)> w);
	// IEnumerator<T>
	void Reset();
	Boolean HasNext();
	T* Next();
};

template <typename T>
WhereExpression<T>::WhereExpression(IEnumerator<T>* p, Func<Boolean(const T&)> w) :
	parent(p), where(w), item(null)
{
}

template <typename T>
void WhereExpression<T>::Reset()
{
	parent->Reset();
	item = null;
}

template <typename T>
Boolean WhereExpression<T>::HasNext()
{
	while (parent->HasNext())
	{
		item = parent->Next();
		if (item == null)
			return false;
		if (where(*item))
			return true;
		item = null;
	}
	return false;
}

template <typename T>
T* WhereExpression<T>::Next()
{
	if (item == null)
		HasNext();
	T* i = item;
	item = null;
	return i;
}

}
}
