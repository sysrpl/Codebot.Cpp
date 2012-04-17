#include "Init.h"

namespace DictionaryTestsSpace
{
	typedef Dictionary<String, Double> Cart;

	Double price;

	Boolean FindPrice(const Cart::Pair& item)
	{
		return item.Value() == price;
	}

	Boolean ComparePrice(const Cart::Pair& a, const Cart::Pair& b)
	{
		return a.Value() < b.Value();
	}

	Ref<Cart> a;
}

void DictionaryTests(UnitTest& tests)
{
	using namespace DictionaryTestsSpace;

	a = New<Cart>();
	AddTestGroup("Dictionary tests");
	AddTestItem("ctor()",
		return a->Length() == 0;
	);
	AddTestItem("add",
		a->Add("steak", 4.25);
		a->Add("lamb", 3.55);
		a->Add("milk", 2.40);
		a->Add("bread", 1.25);
		a->Add("lettuce", 2.50);
		a->Add("cheese", 4.80);
		return a->Length() == 6;
	);
	AddTestItem("remove",
		a->Remove("lettuce");
		return a->Length() == 5;
	);
	AddTestItem("delete",
		a->Delete(1);
		return (a->Length() == 4);
	);
	AddTestItem("key",
		return a->Keys(2) == "bread";
	);
	AddTestItem("values",
		return a->Values(1) == 2.4;
	);
	AddTestItem("item",
		return a->Item("cheese") == 4.8;
	);
	AddTestItem("item[]",
		return (*a)["cheese"] == 4.8;
	);
	AddTestItem("find",
		return a->IndexOf("bread") == 2;
	);
	AddTestItem("findfunc",
		price = 4.8;
		return a->Find(FindPrice) == 3;
	);
	AddTestItem("sort",
		a->Sort();
		return (*a)["cheese"] == 4.8;
	);
	AddTestItem("sortfunc",
		a->Sort(ComparePrice);
		return a->Keys(0) == "bread";
	);
	AddTestItem("foreach",
		Double amount = 0;
		Cart& cart = *a;
		ForEach(item, cart)
			amount += item->Value();
		return amount == 12.7;
	);
	AddTestItem("clear",
		a->Clear();
		return a->Length() == 0;
	);
}
