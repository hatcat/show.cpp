#pragma once

#include "Show.h"

#include <io2d.h>

#include <vector>

namespace show
{
	using namespace std::experimental::io2d;

	class slide
	{
	public:
		slide(presentation name);
		virtual ~slide();
		virtual bool enter() = 0;
		virtual void render(unmanaged_output_surface&) = 0;
		virtual bool exit() = 0;
	};

	std::vector<slide*>& slide_show();
}