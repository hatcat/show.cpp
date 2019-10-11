#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class circle : public show::slide
	{
	public:
		circle(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	circle::circle(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (17).png")
	{}

	bool circle::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void circle::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto path = path_builder{};
		path.clear();
		path.new_figure(point_2d{ 960.f, 600.f });
		if (time_in_slide <= 1000)
		{
			path.line(point_2d{ 960.f + ((1310.f - 960.f) * time_in_slide / 1000), 600.f });
		}
		else if (time_in_slide <= 3000)
		{
			path.line(point_2d{ 1310.f, 600.f });
			path.arc(point_2d{ 350.f, 350.f }, two_pi<float> * (time_in_slide - 1000) / 2000, two_pi<float>);
		}
		else
		{
			path.line(point_2d{ 1310.f, 600.f });
			path.arc(point_2d{ 350.f, 350.f }, two_pi<float>, two_pi<float>);
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
	}

	bool circle::exit()
	{
		return true;
	}

	circle s017{ show::presentation::SLIDE_017 };
}

