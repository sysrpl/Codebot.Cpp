#include <Codebot/Packages/IO.h>
#include <Codebot/Net/WebRequest.h>
#include <Codebot/Net/MimeTypes.h>
#include <Codebot/Text/StringStream.h>

namespace Codebot
{
namespace Net
{

using namespace Text;
using namespace IO;

WebRequest::WebRequest()
	: request(), complete(false)
{
	lines = New<StringList>();
}

String WebRequest::FindNamedItem(const String& name)
{
	static String colon = ":";
	if (!complete)
		return String::Empty;
	Integer length = lines->Length();
	for (Integer i = 1; i < length; i++)
	{
		auto items = lines->Item(i).SplitOne(colon);
		if (items.Length() != 2)
			continue;
		String s = items[0].ToUpper();
		if (s == name)
			return items[1].Trim();
	}
	return String::Empty;
}

void WebRequest::Reset()
{
	request = String::Empty;
	lines->Clear();
	complete = false;
}

Boolean WebRequest::Add(const String& text)
{
	static String nn = "\n\n";
	static String rr = "\r\r";
	static String rn = "\r\n\r\n";
	static String nr = "\n\r\n\r";
	if (complete)
		return false;
	request += text;
	complete = request.EndsWith(nn) || request.EndsWith(rr) || 
		request.EndsWith(rn) || request.EndsWith(nr);
	if (complete)
		lines->Text(request);
	request = String::Empty;
	return true;
}

Boolean GetFile(const String fileName, String& contents)
{
	try
	{
		auto fileStream = New<FileStream>(fileName, FileMode::Read);
		auto stringStream = New<StringStream>();
		stringStream->Assign(fileStream);
		contents = stringStream->ToString();
	}
	catch (...)
	{
		return false;
	}
	return true;
}

Integer Response404(String& response)
{
	static String NotFound =
		"HTTP/1.1 404 NOT FOUND\nConnection: close\n\n";
	response = NotFound;
	return 404;
}

Integer WebRequest::BuildResponse(String& response)
{
	static String GET = "GET";
	static String Ok =
		"HTTP/1.1 200 OK\nConnection: close\n"
		"Content-Type: {0}\nContent-Length: {1}\n\n";
	static String DefaultDoc = "default.htm";
	static String InvalidChars = "~`$%^{}[],:'\"";
	String verb = Verb();
	if (verb != GET)
		return Response404(response);
	String resource = Resource();
	if (resource.IsEmpty())
		return Response404(response);
	// Clean up request resource 
	resource = resource.Replace("/", Path::Separator);
	if (resource.EndsWith(Path::Separator))
		resource = Path::Combine(resource, DefaultDoc);
	if (resource.BeginsWith(Path::Separator))
		resource = resource.SubString(1);
	// Prevent the resource from being outside webroot
	if (resource.BeginsWith(Path::Separator))
		return Response404(response);
	if (resource.ContainsAny(InvalidChars))
		return Response404(response);
	if (resource.Contains(".."))
		return Response404(response);
	// Check if resource is a directory or file
	if (Directory::Exists(resource))
		resource = Path::Combine(resource, DefaultDoc);
	if (!File::Exists(resource))
		return Response404(response);
	// Check content type
	String extension = resource.LastOf(".");
	if (extension.Length() == 0)
		return Response404(response);
	auto mimeTypes = GetMimeTypes();
	auto i = mimeTypes->IndexOf(extension);
	if (i < 0)
		return Response404(response);
	String contentType = mimeTypes->Item(extension);
	String content;
	if (GetFile(resource, content))
	{
		response = Format(Ok, contentType, content.Length()) + content;
		return 200;
	}
	else
		return Response404(response);
}

String WebRequest::Verb()
{
	if (!complete)
		return String::Empty;
	if (lines->Length() < 1)
		return String::Empty;
	String s = lines->Item(0);
	auto items = s.Split(" ");
	if (items.Length() < 1)
		return String::Empty;
	return items[0];
}

String WebRequest::Resource()
{
	if (!complete)
		return String::Empty;
	if (lines->Length() < 1)
		return String::Empty;
	String s = lines->Item(0);
	auto items = s.Split(" ");
	if (items.Length() < 2)
		return String::Empty;
	return items[1];
}

String WebRequest::UserAgent()
{
	static String userAgent = "USER-AGENT";
	return FindNamedItem(userAgent);
}

Boolean WebRequest::Complete()
{
	return complete;
}

}
}
