#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class polar : public show::slide
	{
	public:
		polar(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	polar::polar(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (13).png")
	{}

	bool polar::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void polar::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto path = path_builder{};
		path.clear();
		path.new_figure(point_2d{ 960.f, 600.f });
		if (time_in_slide <= 1000)
		{
			auto fraction = time_in_slide / 1000.f;
			auto x = show::delta(960.f, 1310.f, fraction);
			path.line(point_2d{ x, 600.f });
		}
		else if (time_in_slide <= 3000)
		{
			path.line(point_2d{ 1310.f, 600.f });

			auto fraction = (time_in_slide - 1000.f) / 2000.f;
			auto theta = show::delta(0.f, two_pi<float>, fraction);
			path.arc(point_2d{ 350.f, 350.f }, theta, 0.f);
		}
		else
		{
			path.line(point_2d{ 1310.f, 600.f });
			path.arc(point_2d{ 350.f, 350.f }, two_pi<float>, 0.f);
		}
		uos.stroke(brush{ rgba_color::cornflower_blue }, path, std::nullopt, stroke_props{ 4 });
	}

	bool polar::exit()
	{
		return true;
	}

	polar s013{ show::presentation::SLIDE_013 };
}
