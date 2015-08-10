#pragma once
#include <Codebot/IEnumerable.h>
#include <Codebot/Object.h>

namespace Codebot
{
namespace Expressions
{

// OrderByExpression class

template <typename T, typename R>
class SelectExpression : public Object, public IEnumerator<R>
{
private:
	Ref<IEnumerator<T>> parent;
	Func<R(const T&)> select;
	Shared<R> r;
public:
	SelectExpression(IEnumerator<T>* p, Func<R(const T&)> s);
	// IEnumerator<R>
	void Reset();
	Boolean HasNext();
	R* Next();
};

template <typename T, typename R>
SelectExpression<T, R>::SelectExpression(IEnumerator<T>* p, Func<R(const T&)> s) :
	parent(p), select(s)
{
}

template <typename T, typename R>
void SelectExpression<T, R>::Reset()
{
	parent->Reset();
}

template <typename T, typename R>
Boolean SelectExpression<T, R>::HasNext()
{
	return parent->HasNext();
}

template <typename T, typename R>
R* SelectExpression<T, R>::Next()
{
	T* t = parent->Next();
	if (t == null)
		return null;
	r = MakeShared<R>(select(*t));
	return r.get();
}

}
}
