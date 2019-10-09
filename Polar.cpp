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
		m_image_brush = image_surface{ "C:\\Users\\elgui\\source\\P0267_Master\\StPetersburg\\Slides\\Geometry St Petersburg (13).png",
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

		auto path = path_builder{};
		path.clear();
		path.new_figure(point_2d{ 960.f, 600.f });
		if (time_in_slide <= 1000)
		{
			path.line(point_2d{ 960.f + ((1310.f - 960.f) * time_in_slide / 1000), 600.f });
		}
		else if (time_in_slide <= 3000)
		{
			path.line(point_2d{ 1310.f, 600.f });
			path.arc(point_2d{ 350.f, 350.f }, two_pi<float> * (time_in_slide - 1000) / 2000, two_pi<float>);
		}
		else
		{
			path.line(point_2d{ 1310.f, 600.f });
			path.arc(point_2d{ 350.f, 350.f }, two_pi<float>, two_pi<float>);
		}
		ds.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
	}

	bool polar::exit()
	{
		return true;
	}
}

polar s013{ show::presentation::SLIDE_013 };
