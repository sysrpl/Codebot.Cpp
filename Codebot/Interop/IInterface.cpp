#include <Codebot/Interop/IInterface.h>

namespace Codebot
{
namespace Interop
{

const Result ROk = 0;
const Result RFalse = 0x00000001;
const Result RNoInterface = 0x80004002;
const Result RUnexpected = 0x8000FFFF;
const Result RNotImplemented = 0x80004001;

const Guid IInterface::Id = "{00000000-0000-0000-C000-000000000046}";

}
}
