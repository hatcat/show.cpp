#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class circle3 : public show::slide
	{
	public:
		circle3(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	circle3::circle3(show::presentation p)
		: show::slide(p)
	{}

	bool circle3::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry St Petersburg (74).png");
		return true;
	}

	void circle3::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto line_brush = brush{ rgba_color::green };

		if (time_in_slide <= 1500)
		{
			auto fraction = (time_in_slide) / 1500.f;
			auto theta = show::delta(0.f, two_pi<float> / 8.f, fraction);

			auto c1_a = path_builder{};
			c1_a.new_figure({ 1260.f, 580.f });
			c1_a.arc(point_2d{ 300.f, 300.f }, theta, 0.f);

			auto c1_b = path_builder{};
			c1_b.new_figure({ 1260.f, 580.f });
			c1_b.arc(point_2d{ 300.f, 300.f }, -theta, 0.f);

			auto c2_a = path_builder{};
			c2_a.new_figure({ 960.f, 280.f });
			c2_a.arc(point_2d{ 300.f, 300.f }, theta, pi<float> / 2.f);

			auto c2_b = path_builder{};
			c2_b.new_figure({ 960.f, 280.f });
			c2_b.arc(point_2d{ 300.f, 300.f }, -theta, pi<float> / 2.f);

			auto c3_a = path_builder{};
			c3_a.new_figure({ 660.f, 580.f });
			c3_a.arc(point_2d{ 300.f, 300.f }, theta, pi<float>);

			auto c3_b = path_builder{};
			c3_b.new_figure({ 660.f, 580.f });
			c3_b.arc(point_2d{ 300.f, 300.f }, -theta, pi<float>);

			auto c4_a = path_builder{};
			c4_a.new_figure({ 960.f, 880.f });
			c4_a.arc(point_2d{ 300.f, 300.f }, theta, 3.f * pi<float> / 2.f);

			auto c4_b = path_builder{};
			c4_b.new_figure({ 960.f, 880.f });
			c4_b.arc(point_2d{ 300.f, 300.f }, -theta, 3.f * pi<float> / 2.f);

			uos.stroke(line_brush, c1_a, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c1_b, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c2_a, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c2_b, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c3_a, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c3_b, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c4_a, std::nullopt, stroke_props{ 4 });
			uos.stroke(line_brush, c4_b, std::nullopt, stroke_props{ 4 });
		}
		else
		{
			auto circle_path = path_builder{};
			circle_path.clear();
			circle_path.new_figure(point_2d{ 1260.f, 580.f });

			circle_path.arc(point_2d{ 300.f, 300.f }, two_pi<float>, 0.f);
			uos.stroke(line_brush, circle_path, std::nullopt, stroke_props{ 4 });
		}
	}

	bool circle3::exit()
	{
		return true;
	}

	circle3 s074{ show::presentation::SLIDE_074 };
}

