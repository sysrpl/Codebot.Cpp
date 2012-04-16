#pragma once
#include "Codebot/Collections/CollectionsExceptions.h"
#include "Codebot/Object.h"
#include "Codebot/VectorEnumerator.h"

namespace Codebot
{
namespace Collections
{

// List<T> 

enum class ListDuplicate
{
	Allow,
	Ignore,
	Error
};

template <typename T>
class List : public Object, public IArray<T, Integer>, public IEnumerable<T>
{
public:
	typedef Func<Boolean(const T&)> FindFunc;
	typedef Func<Boolean(const T&, const T&)> CompareFunc;
private:
	Shared<std::vector<T>> data;
	ListDuplicate duplicates;
	void Unshare();
protected:
	void Push(const T& item);
	virtual void Changed();
public:
	List();
	#ifdef GCC
	List(const InitializerList<T>& list);
	#endif
	void Assign(Object* source);
	void Clear();
	void Add(const T& item);
	void Remove(const T& item);
	void Delete(Integer index);
	Boolean IsEmpty() const;
	T& First();
	T& Last();
	void Extract(FindFunc f);
	void Sort();
	void Sort(CompareFunc c);
	Integer Search(const T& item) const;
	Integer Search(const T& item, CompareFunc c) const;
	Integer Find(FindFunc f) const;
	Integer IndexOf(const T& item) const;
	Cardinal Capacity() const;
	void Capacity(Cardinal value);
	ListDuplicate Duplicates() const;
	void Duplicates(ListDuplicate value);
	// IArray<T> 
	T& Item(const Integer& index);
	const T& Item(const Integer& index) const;
	T& operator [] (const Integer& index);
	const T& operator [] (const Integer& index) const;
	Cardinal Length() const;
	// IEnumerable<T>
	IEnumerator<T>* GetEnumerator() const;
};

template <typename T>
List<T>::List()
	: data(MakeShared<std::vector<T>>()), duplicates(ListDuplicate::Allow)
{
}

#ifdef GCC
template <typename T>
List<T>::List(const InitializerList<T>& list)
	: data(MakeShared<std::vector<T>>()), duplicates(ListDuplicate::Allow)
{
    for (auto i = list.begin(); i != list.end(); i++)
    	Add(*i);
}
#endif

template <typename T>
void List<T>::Unshare()
{
	if (!data.unique())
	{
		auto d = data;
		data = MakeShared<std::vector<T>>();
		data->assign(d->begin(), d->end());
	}
}

template <typename T>
void List<T>::Assign(Object* source)
{
	if (Is<List<T>>(source))
	{
		auto list = As<List<T>>(source);
		data = list->data;
		Changed();
	}
	else
		Object::Assign(source);
}

template <typename T>
void List<T>::Changed()
{
}

template <typename T>
void List<T>::Clear()
{
	if (data->size() > 0)
	{
		Unshare();
		data = MakeShared<std::vector<T>>();
		Changed();
	}
}

template <typename T>
void List<T>::Push(const T& item)
{
	Unshare();
	data->push_back(item);
}

template <typename T>
void List<T>::Add(const T& item)
{
	switch (duplicates)
	{
		case ListDuplicate::Ignore:
			if (IndexOf(item) > -1)
				return;
			break;
		case ListDuplicate::Error:
			if (IndexOf(item) > -1)
				ThrowDuplicateItemException(ThisMethod);
			break;
		default:
			break;
	}
	Unshare();
	Cardinal i = data->capacity();
	if (i == 0)
		data->reserve(0xF);
	else if (i == data->size())
		data->reserve(i * 2);
	data->push_back(item);
	Changed();
}

template <typename T>
void List<T>::Remove(const T& item)
{
	if (data->size() == 0)
		return;
	Unshare();
	Integer i = remove(data->begin(), data->end(), item) - data->begin();
	if (i > -1)
	{
		data->erase(data->begin() + i, data->end());
		Changed();
	}
}

template <typename T>
void List<T>::Delete(Integer index)
{
	if (index < 0)
		ThrowRangeException(ThisMethod);
	Integer i = data->size();
	if (index >= i)
		ThrowRangeException(ThisMethod);
	Unshare();
	data->erase(data->begin() + index);
	if (i == 1)
		Clear();
	else
		Changed();
}

template <typename T>
Boolean List<T>::IsEmpty() const
{
	return data->size() == 0;
}

template <typename T>
T& List<T>::First()
{
	if (data->size() == 0)
		ThrowRangeException(ThisMethod);
	return *data->begin();
}

template <typename T>
T& List<T>::Last()
{
	if (data->size() == 0)
		ThrowRangeException(ThisMethod);
	return *(data->end() - 1);
}

template <typename T>
void List<T>::Extract(FindFunc f)
{
	if (f == null)
		ThrowNullReferenceException(ThisMethod, "f");	
	Unshare();
	Integer i = remove_if(data->begin(), data->end(), f) - data->begin();
	if (i > -1)
	{
		data->erase(data->begin() + i, data->end());
		if (data->size() == 0)
			Clear();
		else
			Changed();
	}
}

template <typename T>
void List<T>::Sort()
{
	if (data->size() > 1)
	{
		Unshare();
		sort(data->begin(), data->end());
		Changed();
	}
}

template <typename T>
void List<T>::Sort(CompareFunc c)
{
	if (c == null)
		ThrowNullReferenceException(ThisMethod, "c");	
	if (data->size() > 1)
	{
		Unshare();
		sort(data->begin(), data->end(), c);
		Changed();
	}
}

template <typename T>
Integer List<T>::Search(const T& item) const
{
	Integer i = lower_bound(data->begin(), data->end(), item) - data->begin();
	return (i < (Integer)data->size()) ? i : -1;
}

template <typename T>
Integer List<T>::Search(const T& item, CompareFunc c) const
{
	if (c == null)
		ThrowNullReferenceException(ThisMethod, "c");	
	Integer i = lower_bound(data->begin(), data->end(), item, c) - data->begin();
	return (i < (Integer)data->size()) ? i : -1;
}

template <typename T>
Integer List<T>::Find(FindFunc f) const
{
	if (f == null)
		ThrowNullReferenceException(ThisMethod, "f");	
	Integer i = std::find_if(data->begin(), data->end(), f) - data->begin();
	return i < (Integer)data->size() ? i : -1;
}

template <typename T>
Integer List<T>::IndexOf(const T& item) const
{
	Integer i = std::find(data->begin(), data->end(), item) - data->begin();
	return i < (Integer)data->size() ? i : -1;
}

template <typename T>
Cardinal List<T>::Capacity() const
{
	return data->capacity();
}

template <typename T>
void List<T>::Capacity(Cardinal value)
{
	if (value > data->capacity())
	{
		Unshare();
		data->reserve(value);
	}
}

template <typename T>
ListDuplicate List<T>::Duplicates() const
{
	return duplicates;
}

template <typename T>
void List<T>::Duplicates(ListDuplicate value)
{
	duplicates = value;
}

// List<T>::IArray<T> 

template <typename T>
T& List<T>::Item(const Integer& index)
{
	return (*this)[index];
}

template <typename T>
const T& List<T>::Item(const Integer& index) const
{
	return (*this)[index];
}

template <typename T>
T& List<T>::operator [] (const Integer& index)
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	if (index >= (Integer)data->size())
		ThrowRangeException(ThisMethod);
	Unshare();
	Changed();
	 return (*data)[index];
}

template <typename T>
const T& List<T>::operator [] (const Integer& index) const
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	if (index >= (Integer)data->size())
		ThrowRangeException(ThisMethod);
	 return (*data)[index];
}

template <typename T>
Cardinal List<T>::Length() const
{
	return data->size();
}

// List<T>::IEnumerable<T>

template <typename T>
IEnumerator<T>* List<T>::GetEnumerator() const
{
	return new VectorEnumerator<T>(*data);
}

}
}
