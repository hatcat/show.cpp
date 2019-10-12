#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class ellipse : public show::slide
	{
	public:
		ellipse(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	ellipse::ellipse(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (76).png")
	{}

	bool ellipse::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void ellipse::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto line_brush = brush{ rgba_color::green };

		if (time_in_slide <= 1500)
		{
			auto fraction = (time_in_slide) / 1500.f;
			auto theta = show::delta(0.f, two_pi<float> / 8.f, fraction);

			auto c1_a = path_builder{};
			c1_a.new_figure({ 1460.f, 580.f });
			c1_a.arc(point_2d{ 500.f, 300.f }, theta, 0.f);

			auto c1_b = path_builder{};
			c1_b.new_figure({ 1460.f, 580.f });
			c1_b.arc(point_2d{ 500.f, 300.f }, -theta, 0.f);

			auto c2_a = path_builder{};
			c2_a.new_figure({ 960.f, 280.f });
			c2_a.arc(point_2d{ 500.f, 300.f }, theta, pi<float> / 2.f);

			auto c2_b = path_builder{};
			c2_b.new_figure({ 960.f, 280.f });
			c2_b.arc(point_2d{ 500.f, 300.f }, -theta, pi<float> / 2.f);

			auto c3_a = path_builder{};
			c3_a.new_figure({ 460.f, 580.f });
			c3_a.arc(point_2d{ 500.f, 300.f }, theta, pi<float>);

			auto c3_b = path_builder{};
			c3_b.new_figure({ 460.f, 580.f });
			c3_b.arc(point_2d{ 500.f, 300.f }, -theta, pi<float>);

			auto c4_a = path_builder{};
			c4_a.new_figure({ 960.f, 880.f });
			c4_a.arc(point_2d{ 500.f, 300.f }, theta, 3.f * pi<float> / 2.f);

			auto c4_b = path_builder{};
			c4_b.new_figure({ 960.f, 880.f });
			c4_b.arc(point_2d{ 500.f, 300.f }, -theta, 3.f * pi<float> / 2.f);

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
			circle_path.new_figure(point_2d{ 1460.f, 580.f });

			circle_path.arc(point_2d{ 500.f, 300.f }, two_pi<float>, 0.f);
			uos.stroke(line_brush, circle_path, std::nullopt, stroke_props{ 4 });
		}
	}

	bool ellipse::exit()
	{
		return true;
	}

	ellipse s076{ show::presentation::SLIDE_076 };
}

