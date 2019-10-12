#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class parabola : public show::slide
	{
	public:
		parabola(show::presentation);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	parabola::parabola(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (16).png")
	{}

	bool parabola::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void parabola::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto x_axis = path_builder{};
		x_axis.clear();

		auto y_axis = path_builder{};
		y_axis.clear();

		auto parabola_line = path_builder{};
		parabola_line.clear();

		auto axis_brush = brush{ rgba_color::cornflower_blue };
		auto line_brush = brush{ rgba_color::green };

		if (time_in_slide <= 1000)
		{
			auto fraction = (time_in_slide) / 1000.f;

			auto x_delta = 750.f * fraction;
			x_axis.new_figure(point_2d{ 930.f - x_delta, 330.f });
			x_axis.line(point_2d{ 930.f + x_delta, 330.f });

			auto y_delta = (970.f - 230.f) * fraction;
			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 230.f + y_delta });

			auto p_delta = 330.f * fraction;
			auto p_y_delta = (970.f - 330.f) * fraction;
			auto p_control_delta = 640.f - (640.f * fraction);
			parabola_line.new_figure(point_2d{ 930.f - p_delta, 330.f + p_y_delta });
			parabola_line.quadratic_curve(point_2d{ 930.f, -310.f + p_control_delta }, point_2d{ 930.f + p_delta, 330.f + p_y_delta });
		}
		else
		{
			x_axis.new_figure(point_2d{ 180.f, 330.f });
			x_axis.line(point_2d{ 1680.f, 330.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			parabola_line.new_figure(point_2d{ 600.f, 970.f });
			parabola_line.quadratic_curve(point_2d{ 930.f, -310.f }, point_2d{ 1260.f, 970.f });
		}

		uos.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, parabola_line, std::nullopt, stroke_props{ 4 });
	}

	bool parabola::exit()
	{
		return true;
	}

	parabola s016{ show::presentation::SLIDE_016 };
}
