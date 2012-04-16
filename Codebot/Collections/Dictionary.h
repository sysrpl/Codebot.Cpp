#pragma once
#include "Codebot/Collections/List.h"

namespace Codebot
{
namespace Collections
{

// KeyValue<K, V>

template <typename K, typename V>
class KeyValue : public ValueType
{
private:
	K key;
	V value;
public:
	KeyValue(const K& k, const V& v);
	Boolean operator == (const KeyValue<K, V>& keyValue) const;
	const K& Key() const;
	const V& Value() const;
	K& Key();
	V& Value();
};

// KeyValue<K, V>

template <typename K, typename V>
KeyValue<K, V>::KeyValue(const K& k, const V& v)
	: key(k), value(v)
{
}

template <typename K, typename V>
Boolean KeyValue<K, V>::operator == (const KeyValue<K, V>& keyValue) const
{
	return key == keyValue.key;
}

template <typename K, typename V>
const K& KeyValue<K, V>::Key() const
{
	return key;
}

template <typename K, typename V>
const V& KeyValue<K, V>::Value() const
{
	return value;
}
template <typename K, typename V>
K& KeyValue<K, V>::Key()
{
	return key;
}

template <typename K, typename V>
V& KeyValue<K, V>::Value()
{
	return value;
}

// Dictionary<K, V>

template <typename K, typename V>
class Dictionary : public Object, public IArray<V, K>, public IEnumerable<KeyValue<K, V>>
{
public:
	typedef KeyValue<K, V> Pair;
	typedef Func<Boolean(const Pair&)> FindFunc;
	typedef Func<Boolean(const Pair&, const Pair&)> CompareFunc;
private:
	typedef List<KeyValue<K, V> > KeyValues;
	Ref<KeyValues> keyValues;
	Boolean DefaultSort(const Pair& a, const Pair& b) const;
	Boolean DefaultFind(const Pair& pair) const;
public:
	Dictionary();
	void Add(const K& key, const V& value);
	void Remove(const K& key);
	void Delete(Integer index);
	void Clear();
	void Sort();
	void Sort(CompareFunc c);
	Integer Find(FindFunc f) const;
	Integer IndexOf(const K& key) const;
	const K& Keys(Integer index) const; 
	const V& Values(Integer index) const; 
	// IArray<V, K>
	V& Item(const K& key);
	const V& Item(const K& key) const;
	V& operator [] (const K& key);
	const V& operator [] (const K& key) const;
	Cardinal Length() const;
	// IEnumerable<KeyValue<K, V>>
	IEnumerator<KeyValue<K, V>>* GetEnumerator() const;
};

// Dictionary<K, V>

template <typename K, typename V>
Dictionary<K, V>::Dictionary()
	: keyValues(New<KeyValues>())
{
	keyValues->Duplicates(ListDuplicate::Error);
}

template <typename K, typename V>
void Dictionary<K, V>::Add(const K& key, const V& value)
{
	keyValues->Add(KeyValue<K, V>(key, value));
}

template <typename K, typename V>
void Dictionary<K, V>::Remove(const K& key)
{
	auto i = IndexOf(key);
	if (i > -1)
		keyValues->Delete(i);
}

template <typename K, typename V>
void Dictionary<K, V>::Delete(Integer index)
{
	keyValues->Delete(index);
}

template <typename K, typename V>
void Dictionary<K, V>::Clear()
{
	keyValues->Clear();
}

template <typename K, typename V>
Boolean Dictionary<K, V>::DefaultSort(const Pair& a, const Pair& b) const
{
	return a.Key() < b.Key();
}

template <typename K, typename V>
void Dictionary<K, V>::Sort()
{
	auto c = std::bind(&Dictionary<K, V>::DefaultSort, this,
		std::placeholders::_1, std::placeholders::_2);
	keyValues->Sort(c);
}

template <typename K, typename V>
void Dictionary<K, V>::Sort(CompareFunc c)
{
	keyValues->Sort(c);
}

namespace DictionarySpace
{

extern threadvar Pointer find;

}

template <typename K, typename V>
Boolean Dictionary<K, V>::DefaultFind(const Pair& pair) const
{
	return pair.Key() == *(K*)DictionarySpace::find;
}

template <typename K, typename V>
Integer Dictionary<K, V>::Find(FindFunc f) const
{
	return keyValues->Find(f);
}

template <typename K, typename V>
Integer Dictionary<K, V>::IndexOf(const K& key) const
{
	DictionarySpace::find = (Pointer)&Var(key);
	auto f = std::bind(&Dictionary<K, V>::DefaultFind, this, std::placeholders::_1);
	return keyValues->Find(f);
}

template <typename K, typename V>
const K& Dictionary<K, V>::Keys(Integer index) const
{
	return keyValues->Item(index).Key();
}

template <typename K, typename V>
const V& Dictionary<K, V>::Values(Integer index) const
{
	return keyValues->Item(index).Value();
}

template <typename K, typename V>
V& Dictionary<K, V>::Item(const K& key)
{
	return (*this)[key];
}

template <typename K, typename V>
const V& Dictionary<K, V>::Item(const K& key) const
{
	return (*this)[key];
}

template <typename K, typename V>
V& Dictionary<K, V>::operator [] (const K& key)
{
	Integer i = IndexOf(key);
	if (i < 0)
		ThrowNoKeyException(ThisMethod);
	return keyValues->Item(i).Value();
}

template <typename K, typename V>
const V& Dictionary<K, V>::operator [] (const K& key) const
{
	Integer i = IndexOf(key);
	if (i < 0)
		ThrowNoKeyException(ThisMethod);
	return keyValues->Item(i).Value();
}

template <typename K, typename V>
Cardinal Dictionary<K, V>::Length() const
{
	return keyValues->Length();
}

// Dictionary<K, V>::IEnumerable<KeyValue<K, V>>

template <typename K, typename V>
IEnumerator<KeyValue<K, V>>* Dictionary<K, V>::GetEnumerator() const
{
	return keyValues->GetEnumerator();
}

}	
}
