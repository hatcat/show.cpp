#pragma once

#include <io2d.h>

#include <set>

class slide
{
public:
	using slide_fn = void(*)(std::experimental::io2d::output_surface&);
	slide(slide_fn, const std::string& name);
};

std::set<std::string, slide::slide_fn> slides;