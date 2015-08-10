#pragma once
#include <Codebot/Ref.h>
#include <Codebot/String.h>

namespace Codebot
{

extern Integer objectCount;

class Object : public ValueType, public IManagedType
{
private:
	Integer count;
	// The following methods throw CopyObjectException
	Object(const Object& object); 
	Object& operator = (const Object& object); 
protected:
	Integer RefCount();
	virtual void AssignTo(Object* dest);
public:
	Object();
	virtual void Assign(Object* source);
	Integer _Lock();
	Integer _Unlock();
};

}
