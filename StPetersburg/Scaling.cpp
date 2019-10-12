#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class scaling : public show::slide
	{
	public:
		scaling(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	scaling::scaling(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (31).png")
	{}

	bool scaling::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void scaling::render(unmanaged_output_surface& uos)
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
			auto x1 = show::delta(970.f, 970.f + ((970.f - 945.f) * 2), fraction);
			auto y1 = show::delta(775.f, 775.f + ((775.f - 800.f) * 2), fraction);
			auto x2 = show::delta(1070.f, 1070.f + ((1070.f - 945.f) * 2), fraction);
			auto y2 = show::delta(600.f, 600.f + ((600.f - 800.f) * 2), fraction);
			auto x3 = show::delta(1170.f, 1170.f + ((1170.f - 945.f) * 2), fraction);

			triangle.new_figure(point_2d{ x1, y1 });
			triangle.line(point_2d{ x2, y2 });
			triangle.line(point_2d{ x3, y1 });
			triangle.close_figure();
		}
		else
		{
			triangle.new_figure(point_2d{ 970.f + ((970.f - 945.f) * 2), 775.f + ((775.f - 800.f) * 2) });
			triangle.line(point_2d{ 1070.f + ((1070.f - 945.f) * 2), 600.f + ((600.f - 800.f) * 2) });
			triangle.line(point_2d{ 1170.f + ((1170.f - 945.f) * 2), 775.f + ((775.f - 800.f) * 2) });
			triangle.close_figure();
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, triangle, std::nullopt, stroke_props{ 4 });
	}

	bool scaling::exit()
	{
		return true;
	}

	scaling s031{ show::presentation::SLIDE_031 };
}
