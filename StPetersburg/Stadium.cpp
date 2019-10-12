#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class stadium : public show::slide
	{
	public:
		stadium(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	stadium::stadium(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (78).png")
	{}

	bool stadium::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void stadium::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto line_brush = brush{ rgba_color::green };

		auto s1_a = path_builder{};
		s1_a.new_figure({ 1460.f, 280.f });

		auto s1_b = path_builder{};
		s1_b.new_figure({ 1460.f, 280.f });

		auto s2_a = path_builder{};
		s2_a.new_figure({ 460.f, 280.f });

		auto s2_b = path_builder{};
		s2_b.new_figure({ 460.f, 280.f });

		auto s3_a = path_builder{};
		s3_a.new_figure({ 460.f, 880.f });

		auto s3_b = path_builder{};
		s3_b.new_figure({ 460.f, 880.f });

		auto s4_a = path_builder{};
		s4_a.new_figure({ 1460.f, 880.f });

		auto s4_b = path_builder{};
		s4_b.new_figure({ 1460.f, 880.f });

		if (time_in_slide <= 1500)
		{
			auto fraction = (time_in_slide) / 1500.f;
			auto theta = show::delta(0.f, pi<float> / 2.f, fraction);
			auto delta = show::delta(0.f, 500.f, fraction);

			s1_a.arc(point_2d{ 300.f, 300.f }, -theta, pi<float> / 2.f);
			s1_b.line(point_2d{ 1460.f - delta, 280.f });
			s2_a.arc(point_2d{ 300.f, 300.f }, theta, pi<float> / 2.f);
			s2_b.line(point_2d{ 460.f + delta, 280.f });
			s3_a.arc(point_2d{ 300.f, 300.f }, -theta, 3.f * pi<float> / 2.f);
			s3_b.line(point_2d{ 460.f + delta, 880.f });
			s4_a.arc(point_2d{ 300.f, 300.f }, theta, 3.f * pi<float> / 2.f);
			s4_b.line(point_2d{ 1460.f - delta, 880.f });
		}
		else
		{
			s1_a.arc(point_2d{ 300.f, 300.f }, -pi<float> / 2.f, pi<float> / 2.f);
			s1_b.line(point_2d{ 960.f, 280.f });
			s2_a.arc(point_2d{ 300.f, 300.f }, pi<float> / 2.f, pi<float> / 2.f);
			s2_b.line(point_2d{ 960.f, 280.f });
			s3_a.arc(point_2d{ 300.f, 300.f }, -pi<float> / 2.f, 3.f * pi<float> / 2.f);
			s3_b.line(point_2d{ 960.f, 880.f });
			s4_a.arc(point_2d{ 300.f, 300.f }, pi<float> / 2.f, 3.f * pi<float> / 2.f);
			s4_b.line(point_2d{ 960.f, 880.f });
		}
		uos.stroke(line_brush, s1_a, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s1_b, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s2_a, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s2_b, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s3_a, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s3_b, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s4_a, std::nullopt, stroke_props{ 4 });
		uos.stroke(line_brush, s4_b, std::nullopt, stroke_props{ 4 });
	}

	bool stadium::exit()
	{
		return true;
	}

	stadium s078{ show::presentation::SLIDE_078 };
}

