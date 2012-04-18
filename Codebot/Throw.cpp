#include <Codebot/Throw.h>
#include <Codebot/Exception.h>
#include <Codebot/Text/Format.h>

namespace Codebot
{

using namespace Text;

const PChar SCannotCopyObject = "Objects cannot be copied";
const PChar SCastFailed = "Could not cast to '{0}'";
const PChar SNullReference = "Identifier '{0}' references null value";
const PChar SInvalidArgument = "Argument '{0}' is invalid";
const PChar SCannotAssign = "Cannot assign'{0}' to '{1}'";
const PChar SIndexRange = "Index is out of range";

void Abort()
{
	throw AbortException(String::Empty, String::Empty);
}

void ThrowCopyObjectException(const PChar method)
{
	throw CopyObjectException(method, SCannotCopyObject);
}

void ThrowException(const PChar method, const PChar message)
{
	throw Exception(method, message);
}

void ThrowCastException(const PChar method, const PChar type)
{
	throw CastException(method, Format(SCastFailed, type));
}

void ThrowAssignException(const PChar method, const PChar source, const PChar dest)
{
	throw AssignException(method, Format(SCannotAssign, source, dest));
}

void ThrowArgumentException(const PChar method, const PChar argument)
{
	throw ArgumentException(method, Format(SInvalidArgument, argument));
}

void ThrowNullReferenceException(const PChar method, const PChar ident)
{
	throw NullReferenceException(method, Format(SNullReference, ident));
}

void ThrowRangeException(const PChar method)
{
	throw RangeException(method, SIndexRange);
}

}
