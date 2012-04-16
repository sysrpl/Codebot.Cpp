#pragma once
#include <Codebot/Expressions/SelectExpression.h>
#include <Codebot/Expressions/WhereExpression.h>
#include <Codebot/Expressions/OrderByExpression.h>
#include <Codebot/Text/Format.h>

namespace Codebot
{
namespace Expressions
{

extern Boolean CodebotExpressions;

Boolean GetCodebotExpressions();

// QueryNode class

template <typename T>
class QueryNode : public ValueType, public IEnumerable<T>
{
private:
	Ref<IEnumerator<T>> enumerator;
	T* item;
public:
	QueryNode(IEnumerator<T>* e);
	operator T& () const;
	T& operator ()() const;
	T* operator -> () const;
	T& Value() const;
	void Reset();
	Boolean Next();
	// QueryNode expressions
	/*template <typename R>
	QueryNode<R> Cast();*/
	template <typename R>
	QueryNode<R> Select(Func<R(const T&)> expression);
	QueryNode<T> Where(Func<Boolean(const T&)> expression);
	QueryNode<T> OrderBy(Func<Boolean(const T&, const T&)> expression);
	Cardinal Count();
	T First();
	T FirstOrDefault();
	T Average();
	T Sum();
	T Max();
	T Min();
	Array<T> ToArray();
	// Override methods
	String ToFormat(const String& format) const;
	// IEnumerable<T>
	IEnumerator<T>* GetEnumerator() const;
};

template <typename T>
QueryNode<T>::QueryNode(IEnumerator<T>* e)
	: enumerator(e), item(null)
{
}

template <typename T>
QueryNode<T>::operator T& () const
{
	return Value();
}

template <typename T>
T& QueryNode<T>::operator ()() const
{
	return Value();
}

template <typename T>
T* QueryNode<T>::operator -> () const
{
	if (item == null)
		ThrowNullReferenceException(ThisMethod, "item");
	return item;
}

template <typename T>
T& QueryNode<T>::Value() const
{
	if (item == null)
		ThrowNullReferenceException(ThisMethod, "item");
	return *item;
}

template <typename T>
void QueryNode<T>::Reset()
{
	enumerator->Reset();
}

template <typename T>
Boolean QueryNode<T>::Next()
{
	if (enumerator->HasNext())
	{
		item = enumerator->Next();
		return true;
	}
	return false;
}

// QueryNode<T> expressions

template <typename T>
template <typename R>
QueryNode<R> QueryNode<T>::Select(Func<R(const T&)> expression)
{
	return new SelectExpression<T, R>(enumerator, expression);
}

template <typename T>
QueryNode<T> QueryNode<T>::Where(Func<Boolean(const T&)> expression)
{
	return new WhereExpression<T>(enumerator, expression);
}

template <typename T>
QueryNode<T> QueryNode<T>::OrderBy(Func<Boolean(const T&, const T&)> expression)
{
	return new OrderByExpression<T>(enumerator, expression);
}

template <typename T>
T QueryNode<T>::First()
{
	if (!Next())
		ThrowNullReferenceException(ThisMethod, "item");
	return *item;
}

template <typename T>
T QueryNode<T>::FirstOrDefault()
{
	return Next() ? *item : T();
}

template <typename T>
Cardinal QueryNode<T>::Count()
{
	Reset();
	Cardinal r = 0;
	while (Next())
		r++;
	return r;
}

template <typename T>
T QueryNode<T>::Average()
{
	Reset();
	Integer c = 0;
	T r = T();
	while (Next())
	{
		r += *item;
		c++;
	}
	if (c > 1)
		r = r / c;
	return r;
}

template <typename T>
T QueryNode<T>::Sum()
{
	Reset();
	T r = T();
	while (Next())
		r += *item;
	return r;
}

template <typename T>
T QueryNode<T>::Max()
{
	Reset();
	if (!Next())
		ThrowNullReferenceException(ThisMethod, "item");
	T r = *item;
	while (Next())
		r = Codebot::Max(r, *item);
	return r;
}

template <typename T>
T QueryNode<T>::Min()
{
	Reset();
	if (!Next())
		ThrowNullReferenceException(ThisMethod, "item");
	T r = *item;
	while (Next())
		r = Codebot::Min(r, *item);
	return r;
}

template <typename T>
Array<T> QueryNode<T>::ToArray()
{
	Reset();
	Array<T> r;
	while (Next())
		r.Add(*item);
	return r;
}

// QueryNode<T> Override methods

template <typename T>
String QueryNode<T>::ToFormat(const String& format) const
{
	if (format == "list")
	{
		QueryNode<T> q = GetEnumerator();
		q.Reset();
		String r = "{";
		Integer i = 0;
		while(q.Next())
		{
			if (i > 0)
				r += ", ";
			r += Text::FormatValue(q.Value(), String::Empty);
			i++;
		}
		r += "}";
		return r;
	}
	else
		return Text::FormatValue(Value(), format);
}

// QueryNode<T>::IEnumerable<T>

template <typename T>
IEnumerator<T>* QueryNode<T>::GetEnumerator() const
{
	return enumerator;
}

}
}
