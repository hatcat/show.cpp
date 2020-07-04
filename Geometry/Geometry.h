#pragma once

#include <show.h>

#include <string>

namespace pres
{
	inline auto res = std::string{ "C:\\Users\\elgui\\source\\repos\\hatcat\\show.cpp\\Geometry\\res\\" };
	inline auto root(const show::show& s)
	{
		return s.res_root().empty() ? res : s.res_root();
	}
}