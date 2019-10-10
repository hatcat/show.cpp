#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class polar : public show::slide
	{
	public:
		polar(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		brush			m_image_brush;
		render_props	m_rp;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	polar::polar(show::presentation p)
		: show::slide(p)
		, m_image_brush{ rgba_color::black }
	{}

	bool polar::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_image_brush = image_surface{ pres::res + "Geometry St Petersburg (14).png",
										std::experimental::io2d::image_file_format::png,
										std::experimental::io2d::format::argb32 };
		auto image_size = display_point{ 960, 540 };
		auto x_scale = 1920.f / image_size.x();
		auto y_scale = 1080.f / image_size.y();
		m_rp.surface_matrix(matrix_2d::create_scale(point_2d{ x_scale, y_scale }));
		return true;
	}

	void polar::render(unmanaged_output_surface& ds)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		ds.paint(m_image_brush, std::nullopt, m_rp);

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

			auto x_delta = 765.f * fraction;
			x_axis.new_figure(point_2d{ 945.f - x_delta, 600.f });
			x_axis.line(point_2d{ 945.f + x_delta, 600.f });

			auto y_delta = 370.f * fraction;
			y_axis.new_figure(point_2d{ 930.f, 600.f - y_delta });
			y_axis.line(point_2d{ 930.f, 600.f + y_delta });
		}
		else if (time_in_slide <= 2000)
		{
			x_axis.new_figure(point_2d{ 180.f, 600.f });
			x_axis.line(point_2d{ 1710.f, 600.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			auto fraction = (time_in_slide - 1000.f) / 1000.f;
			auto sample_line_x_displacement = (1035.f - 665.f) * fraction;
			auto sample_line_y_displacement = (230.f - 970.f) * fraction;
			sample_line.new_figure(point_2d{ 665.f, 970.f });
			sample_line.line(point_2d{ 665.f + sample_line_x_displacement, 970.f + sample_line_y_displacement });
		}
		else
		{
			x_axis.new_figure(point_2d{ 180.f, 600.f });
			x_axis.line(point_2d{ 1710.f, 600.f });

			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });

			sample_line.new_figure(point_2d{ 665.f, 970.f });
			sample_line.line(point_2d{ 1035.f, 230.f });
		}
		ds.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		ds.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
		ds.stroke(line_brush, sample_line, std::nullopt, stroke_props{ 4 });
	}

	bool polar::exit()
	{
		return true;
	}
}

polar s014{ show::presentation::SLIDE_014 };
