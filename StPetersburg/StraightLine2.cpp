#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class straight_line2 : public show::slide
	{
	public:
		straight_line2(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		brush			m_image_brush;
		render_props	m_rp;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	straight_line2::straight_line2(show::presentation p)
		: show::slide(p)
		, m_image_brush{ rgba_color::black }
	{}

	bool straight_line2::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_image_brush = image_surface{ pres::res + "Geometry St Petersburg (15).png",
										std::experimental::io2d::image_file_format::png,
										std::experimental::io2d::format::argb32 };
		auto image_size = display_point{ 960, 540 };
		auto x_scale = 1920.f / image_size.x();
		auto y_scale = 1080.f / image_size.y();
		m_rp.surface_matrix(matrix_2d::create_scale(point_2d{ x_scale, y_scale }));
		return true;
	}

	void straight_line2::render(unmanaged_output_surface& ds)
	{
		ds.paint(m_image_brush, std::nullopt, m_rp);

		auto x_axis = path_builder{};
		x_axis.clear();

		auto y_axis = path_builder{};
		y_axis.clear();

		auto sample_line = path_builder{};
		sample_line.clear();

		auto axis_brush = brush{ rgba_color::cornflower_blue };
		auto line_brush = brush{ rgba_color::green };

		x_axis.new_figure(point_2d{ 180.f, 600.f });
		x_axis.line(point_2d{ 1710.f, 600.f });

		y_axis.new_figure(point_2d{ 930.f, 230.f });
		y_axis.line(point_2d{ 930.f, 970.f });

		sample_line.new_figure(point_2d{ 665.f, 970.f });
		sample_line.line(point_2d{ 1035.f, 230.f });

		ds.stroke(axis_brush, x_axis, std::nullopt, stroke_props{ 4 });
		ds.stroke(axis_brush, y_axis, std::nullopt, stroke_props{ 4 });
		ds.stroke(line_brush, sample_line, std::nullopt, stroke_props{ 4 });
	}

	bool straight_line2::exit()
	{
		return true;
	}
}

straight_line2 s015{ show::presentation::SLIDE_015 };
