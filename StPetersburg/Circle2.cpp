#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class circle2 : public show::slide
	{
	public:
		circle2(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	circle2::circle2(show::presentation p)
		: show::slide(p)
	{}

	bool circle2::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry St Petersburg (18).png");
		return true;
	}

	void circle2::render(unmanaged_output_surface& uos)
	{
		m_bg.render(uos);

		auto x_axis = path_builder{};
		auto y_axis = path_builder{};
		auto circle_path = path_builder{};
		auto axis_brush = brush{ rgba_color::cornflower_blue };
		auto line_brush = brush{ rgba_color::green };

		x_axis.clear();
		x_axis.new_figure(point_2d{ 180.f, 600.f });
		x_axis.line(point_2d{ 1710.f, 600.f });

		y_axis.clear();
		y_axis.new_figure(point_2d{ 930.f, 230.f });
		y_axis.line(point_2d{ 930.f, 970.f });

		circle_path.clear();
		circle_path.new_figure(point_2d{ 1180.f, 600.f });
		circle_path.arc(point_2d{ 250.f, 250.f }, two_pi<float>, 0.f);
		uos.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, circle_path, std::nullopt, stroke_props{ 4 });
	}

	bool circle2::exit()
	{
		return true;
	}

	circle2 s018{ show::presentation::SLIDE_018 };
}

