#pragma once
#include "Codebot/Packages/System.h"
#include "Codebot/NotifyEvent.h"
#include "Codebot/Property.h"

using namespace Codebot;
using namespace Text;
using namespace Time;

class Person : ValueType
{
private:
	static const String DefaultName;
	String name;
	Nullable<DateTime> birthDate;
	// Property access
	String GetName() const
	{
		return name;
	}

	void SetName(const String& value)
	{
		String s = value.IsBlank() ? DefaultName : value.Trim();
		if (name != s)
			name = s;
	}

	Nullable<DateTime>& GetBirthDate() const
	{
		return Var(birthDate);
	}

	void SetBirthDate(const Nullable<DateTime>& value)
	{
		birthDate(value);
	}

	Nullable<int> GetAge() const
	{
		if (birthDate.HasValue())
		{
			DateTime now = DateTime::Now();
			if (birthDate > now)
				return null;
			return now.YearDifference(birthDate);
		}
		else
			return null;
	}
public:
	Person(const String& n = DefaultName) :
		name(n),
		CreateProp(Person, Name),
		CreateProp(Person, BirthDate),
		CreateConstProp(Person, Age)
	{
	}
	// Override methods
	String ToFormat(const String& format) const
	{
		if (format == "age")
			return FormatValue(Age, "");
		if (format == "dob")
			return FormatValue(BirthDate, "MMMM D YYYY");
		return FormatValue(Name, "trim");
	}
	// Properties
	Property<Person, String> Name;
	Property<Person, Nullable<DateTime>, true> BirthDate;
	const Property<Person, Nullable<int>> Age;
};

const String Person::DefaultName = "John Doe";

int TestProp()
{
	Person p;
	p.Name = "John Walker";
	p.BirthDate = DateTime(1980, 7, 21);
	WriteLine("{0} was born on {1:MMMM D YYYY}\n"
		"{0} is {2} years old", p.Name, p.BirthDate, p.Age);
	WriteLine("{0} was born on {0:dob}\n{0} is {0:age} years old", p);
	return 0;
}
