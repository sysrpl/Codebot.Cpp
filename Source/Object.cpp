#include <Codebot/Object.h>
#include <Codebot/Interop/CoreInterop.h>

namespace Codebot
{

Integer objectCount = 0;

Object::Object()
	: count(0)
{
}

Object::Object(const Object& object)
{
	ThrowCopyObjectException(ThisMethod);
}

Object& Object::operator = (const Object& object)
{
	ThrowCopyObjectException(ThisMethod);
	return *this;
}

void Object::Assign(Object* source)
{
	if (source == null)
		ThrowNullReferenceException(ThisMethod, "source");
	AssignTo(this);
}

void Object::AssignTo(Object* dest)
{
	ThrowAssignException(ThisMethod, TypeToStr(*this), TypeToStr(*dest));
}

Integer Object::RefCount()
{
	return count;
}

Integer Object::_Lock()
{
	Interop::AtomicIncrement(objectCount);
	return Interop::AtomicIncrement(count);
}

Integer Object::_Unlock()
{
	Interop::AtomicDecrement(objectCount);
	Integer c = Interop::AtomicDecrement(count);
	if (c == 0)
		delete this;
	return c;
}

}
