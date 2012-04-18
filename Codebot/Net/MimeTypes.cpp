#include <Codebot/Net/MimeTypes.h>

namespace Codebot
{
namespace Net
{

Ref<MimeTypes> mimeTypes = null;

Ref<MimeTypes> GetMimeTypes()
{
	if (mimeTypes.HasValue())
		return mimeTypes;
	mimeTypes = New<MimeTypes>();
	mimeTypes->Add("css", "text/css");
	mimeTypes->Add("htm", "text/html");
	mimeTypes->Add("html", "text/html");
	mimeTypes->Add("txt", "text/plain");
	mimeTypes->Add("xml", "text/xml");
	mimeTypes->Add("png", "image/png");
	mimeTypes->Add("jpg", "image/jpeg");
	mimeTypes->Add("jpeg", "image/jpeg");
	mimeTypes->Add("gif", "image/gif");
	mimeTypes->Add("js", "application/javascript");
	mimeTypes->Add("ico", "image/x-icon");
	return mimeTypes;
}

}
}
