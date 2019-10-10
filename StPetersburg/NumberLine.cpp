#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class number_line : public show::slide
	{
	public:
		number_line(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		brush			m_image_brush;
		render_props	m_rp;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	number_line::number_line(show::presentation p)
		: show::slide(p)
		, m_image_brush{ rgba_color::black }
	{}

	bool number_line::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_image_brush = image_surface{ pres::res + "Geometry St Petersburg (11).png",
										std::experimental::io2d::image_file_format::png,
										std::experimental::io2d::format::argb32 };
		auto image_size = display_point{ 960, 540 };
		auto x_scale = 1920.f / image_size.x();
		auto y_scale = 1080.f / image_size.y();
		m_rp.surface_matrix(matrix_2d::create_scale(point_2d{ x_scale, y_scale }));
		return true;
	}

	void number_line::render(unmanaged_output_surface& ds)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		ds.paint(m_image_brush, std::nullopt, m_rp);

		auto path = path_builder{};
		path.clear();
		if (time_in_slide <= 1530)
		{
			auto displacement = 765.f - (time_in_slide / 2);
			path.new_figure(point_2d{ 180.f + displacement, 600.f });
			path.line(point_2d{ 1710.f - displacement, 600.f });
		}
		else
		{
			path.new_figure(point_2d{ 180.f, 600.f });
			path.line(point_2d{ 1710.f, 600.f });
		}
		ds.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
	}

	bool number_line::exit()
	{
		return true;
	}
}

number_line s011{ show::presentation::SLIDE_011 };
