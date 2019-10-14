#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class line_segment : public show::slide
	{
	public:
		line_segment(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	line_segment::line_segment(show::presentation p)
		: show::slide(p)
	{}

	bool line_segment::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry St Petersburg (42).png");
		return true;
	}

	void line_segment::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		// Draw the axes, then draw the line. x = 180 -> 1710, y = 970 -> 230, line = 665,970 -> 1035, 230
		auto x_axis = path_builder{};
		x_axis.clear();

		auto y_axis = path_builder{};
		y_axis.clear();

		auto sample_line = path_builder{};
		sample_line.clear();

		auto axis_brush = brush{ rgba_color::cornflower_blue };
		auto line_brush = brush{ rgba_color::green };

		if (time_in_slide <= 1000)
		{
			auto fraction = (time_in_slide) / 1000.f;
			auto x1 = show::delta(945.f, 180.f, fraction);
			auto x2 = show::delta(945.f, 1710.f, fraction);
			auto y1 = show::delta(850.f, 230.f, fraction);
			auto y2 = show::delta(850.f, 970.f, fraction);

			x_axis.new_figure(point_2d{ x1, 850.f });
			x_axis.line(point_2d{ x2, 850.f });

			y_axis.new_figure(point_2d{ 930.f, y1 });
			y_axis.line(point_2d{ 930.f, y2 });
		}
		else if (time_in_slide <= 2000)
		{
			auto fraction = (time_in_slide - 1000.f) / 1000.f;
			auto x = show::delta(650.f, 1500.f, fraction);
			auto y = show::delta(735.f, 400.f, fraction);

			x_axis.new_figure(point_2d{ 180.f, 850.f });
			x_axis.line(point_2d{ 1710.f, 850.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			sample_line.new_figure(point_2d{ 650.f, 735.f });
			sample_line.line(point_2d{ x, y });
		}
		else
		{
			x_axis.new_figure(point_2d{ 180.f, 850.f });
			x_axis.line(point_2d{ 1710.f, 850.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			sample_line.new_figure(point_2d{ 650.f, 735.f });
			sample_line.line(point_2d{ 1500.f, 400.f });
		}
		uos.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, sample_line, std::nullopt, stroke_props{ 4 });
	}

	bool line_segment::exit()
	{
		return true;
	}

	line_segment s042{ show::presentation::SLIDE_042 };
}
