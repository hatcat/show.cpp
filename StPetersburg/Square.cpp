#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class square : public show::slide
	{
	public:
		square(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	square::square(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (69).png")
	{}

	bool square::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void square::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto p1a = path_builder{};
		p1a.new_figure({ 660.f, 240.f });
		auto p1b = path_builder{};
		p1b.new_figure({ 660.f, 240.f });

		auto p2a = path_builder{};
		p2a.new_figure({ 660.f, 840.f });
		auto p2b = path_builder{};
		p2b.new_figure({ 660.f, 840.f });

		auto p3a = path_builder{};
		p3a.new_figure({ 1260.f, 840.f });
		auto p3b = path_builder{};
		p3b.new_figure({ 1260.f, 840.f });

		auto p4a = path_builder{};
		p4a.new_figure({ 1260.f, 240.f });
		auto p4b = path_builder{};
		p4b.new_figure({ 1260.f, 240.f });

		if (time_in_slide <= 1500)
		{
			auto fraction = (time_in_slide) / 1500.f;

			auto p1a_x = show::delta(660.f, 960.f, fraction);
			auto p1a_y = 240.f;
			auto p1b_x = 660.f;
			auto p1b_y = show::delta(240.f, 540.f, fraction);

			auto p2a_x = show::delta(660.f, 960.f, fraction);
			auto p2a_y = 840.f;
			auto p2b_x = 660.f;
			auto p2b_y = show::delta(840.f, 540.f, fraction);

			auto p3a_x = show::delta(1260.f, 960.f, fraction);
			auto p3a_y = 840.f;
			auto p3b_x = 1260.f;
			auto p3b_y = show::delta(840.f, 540.f, fraction);

			auto p4a_x = show::delta(1260.f, 960.f, fraction);
			auto p4a_y = 240.f;
			auto p4b_x = 1260.f;
			auto p4b_y = show::delta(240.f, 540.f, fraction);

			p1a.line({ p1a_x, p1a_y });
			p1b.line({ p1b_x, p1b_y });
			p2a.line({ p2a_x, p2a_y });
			p2b.line({ p2b_x, p2b_y });
			p3a.line({ p3a_x, p3a_y });
			p3b.line({ p3b_x, p3b_y });
			p4a.line({ p4a_x, p4a_y });
			p4b.line({ p4b_x, p4b_y });
		}
		else
		{
			p1a.line({ 960.f, 240.f });
			p1b.line({ 660.f, 540.f });
			p2a.line({ 960.f, 840.f });
			p2b.line({ 660.f, 540.f });
			p3a.line({ 960.f, 840.f });
			p3b.line({ 1260.f, 540.f });
			p4a.line({ 960.f, 240.f });
			p4b.line({ 1260.f, 540.f });
		}

		uos.stroke(brush{ rgba_color::green }, p1a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p1b, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p2a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p2b, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p3a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p3b, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p4a, std::nullopt, stroke_props{ 4 });
		uos.stroke(brush{ rgba_color::green }, p4b, std::nullopt, stroke_props{ 4 });
	}

	bool square::exit()
	{
		return true;
	}

	square s069{ show::presentation::SLIDE_069 };
}
