#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class number_line : public show::slide
	{
	public:
		number_line(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	number_line::number_line(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (11).png")
	{}

	bool number_line::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void number_line::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto path = path_builder{};
		path.clear();
		if (time_in_slide <= 1500)
		{
			auto displacement = show::delta(0.f, 765.f, time_in_slide / 1500.f);
			path.new_figure(point_2d{ 945.f - displacement, 600.f });
			path.line(point_2d{ 945.f + displacement, 600.f });
		}
		else
		{
			path.new_figure(point_2d{ 180.f, 600.f });
			path.line(point_2d{ 1710.f, 600.f });
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
	}

	bool number_line::exit()
	{
		return true;
	}

	number_line s011{ show::presentation::SLIDE_011 };
}
