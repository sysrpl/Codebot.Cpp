#pragma once
#include "Codebot/Collections/List.h"
#include "Codebot/IPersistent.h"

namespace Codebot
{
namespace Text
{

class StringList : public Collections::List<String>, public IPersistent, public ISerializable
{
public:
	StringList();
	// Properties
	String Text() const;
	void Text(const String& value);
	// IPersistent
	void Load(const String& fileName);
	void Load(Stream* stream);
	void Save(const String& fileName) const;
	void Save(Stream* stream) const;
	// ISerializable
	String ToFormat(const String& format) const;
	Boolean FromFormat(const String& value, const String& format);
};

}
}
