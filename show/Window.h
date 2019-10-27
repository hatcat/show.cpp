#pragma once

#include "Show.h"
#include <io2d.h>

namespace show
{
	using namespace std::experimental::io2d;

	LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM);

	class Win32Win
	{
	public:
		Win32Win(HINSTANCE hinst, LPSTR cmd_line, format fmt = format::argb32, scaling scl = scaling::letterbox);

		LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int Run();

	private:
		friend LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM);

		void		OnWmCreate(HWND hwnd);
		void		UpdateBorder();

		HINSTANCE	m_hInstance = nullptr;
		HWND		m_hwnd = nullptr;
		HDC			m_hdc = nullptr;
		bool		m_border = false;

		bool		m_canDraw = false;
		int			m_x;
		int			m_y;
		int			m_w;
		int			m_h;
		format		m_fmt;
		scaling		m_scl;

		show		m_show;
		std::experimental::io2d::unmanaged_output_surface	m_outputSfc;
	};
}
