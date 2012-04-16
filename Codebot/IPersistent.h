#pragma once
#include "Codebot/Stream.h"

namespace Codebot
{

// IPersistent

class IPersistent
{
public:
	virtual ~IPersistent() { }
	virtual void Load(const String& fileName) = 0;
	virtual void Load(Stream* stream) = 0;
	virtual void Save(const String& fileName) const = 0 ;
	virtual void Save(Stream* stream) const = 0;
};

}
