#include <Codebot/Collections/CollectionsExceptions.h>

namespace Codebot
{
namespace Collections
{

const PChar SDuplicateItem = "Collection does not allow duplicate items";
const PChar SNoKey = "Collections does not contain specified key";

void ThrowDuplicateItemException(const String& method)
{
	throw CollectionItemException(method, SDuplicateItem);
}

void ThrowNoKeyException(const String& method)
{
	throw CollectionItemException(method, SNoKey);
}

}
}
