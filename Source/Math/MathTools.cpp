#include <cmath>
#include <Codebot/Math/MathTools.h>

namespace Codebot
{
namespace Math
{

const Double PI = std::atan(1.0) * 4.0;

inline Double Pow(const Double& d, const Double& e)
{
	return std::pow(d, e);
}

inline Double Sin(const Double& d)
{
	return std::sin(d);
}

inline Double Cos(const Double& d)
{
	return std::cos(d);
}

inline Double Tan(const Double& d)
{
	return std::tan(d);
}

inline Double Asin(const Double& d)
{
	return std::asin(d);
}

inline Double Acos(const Double& d)
{
	return std::acos(d);
}

inline Double Atan(const Double& d)
{
	return std::atan(d);
}

}
}
