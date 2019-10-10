#include "slide.h"
#include "Window.h"

#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace std::experimental::io2d;

namespace
{
////////////////////////////////////////////////////////////////////////////////
// Input
////////////////////////////////////////////////////////////////////////////////

	auto next_key = 'X';
	auto prev_key = 'Z';
	auto next_down = false;
	auto prev_down = false;

	bool bounce_check(const bool& key, bool& down)
	{
		if (key && !down)
		{
			down = true;
			return true;
		}
		down = key;
		return false;
	}

	struct keypress
	{
		bool next = false;
		bool prev = false;
	};

	keypress& press()
	{
		static keypress k;
		return k;
	}

	bool next()
	{
		return bounce_check(press().next, next_down);
	}

	bool prev()
	{
		return bounce_check(press().prev, prev_down);
	}

	void get_key_state()
	{
		press() = { GetKeyState(next_key) < 0, GetKeyState(prev_key) < 0 };
	}

////////////////////////////////////////////////////////////////////////////////
// Navigation
////////////////////////////////////////////////////////////////////////////////

	class navigate
	{
	public:
		navigate(show::presentation);
		int current_slide() const;
		void check();
	private:
		int m_current_slide;
	};

	navigate nav(show::presentation::SLIDE_001);

	navigate::navigate(show::presentation id)
		: m_current_slide(id)
	{}

	int navigate::current_slide() const
	{
		return m_current_slide;
	}

	void navigate::check()
	{
		if (next())
		{
			if (++m_current_slide == show::presentation::SLIDE_LAST)
			{
				--m_current_slide;
			}
			else
			{
				show::slide_show()[show::presentation(m_current_slide - 1)]->exit();
				show::slide_show()[show::presentation(m_current_slide)]->enter();
			}
		}
		else if (prev())
		{
			if (m_current_slide-- == show::presentation::SLIDE_FIRST)
			{
				++m_current_slide;
			}
			else
			{
				show::slide_show()[show::presentation(m_current_slide + 1)]->exit();
				show::slide_show()[show::presentation(m_current_slide)]->enter();
			}
		}
	}
}

void show::show::update(unmanaged_output_surface& s)
{
	static auto first_entry = slide_show()[presentation(nav.current_slide())]->enter();
	static auto last_update = std::chrono::steady_clock::now();
	auto now = std::chrono::steady_clock::now();

	get_key_state();
	nav.check();
	slide_show()[presentation(nav.current_slide())]->render(s);
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	show::Win32Win s(hInst);
	return s.Run();
}
