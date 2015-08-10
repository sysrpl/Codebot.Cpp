#pragma once
#include <Codebot/IEnumerable.h>
#include <Codebot/Object.h>

namespace Codebot
{
namespace Expressions
{

// OrderByExpression class

template <typename T>
class OrderByExpression : public Object, public IEnumerator<T>
{
private:
	Ref<IEnumerator<T>> parent;
	Func<Boolean(const T&, const T&)> orderBy;
	Shared<std::vector<T>> items;
	Integer count;
	Integer index;
	Integer length;
	void Prepare();
public:
	OrderByExpression(IEnumerator<T>* p, Func<Boolean(const T&, const T&)> o);
	// IEnumerator<T>
	void Reset();
	Boolean HasNext();
	T* Next();
};

template <typename T>
OrderByExpression<T>::OrderByExpression(IEnumerator<T>* p, Func<Boolean(const T&, const T&)> o) :
	parent(p), orderBy(o)
{
}

template <typename T>
void OrderByExpression<T>::Prepare()
{
	if (items == null)
	{
		items = MakeShared<std::vector<T>>();
		while (parent->HasNext())
		{
			auto item = parent->Next();
			items->push_back(*item);
		}
		length = items->size();
		sort(items->begin(), items->end(), orderBy);
	}
}

template <typename T>
void OrderByExpression<T>::Reset()
{
	parent->Reset();
	items = null;
	Prepare();
	count = 0;
	index = 0;
}

template <typename T>
Boolean OrderByExpression<T>::HasNext()
{
	Prepare();
	return length > index;
}

template <typename T>
T* OrderByExpression<T>::Next()
{
	if (index >= length)
		return null;
	return &(*items)[index++];
}

}
}
