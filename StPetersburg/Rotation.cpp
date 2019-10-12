#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class rotation : public show::slide
	{
	public:
		rotation(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	rotation::rotation(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (34).png")
	{}

	bool rotation::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	point_2d rotate(point_2d p, float theta)
	{
		return { p.x() * cosf(theta) + p.y() * sinf(theta), p.y() * cosf(theta) - p.x() * sinf(theta) };
	}

	void rotation::render(unmanaged_output_surface& uos)
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

			auto theta = show::delta(0.f, -two_pi<float> / 4.f, fraction);
			auto origin = point_2d{ 945.f, 800.f };

			triangle.new_figure(origin + rotate(point_2d{ 970.f, 775.f } - origin, theta));
			triangle.line(origin + rotate(point_2d{ 1070.f, 600.f } - origin, theta));
			triangle.line(origin + rotate(point_2d{ 1170.f, 775.f } - origin, theta));
			triangle.close_figure();
		}
		else
		{
			triangle.new_figure(point_2d{ 970.f, 825.f });
			triangle.line(point_2d{ 1145.f, 925.f });
			triangle.line(point_2d{ 970.f, 1025.f });
			triangle.close_figure();
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, triangle, std::nullopt, stroke_props{ 4 });
	}

	bool rotation::exit()
	{
		return true;
	}

	rotation s034{ show::presentation::SLIDE_034 };
}
