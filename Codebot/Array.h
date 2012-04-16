#pragma once
#include "Codebot/VectorEnumerator.h"

namespace Codebot
{

// Array<T, Integer> 

#pragma message(Reminder "Test IEnumerable<T>")

template <typename T>
class Array : public ValueType, public IArray<T, Integer>, public IEnumerable<T>
{
private:
	Shared<std::vector<T>> data;
	void Unshare();
public:
	virtual ~Array();
	Array();
	Array(Integer length);
	#ifdef GCC
	Array(const InitializerList<T>& list);
	#endif
	Array(const Array<T>& a);
	Array<T>& operator = (const Array<T>& a);
	void Add(const T& item);
	void Remove(const T& item);
	void Delete(Integer index);
	void Clear();
	// IArray<T, Integer> 
	T& Item(const Integer& index);
	const T& Item(const Integer& index) const;
	T& operator [] (const Integer& index);
	const T& operator [] (const Integer& index) const;
	Cardinal Length() const;
	void Length(Cardinal value);
	// IEnumerable<T> 
	IEnumerator<T>* GetEnumerator() const;
};

// Array<T>

template <typename T>
Array<T>::~Array()
{
}

template <typename T>
Array<T>::Array()
{
	data = MakeShared<std::vector<T>>();
}

template <typename T>
Array<T>::Array(Integer length)
{
	if (length < 0)
		ThrowArgumentException(ThisMethod, "length");
	data = MakeShared<std::vector<T>>();
	data->resize(length);
}

#ifdef GCC
template <typename T>
Array<T>::Array(const InitializerList<T>& list)
{
	data = MakeShared<std::vector<T>>();
    for (auto i = list.begin(); i != list.end(); i++)
    	Add(*i);
}
#endif

template <typename T>
Array<T>::Array(const Array<T>& a)
{
	data = a.data;
}

template <typename T>
void Array<T>::Unshare()
{
	if (!data.unique())
	{
		auto d = data;
		data = MakeShared<std::vector<T>>();
		data->assign(d->begin(), d->end());
	}
}

template <typename T>
Array<T>& Array<T>::operator = (const Array<T>& a)
{
	if (&a == this)
		return *this;
	data = a.data;
	return *this;
}

template <typename T>
void Array<T>::Add(const T& item)
{
	Unshare();
	Integer size = data->size();
	Integer capacity = data->capacity();
	if (size >= capacity)
	{
		if (capacity == 0)
			data->reserve(0xF);
		else if (size < 0xF)
			data->reserve(0xF);
		else
			data->reserve(capacity * 2);
	}
	data->push_back(item);
}

template <typename T>
void Array<T>::Remove(const T& item)
{
	if (data->size() == 0)
		return;
	Integer i = remove(data->begin(), data->end(), item) - data->begin();
	if (i > -1)
		data->erase(data->begin() + i, data->end());
	if (data->size() == 0)
		Clear();
}

template <typename T>
void Array<T>::Delete(Integer index)
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	Integer i = data->size();
	if (index >= i)
		ThrowRangeException(ThisMethod);
	Unshare();
	data->erase(data->begin() + index);
	if (i == 1)
		Clear();
}

template <typename T>
void Array<T>::Clear()
{
	data = MakeShared<std::vector<T>>();
}

// Array<T>::IArray<T, Integer> 

template <typename T>
T& Array<T>::Item(const Integer& index)
{
	return (*this)[index];
}

template <typename T>
const T& Array<T>::Item(const Integer& index) const
{
	return (*this)[index];
}

template <typename T>
T& Array<T>::operator [] (const Integer& index)
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	Unshare();
	Integer size = data->size();
	Integer capacity = data->capacity();
	if (index >= size)
	{
		if (index >= capacity)
		{
			if (capacity == 0)
				data->reserve(0xF);
			else
				data->reserve(capacity * 2);
		}
		data->resize(index + 1);
	}
	return (*data)[index];
}

template <typename T>
const T& Array<T>::operator [] (const Integer& index) const
{
	if (index < 0)
		ThrowArgumentException(ThisMethod, "index");
	if (index >= (Integer)data->size())
		ThrowRangeException(ThisMethod);
	return (*data)[index];
}

template <typename T>
Cardinal Array<T>::Length() const
{
	return data->size();
}

template <typename T>
void Array<T>::Length(Cardinal value)
{
	if (value < 0)
		ThrowArgumentException(ThisMethod, "value");
	auto i = data->size();
	if (i == value)
		return;
	if (value == 0)
		Clear();
	else
	{
		Unshare();
		data->resize(value);
		data->shrink_to_fit();
	}
}

// Array<T>::IEnumerable<T>

template <typename T>
IEnumerator<T>* Array<T>::GetEnumerator() const
{
	return new VectorEnumerator<T>(*data);
}

}
