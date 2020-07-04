#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class triangle : public show::slide
	{
	public:
		triangle(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	triangle::triangle(show::presentation p)
		: show::slide(p)
	{}

	bool triangle::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry (67).png");
		return true;
	}

	void triangle::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto p1a = path_builder{};
		p1a.new_figure({ 960.f, 280.f });
		auto p1b = path_builder{};
		p1b.new_figure({ 960.f, 280.f });

		auto p2a = path_builder{};
		p2a.new_figure({ 650.f, 800.f });
		auto p2b = path_builder{};
		p2b.new_figure({ 650.f, 800.f });

		auto p3a = path_builder{};
		p3a.new_figure({ 1260.f, 800.f });
		auto p3b = path_builder{};
		p3b.new_figure({ 1260.f, 800.f });

		if (time_in_slide <= 1500)
		{
			auto fraction = (time_in_slide) / 1500.f;

			auto p1a_x = show::delta(960.f, 805.f, fraction);
			auto p1_y = show::delta(280.f, 540.f, fraction);
			auto p1b_x = show::delta(960.f, 1115.f, fraction);

			auto p2a_x = show::delta(650.f, 805.f, fraction);
			auto p2a_y = show::delta(800.f, 540.f, fraction);
			auto p2b_x = show::delta(650.f, 955.f, fraction);
			auto p2b_y = 800.f;

			auto p3a_x = show::delta(1260.f, 1115.f, fraction);
			auto p3a_y = show::delta(800.f, 540.f, fraction);
			auto p3b_x = show::delta(1260.f, 955.f, fraction);
			auto p3b_y = 800.f;

			p1a.line({ p1a_x, p1_y });
			p1b.line({ p1b_x, p1_y });
			p2a.line({ p2a_x, p2a_y });
			p2b.line({ p2b_x, p2b_y });
			p3a.line({ p3a_x, p3a_y });
			p3b.line({ p3b_x, p3b_y });
		}
		else
		{
			p1a.line({ 805.f, 540.f });
			p1b.line({ 1115.f, 540.f });
			p2a.line({ 805.f, 540.f });
			p2b.line({ 955.f, 800.f });
			p3a.line({ 1115.f, 540.f });
			p3b.line({ 955.f, 800.f });
		}

		uos.stroke(brush{ rgba_color::green }, p1a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p1b, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p2a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p2b, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p3a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p3b, std::nullopt, stroke_props{ 4 });
	}

	bool triangle::exit()
	{
		return true;
	}

	triangle s067{ show::presentation::SLIDE_067 };
}
