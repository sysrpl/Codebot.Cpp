#pragma once
#include <Codebot/ValueType.h>

namespace Codebot
{
namespace Time
{

class Timer : public ValueType
{
private:
	Double frequency;
	LargeWord origin;
	LargeWord current;
	Double elapsed;
	Double time;
	LargeWord frame;
public:
	Timer();
	void Update();
	Double Elapsed() const;
	Double Time() const;
	Integer Fps() const;
	LargeWord Frame() const;
};

}
}
