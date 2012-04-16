#pragma once
#include "Codebot/Object.h"
#include "Codebot/Text/StringList.h"

namespace Codebot
{
namespace Net
{

class WebRequest : public Object
{
private:
	String request;
	Ref<Text::StringList> lines;
	Boolean complete;
	String FindNamedItem(const String& name);
public:
	WebRequest();
	void Reset();
	Boolean Add(const String& text);
	Integer BuildResponse(String& response);
	String Verb();
	String Resource();
	String UserAgent();
	Boolean Complete();
};

}
}
