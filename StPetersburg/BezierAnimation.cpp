#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class bezier_animation : public show::slide
	{
	public:
		bezier_animation(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	bezier_animation::bezier_animation(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (64).png")
	{}

	bool bezier_animation::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void bezier_animation::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto left = path_builder{};
		left.clear();
		left.new_figure(point_2d{ 350.f, 825.f });
		left.line(point_2d{ 350.f, 255.f });
		left.line(point_2d{ 925.f, 825.f });

		auto right = path_builder{};
		right.clear();
		right.new_figure(point_2d{ 995.f, 825.f });
		right.line(point_2d{ 995.f, 255.f });
		right.line(point_2d{ 1565.f, 255.f });

		auto left_curve = path_builder{};
		left_curve.clear();
		left_curve.new_figure(point_2d{ 350.f, 825.f });
		left_curve.quadratic_curve(point_2d{ 350.f, 255.f }, point_2d{ 925.f, 825.f });

		auto right_curve = path_builder{};
		right_curve.clear();
		right_curve.new_figure(point_2d{ 995.f, 825.f });
		right_curve.quadratic_curve(point_2d{ 995.f, 255.f }, point_2d{ 1565.f, 255.f });

		if (time_in_slide <= 5000)
		{
			auto fraction = (time_in_slide) / 5000.f;
			auto l_x1 = 350.f;
			auto l_y1 = show::delta(825.f, 255.f, fraction);
			auto l_x2 = show::delta(350.f, 925.f, fraction);
			auto l_y2 = show::delta(255.f, 825.f, fraction);

			auto r_x1 = 995.f;
			auto r_y1 = show::delta(825.f, 255.f, fraction);
			auto r_x2 = show::delta(995.f, 1565.f, fraction);
			auto r_y2 = 255.f;

			auto left_normal = path_builder{};
			left_normal.clear();
			left_normal.new_figure({ l_x1, l_y1 });
			left_normal.line({ l_x2, l_y2 });

			auto right_normal = path_builder{};
			right_normal.clear();
			right_normal.new_figure({ r_x1, r_y1 });
			right_normal.line({ r_x2, r_y2 });

			uos.stroke(brush{ rgba_color::black }, left_normal, std::nullopt, stroke_props{ 4 });
			uos.stroke(brush{ rgba_color::black }, right_normal, std::nullopt, stroke_props{ 4 });
		}

		uos.stroke(brush{ rgba_color::red }, left, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::red }, right, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, left_curve, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, right_curve, std::nullopt, stroke_props{ 4 });
	}

	bool bezier_animation::exit()
	{
		return true;
	}

	bezier_animation s064{ show::presentation::SLIDE_064 };
}
