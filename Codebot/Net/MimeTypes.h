#pragma once
#include "Codebot/Collections/Dictionary.h"

namespace Codebot
{
namespace Net
{

typedef Collections::Dictionary<String, String> MimeTypes;

Ref<MimeTypes> GetMimeTypes();

}
}
