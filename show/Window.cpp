#include "Window.h"
#include <mutex>
#include <system_error>
#include <charconv>

namespace
{
	static std::once_flag winClassRegistered;
	const wchar_t* winClassName = L"Show.cpp_WinClass";

	[[nodiscard]]
	int int_from_cmd_line(std::string const& cmd_line, std::string const& token)
	{
		auto start = cmd_line.find(token);
		if (start == std::string::npos)
		{
			return 0;
		}
		return atoi(cmd_line.c_str() + start + token.size());
	}

	[[nodiscard]]
	int x_from_cmd_line(std::string const& cmd_line)
	{
		return int_from_cmd_line(cmd_line, "-x=");
	}

	[[nodiscard]]
	int y_from_cmd_line(std::string const& cmd_line)
	{
		return int_from_cmd_line(cmd_line, "-y=");
	}

	[[nodiscard]]
	int w_from_cmd_line(std::string const& cmd_line)
	{
		return int_from_cmd_line(cmd_line, "-w=");
	}

	[[nodiscard]]
	int h_from_cmd_line(std::string const& cmd_line)
	{
		return int_from_cmd_line(cmd_line, "-h=");
	}

	[[noreturn]]
	void throw_system_error_for_GetLastError(DWORD getLastErrorValue, const char* message) {
		if (message != nullptr) {
			// Updated Note: Changed to static_cast due to 4.7F/3 [conv.integral]. // Old Note: C-style cast because system_error requires an int but GetLastError returns a DWORD (i.e. unsigned long) but ordinary WinError.h values never exceed the max value of an int.
			throw std::system_error(static_cast<int>(getLastErrorValue), std::system_category(), message);
		}
		else {
			throw std::system_error(static_cast<int>(getLastErrorValue), std::system_category());
		}
	}

	void RegisterWindowClass() {
		::std::call_once(winClassRegistered, []() {
			WNDCLASSEX wcex{};

			wcex.cbSize = sizeof(WNDCLASSEX);
			// We want to keep a DC so we don't have to constantly recreate the native cairo device.
			// We want CS_HREDRAW and CS_VREDRAW so we get a refresh of the whole window if the client area changes due to movement or size adjustment.
			wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = show::DefaultWindowProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(LONG_PTR);
			wcex.hInstance = static_cast<HINSTANCE>(GetModuleHandleW(nullptr));
			wcex.hIcon = static_cast<HICON>(nullptr);
			wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
			wcex.lpszMenuName = static_cast<LPCWSTR>(nullptr);
			wcex.lpszClassName = winClassName;
			wcex.hIconSm = static_cast<HICON>(nullptr);

			return RegisterClassEx(&wcex);
			});
	}
}

LRESULT CALLBACK show::DefaultWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	LONG_PTR objPtr = GetWindowLongPtr(hwnd, 0);
	if (msg == WM_CREATE) {
		CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
		// Return 0 to allow the window to proceed in the creation process.
		static_cast<Win32Win*>(create->lpCreateParams)->OnWmCreate(hwnd);
		return static_cast<LRESULT>(0);
	}
	if (objPtr == 0) {
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
	else {
		// Run the class-specific window proc code
		auto win32Win = reinterpret_cast<Win32Win*>(objPtr);
		return win32Win->WindowProc(hwnd, msg, wParam, lParam);
	}
}

show::Win32Win::Win32Win(HINSTANCE hinst, LPSTR cmd_line, format fmt, scaling scl)
	: m_hInstance(hinst)
	, m_hwnd(nullptr)
	, m_hdc(nullptr)
	, m_x(x_from_cmd_line(cmd_line))
	, m_y(y_from_cmd_line(cmd_line))
	, m_w(w_from_cmd_line(cmd_line))
	, m_h(h_from_cmd_line(cmd_line))
	, m_fmt(fmt)
	, m_scl(scl)
	, m_show(cmd_line)
	, m_outputSfc(default_graphics_surfaces::surfaces::create_unmanaged_output_surface())
{
	RegisterWindowClass();
}

LRESULT CALLBACK show::Win32Win::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	const static auto lrZero = static_cast<LRESULT>(0);
	switch (msg) {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunreachable-code-break"
#endif
	case WM_CLOSE:
	{
		m_canDraw = false;
		// This message is sent when a window or an application should
		// terminate.
		if (!DestroyWindow(hwnd)) {
			throw_system_error_for_GetLastError(GetLastError(), "Failed call to DestroyWindow when processing WM_CLOSE.");
		}
		m_hwnd = nullptr;
		m_outputSfc.data()->data.hwnd = m_hwnd;
		return lrZero;
	} break;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunreachable-code-break"
#endif
	case WM_DESTROY:
	{
		m_canDraw = false;
		// This message is sent when a window has been destroyed.
		PostQuitMessage(0);
		return lrZero;
	} break;
#ifdef __clang__
#pragma clang diagnostic pop
#endif

	case WM_SIZE:
	{
		m_outputSfc.display_dimensions(display_point(LOWORD(lParam), HIWORD(lParam)));
	} break;

	case WM_CHAR:
	{
		if (wParam == '5')
		{
			m_border = !m_border;
			UpdateBorder();
		}
	} break;

	case WM_PAINT:
	{
		if (m_canDraw) {
			PAINTSTRUCT ps;

			BeginPaint(hwnd, &ps);
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			const auto& data = *m_outputSfc.data();
			if (width != data.data.display_dimensions.x() || height != data.data.display_dimensions.y()) {
				//if (width != m_outputSfc.display_dimensions().x() || height != m_outputSfc.display_dimensions().y()) {
				// If there is a size mismatch we skip painting and resize the window instead.
				EndPaint(hwnd, &ps);
				m_outputSfc.display_dimensions(display_point{ width, height });
				break;
			}

			if (m_outputSfc.has_draw_callback() && m_canDraw) {
				m_outputSfc.clear();
				m_outputSfc.invoke_draw_callback();
				m_outputSfc.draw_to_output();
			}

			EndPaint(hwnd, &ps);
		}
	} break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);

}

int show::Win32Win::Run()
{
	const wchar_t* winTitle = L"Show.cpp";

	// Create an instance of the window
	m_hwnd = CreateWindowEx(
		static_cast<DWORD>(0),				// extended style
		winClassName,						// class name
		winTitle,							// instance title
		0,									// window style
		m_x, m_y,							// initial x, y
		m_w, m_h,							// initial width, height
		static_cast<HWND>(nullptr),			// handle to parent
		static_cast<HMENU>(nullptr),		// handle to menu
		static_cast<HINSTANCE>(nullptr),	// instance of this application
		static_cast<LPVOID>(this));			// extra creation parms

	if (m_hwnd == nullptr) {
		throw_system_error_for_GetLastError(GetLastError(), "Failed call to CreateWindowEx.");
	}

	SetLastError(ERROR_SUCCESS);
	if (SetWindowLongPtr(m_hwnd, GWL_STYLE, 0) == 0) {
		// SetWindowLongPtr is weird in terms of how it fails. See its documentation. Hence this weird check.
		DWORD lastError = GetLastError();
		if (lastError != ERROR_SUCCESS) {
			throw_system_error_for_GetLastError(lastError, "Failed call to SetWindowLongPtr(HWND, int, LONG_PTR)");
		}
	}

	// Initially display the window
	ShowWindow(m_hwnd, SW_SHOWNORMAL);
	UpdateWindow(m_hwnd);

	MSG msg{};
	msg.message = WM_NULL;

	while (msg.message != WM_QUIT)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			RECT clientRect;
			GetClientRect(m_hwnd, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;
			const auto& data = *m_outputSfc.data();
			if (width != data.data.display_dimensions.x() || height != data.data.display_dimensions.y())
			{
				// If there is a size mismatch we skip painting and resize the window instead.
				auto displayDimensions = display_point{ width, height };
				m_outputSfc.display_dimensions(displayDimensions);

				if (display_point{ width, height } != data.data.display_dimensions)
				{
					if (m_outputSfc.has_size_change_callback())
					{
						m_outputSfc.invoke_size_change_callback();
					}
				}
				continue;
			}
			else
			{
				if (m_canDraw && m_outputSfc.has_draw_callback())
				{
					m_outputSfc.clear();
					m_outputSfc.invoke_draw_callback();
					m_outputSfc.draw_to_output();
				}
			}
		}
		else
		{
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return static_cast<int>(msg.wParam);
}

void show::Win32Win::OnWmCreate(HWND hwnd)
{
	SetLastError(ERROR_SUCCESS);
	if (SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(this)) == 0) {
		// SetWindowLongPtr is weird in terms of how it fails. See its documentation. Hence this weird check.
		DWORD lastError = GetLastError();
		if (lastError != ERROR_SUCCESS) {
			throw_system_error_for_GetLastError(lastError, "Failed call to SetWindowLongPtr(HWND, int, LONG_PTR) in cairo_display_surface::cairo_display_surface(int, int, format, int, int, scaling)");
		}
	}
	m_hwnd = hwnd;
	m_hdc = GetDC(m_hwnd);
	default_graphics_surfaces::surfaces::unmanaged_surface_context_type unmanaged_context;
	unmanaged_context.hInstance = m_hInstance;
	unmanaged_context.hwnd = m_hwnd;
	unmanaged_context.hdc = m_hdc;
	m_outputSfc = std::experimental::io2d::unmanaged_output_surface(default_graphics_surfaces::surfaces::create_unmanaged_output_surface(unmanaged_context, m_w, m_h, m_fmt, m_scl));
	m_outputSfc.display_dimensions(display_point{ m_w, m_h });
	m_outputSfc.draw_callback([&](std::experimental::io2d::unmanaged_output_surface& uos) { m_show.update(uos); });
	m_canDraw = true;
}

void show::Win32Win::UpdateBorder()
{
/*	if (m_border)
	{
		SetLastError(ERROR_SUCCESS);
		if (SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPED) == 0) {
			// SetWindowLongPtr is weird in terms of how it fails. See its documentation. Hence this weird check.
			DWORD lastError = GetLastError();
			if (lastError != ERROR_SUCCESS) {
				throw_system_error_for_GetLastError(lastError, "Failed call to SetWindowLongPtr(HWND, int, LONG_PTR)");
			}
		}
	}
	else
	{
		SetLastError(ERROR_SUCCESS);
		if (SetWindowLongPtr(m_hwnd, GWL_STYLE, 0) == 0) {
			// SetWindowLongPtr is weird in terms of how it fails. See its documentation. Hence this weird check.
			DWORD lastError = GetLastError();
			if (lastError != ERROR_SUCCESS) {
				throw_system_error_for_GetLastError(lastError, "Failed call to SetWindowLongPtr(HWND, int, LONG_PTR)");
			}
		}
	}*/
}