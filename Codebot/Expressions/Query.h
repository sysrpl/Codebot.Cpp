#pragma once
#include <Codebot/Expressions/QueryNode.h>

namespace Codebot
{
namespace Expressions
{

// The QueryInstance function

template <typename T>
T& QueryInstance(const Ref<T>& ref)
{
	return ref.Value();
}

template <typename T>
T& QueryInstance(T* instance)
{
	if (instance == null)
		ThrowNullReferenceException(ThisMethod, "instance");
	return *instance;
}

template <typename T>
const T& QueryInstance(const T& instance)
{
	return instance;
}

// The MakeQuery function

template <typename T>
QueryNode<T> MakeQuery(IEnumerator<T>* e)
{
	if (e == null)
		ThrowNullReferenceException(ThisMethod, "e");
	return QueryNode<T>(e);
}

// Macros

#define Query(a) MakeQuery(QueryInstance(a).GetEnumerator())
#define ForEach(a, b) auto a = Query(b); while (a.Next())
#define ExecuteEach(a) { auto z = Query(a); while (z.Next()) z.Value()(); }


}
}
