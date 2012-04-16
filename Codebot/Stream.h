#pragma once
#include "Codebot/Object.h"

namespace Codebot
{

enum class StreamOrigin
{
	Begin,
	Current,
	End
};

class Stream : public Object, public ISerializable
{
public:
	virtual void Clear();
	virtual Cardinal Read(Pointer data, Cardinal count) = 0;
	virtual Cardinal Write(Pointer data, Cardinal count) = 0;
	virtual Cardinal Seek(StreamOrigin origin, Integer cursor) = 0;
	Cardinal Position();
	void Position(Cardinal value);
	virtual Cardinal Size();
	virtual void Size(Cardinal value);
	void Assign(Object* source);
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
