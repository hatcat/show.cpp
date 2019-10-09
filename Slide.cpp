#include "Slide.h"

show::slide::slide(presentation id)
{
	slide_show()[id] = this;
}

show::slide::~slide() {}

std::vector<show::slide*>& show::slide_show()
{
	static std::vector<slide*> slides{ SLIDE_LAST };
	return slides;
}