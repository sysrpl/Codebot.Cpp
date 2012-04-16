#include "Codebot/ValueType.h"
#include "Codebot/String.h"
#include "Codebot/Exception.h"

namespace Codebot
{

Boolean ValueType::operator == (const ValueType& instance) const
{
	return Equals(&Var(instance));
}

Boolean ValueType::Equals(ValueType* instance) const
{
	return instance == this;
}

String ValueType::ToFormat(const String& format) const
{
	return ClassName();
}

String ValueType::ClassName() const
{
	return TypeToStr(*this);
}

String ValueType::ToString() const
{
	return ToFormat(String::Empty);
}

}
