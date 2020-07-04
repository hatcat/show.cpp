#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class translation : public show::slide
	{
	public:
		translation(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	translation::translation(show::presentation p)
		: show::slide(p)
	{}

	bool translation::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry (30).png");
		return true;
	}

	void translation::render(unmanaged_output_surface& uos)
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
			auto delta = show::delta(0.f, 300.f, fraction);

			triangle.new_figure(point_2d{ 970.f + delta, 775.f - delta });
			triangle.line(point_2d{ 1070.f + delta, 600.f - delta });
			triangle.line(point_2d{ 1170.f + delta, 775.f - delta });
			triangle.close_figure();
		}
		else
		{
			triangle.new_figure(point_2d{ 1270.f, 475.f });
			triangle.line(point_2d{ 1370.f, 300.f });
			triangle.line(point_2d{ 1470.f, 475.f });
			triangle.close_figure();
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, triangle, std::nullopt, stroke_props{ 4 });
	}

	bool translation::exit()
	{
		return true;
	}

	translation s030{ show::presentation::SLIDE_030 };
}
