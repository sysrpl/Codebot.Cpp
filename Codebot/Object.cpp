#include "Codebot/Object.h"
#include "Codebot/Interop/CoreInterop.h"

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

void Object::_Lock()
{
	Codebot::Interop::AtomicIncrement(objectCount);	
	Codebot::Interop::AtomicIncrement(count);
}

void Object::_Unlock()
{
	Codebot::Interop::AtomicDecrement(objectCount);	
	if (Codebot::Interop::AtomicDecrement(count) == 0)
		delete this;
}

}
