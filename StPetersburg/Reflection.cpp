#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class reflection : public show::slide
	{
	public:
		reflection(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	reflection::reflection(show::presentation p)
		: show::slide(p)
	{}

	bool reflection::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry (33).png");
		return true;
	}

	void reflection::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto path = path_builder{};
		path.clear();
		path.new_figure(point_2d{ 900.f, 800.f });
		path.line(point_2d{ 1710.f, 800.f });
		path.new_figure(point_2d{ 945.f, 300.f });
		path.line(point_2d{ 945.f, 1050.f });

		auto triangle = path_builder{};

		if (time_in_slide <= 1500)
		{
			triangle.new_figure(point_2d{ 970.f, 775.f });
			triangle.line(point_2d{ 1070.f, 600.f });
			triangle.line(point_2d{ 1170.f, 775.f });
			triangle.close_figure();
		}
		else if (time_in_slide <= 3000)
		{
			auto fraction = (time_in_slide - 1500.f) / 1500.f;
			auto y1 = show::delta(775.f, 825.f, fraction);
			auto y2 = show::delta(600.f, 1000.f, fraction);
			auto y3 = show::delta(775.f, 825.f, fraction);

			triangle.new_figure(point_2d{ 970.f, y1 });
			triangle.line(point_2d{ 1070.f, y2 });
			triangle.line(point_2d{ 1170.f, y3 });
			triangle.close_figure();
		}
		else
		{
			triangle.new_figure(point_2d{ 970.f, 825.f });
			triangle.line(point_2d{ 1070.f, 1000.f });
			triangle.line(point_2d{ 1170.f, 825.f });
			triangle.close_figure();
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, triangle, std::nullopt, stroke_props{ 4 });
	}

	bool reflection::exit()
	{
		return true;
	}

	reflection s033{ show::presentation::SLIDE_033 };
}
