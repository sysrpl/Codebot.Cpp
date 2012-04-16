#pragma once
#include "Codebot/Collections/List.h"

namespace Codebot
{
namespace Collections
{

template <typename T>
class ObjectList : public List<Ref<T>>
{
};

}
}
