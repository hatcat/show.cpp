#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class circle : public show::slide
	{
	public:
		circle(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	circle::circle(show::presentation p)
		: show::slide(p)
	{}

	bool circle::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry (17).png");
		return true;
	}

	void circle::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		// Draw the axes, then draw the line. x = 180 -> 1710, y = 970 -> 230, line = 665,970 -> 1035, 230
		auto x_axis = path_builder{};
		x_axis.clear();

		auto y_axis = path_builder{};
		y_axis.clear();

		auto circle_path = path_builder{};
		circle_path.clear();
		circle_path.new_figure(point_2d{ 1180.f, 600.f });

		auto axis_brush = brush{ rgba_color::cornflower_blue };
		auto line_brush = brush{ rgba_color::green };

		if (time_in_slide <= 1000)
		{
			auto fraction = (time_in_slide) / 1000.f;

			auto x1 = show::delta(945.f, 180.f, fraction);
			auto x2 = show::delta(945.f, 1710.f, fraction);

			x_axis.new_figure(point_2d{ x1, 600.f });
			x_axis.line(point_2d{ x2, 600.f });

			auto y1 = show::delta(600.f, 230.f, fraction);
			auto y2 = show::delta(600.f, 970.f, fraction);
			y_axis.new_figure(point_2d{ 930.f, y1 });
			y_axis.line(point_2d{ 930.f, y2 });
		}
		else if (time_in_slide <= 2000)
		{
			x_axis.new_figure(point_2d{ 180.f, 600.f });
			x_axis.line(point_2d{ 1710.f, 600.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			auto fraction = (time_in_slide - 1000.f) / 1000.f;
			auto theta = show::delta(0.f, two_pi<float>, fraction);
			circle_path.arc(point_2d{ 250.f, 250.f }, theta, 0.f);
		}
		else
		{
			x_axis.new_figure(point_2d{ 180.f, 600.f });
			x_axis.line(point_2d{ 1710.f, 600.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			circle_path.arc(point_2d{ 250.f, 250.f }, two_pi<float>, 0.f);
		}
		uos.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, circle_path, std::nullopt, stroke_props{ 4 });
	}

	bool circle::exit()
	{
		return true;
	}

	circle s017{ show::presentation::SLIDE_017 };
}

