#include "show.h"
#include "slide.h"
#include "Window.h"

#include <iostream>
#include <string>

using namespace std;
using namespace std::chrono;
using namespace std::experimental::io2d;

namespace
{
	std::string res_root_from_cmd_line(std::string const& cmd_line)
	{
		auto start = cmd_line.find("-res_root=\"");
		if (start == std::string::npos)
		{
			return {};
		}
		start += 11;
		auto end = cmd_line.find("\"", start);
		if (end == std::string::npos)
		{
			return {};
		}
		return cmd_line.substr(start, end - start);
	}

////////////////////////////////////////////////////////////////////////////////
// Input
////////////////////////////////////////////////////////////////////////////////

	auto next_key = 'X';
	auto prev_key = 'Z';
	auto repeat_key = 'M';
	auto next_down = false;
	auto prev_down = false;
	auto repeat_down = false;

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
		bool repeat = false;
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

	bool repeat()
	{
		return bounce_check(press().repeat, repeat_down);
	}

	void get_key_state()
	{
		press() = { GetKeyState(next_key) < 0, GetKeyState(prev_key) < 0, GetKeyState(repeat_key) < 0 };
	}

////////////////////////////////////////////////////////////////////////////////
// Navigation
////////////////////////////////////////////////////////////////////////////////

	class navigate
	{
	public:
		navigate(show::presentation);
		int current_slide() const;
		void check(show::show const&);
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

	void navigate::check(show::show const& s)
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
				show::slide_show()[show::presentation(m_current_slide)]->enter(s);
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
				show::slide_show()[show::presentation(m_current_slide)]->enter(s);
			}
		}
		else if (repeat())
		{
			show::slide_show()[show::presentation(m_current_slide)]->enter(s);
		}
	}
}

show::show::show(std::string const& cmd_line)
	: m_res_root(res_root_from_cmd_line(cmd_line))
{}

void show::show::update(unmanaged_output_surface& s)
{
	static auto first_entry = slide_show()[presentation(nav.current_slide())]->enter(*this);

	get_key_state();
	nav.check(*this);
	slide_show()[presentation(nav.current_slide())]->render(s);
}

std::string const& show::show::res_root() const
{
	return m_res_root;
}

int CALLBACK WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPSTR cmd_line, _In_ int)
{
	show::Win32Win s(hInst, cmd_line);
	return s.Run();
}
