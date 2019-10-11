#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class axes : public show::slide
	{
	public:
		axes(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		brush			m_image_brush;
		render_props	m_rp;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	axes::axes(show::presentation p)
		: show::slide(p)
		, m_image_brush{ rgba_color::black }
	{}

	bool axes::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_image_brush = image_surface{ pres::res + "Geometry St Petersburg (12).png",
										std::experimental::io2d::image_file_format::png,
										std::experimental::io2d::format::argb32 };
		auto image_size = display_point{ 960, 540 };
		auto x_scale = 1920.f / image_size.x();
		auto y_scale = 1080.f / image_size.y();
		m_rp.surface_matrix(matrix_2d::create_scale(point_2d{ x_scale, y_scale }));
		return true;
	}

	void axes::render(unmanaged_output_surface& ds)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		ds.paint(m_image_brush, std::nullopt, m_rp);

		auto x_axis = path_builder{};
		x_axis.clear();
		x_axis.new_figure(point_2d{ 180.f, 600.f });
		x_axis.line(point_2d{ 1710.f, 600.f });
		auto line_brush = brush{ rgba_color::cornflower_blue };
		ds.stroke(line_brush, x_axis, std::nullopt, stroke_props{ 4 });

		auto y_axis = path_builder{};
		y_axis.clear();
		if (time_in_slide <= 1500)
		{
			auto x1 = 180.f + ((930.f - 180.f) * time_in_slide / 1500);
			auto y1 = 600.f + ((970.f - 600.f) * time_in_slide / 1500);
			auto x2 = 1710.f - ((1710.f - 930.f) * time_in_slide / 1500);
			auto y2 = 600.f - ((600.f - 230.f) * time_in_slide / 1500);
			y_axis.new_figure(point_2d{ x1, y1 });
			y_axis.line(point_2d{ x2, y2 });
			line_brush = brush{ rgba_color::red };
		}
		else
		{
			y_axis.new_figure(point_2d{ 930.f, 230.f });
			y_axis.line(point_2d{ 930.f, 970.f });
		}

		ds.stroke(line_brush, y_axis, std::nullopt, stroke_props{ 4 });
	}

	bool axes::exit()
	{
		return true;
	}
}

axes s012{ show::presentation::SLIDE_012 };
