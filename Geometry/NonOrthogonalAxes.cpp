#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class straight_line : public show::slide
	{
	public:
		straight_line(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	straight_line::straight_line(show::presentation p)
		: show::slide(p)
	{}

	bool straight_line::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(pres::root(s) + "Geometry (73).png");
		return true;
	}

	void straight_line::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		// Draw the axes, then draw the line. x = 180 -> 1710, y = 970 -> 230, line = 665,970 -> 1035, 230
		auto x_axis = path_builder{};
		x_axis.clear();

		auto y_axis = path_builder{};
		y_axis.clear();

		auto axis_brush = brush{ rgba_color::cornflower_blue };

		if (time_in_slide <= 1500)
		{
			auto fraction = (time_in_slide) / 1500.f;
			auto x1 = show::delta(945.f, 180.f, fraction);
			auto x2 = show::delta(945.f, 1710.f, fraction);
			auto y1 = show::delta(600.f, 970.f, fraction);
			auto y2 = show::delta(600.f, 230.f, fraction);
			auto ya = show::delta(945.f, 545.f, fraction);
			auto yb = show::delta(945.f, 1345.f, fraction);

			x_axis.new_figure(point_2d{ x1, 600.f });
			x_axis.line(point_2d{ x2, 600.f });

			y_axis.new_figure(point_2d{ ya, y1 });
			y_axis.line(point_2d{ yb, y2 });
		}
		else
		{
			x_axis.new_figure(point_2d{ 180.f, 600.f });
			x_axis.line(point_2d{ 1710.f, 600.f });

			y_axis.new_figure(point_2d{ 545.f, 970.f });
			y_axis.line(point_2d{ 1345.f, 230.f });
		}
		uos.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
	}

	bool straight_line::exit()
	{
		return true;
	}

	straight_line s073{ show::presentation::SLIDE_073 };
}
