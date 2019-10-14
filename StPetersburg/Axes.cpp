#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class axes : public show::slide
	{
	public:
		axes(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	axes::axes(show::presentation p)
		: show::slide(p)
	{}

	bool axes::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry St Petersburg (12).png");
		return true;
	}

	void axes::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto x_axis = path_builder{};
		x_axis.clear();
		x_axis.new_figure(point_2d{ 180.f, 600.f });
		x_axis.line(point_2d{ 1710.f, 600.f });
		auto line_brush = brush{ rgba_color::cornflower_blue };
		uos.stroke(line_brush, x_axis, std::nullopt, stroke_props{ 4 });

		auto y_axis = path_builder{};
		y_axis.clear();
		if (time_in_slide <= 1500)
		{
			auto fraction = time_in_slide / 1500.f;
			auto x1 = show::delta(180.f, 930.f, fraction);
			auto y1 = show::delta(600.f, 970.f, fraction);
			auto x2 = show::delta(1710.f, 930.f, fraction);
			auto y2 = show::delta(600.f, 230.f, fraction);
			y_axis.new_figure(point_2d{ x1, y1 });
			y_axis.line(point_2d{ x2, y2 });
			line_brush = brush{ rgba_color::red };
		}
		else
		{
			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });
		}

		uos.stroke(line_brush, y_axis, std::nullopt, stroke_props{ 4 });
	}

	bool axes::exit()
	{
		return true;
	}

	axes s012{ show::presentation::SLIDE_012 };
}
