#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class number_line : public show::slide
	{
	public:
		number_line(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	number_line::number_line(show::presentation p)
		: show::slide(p)
	{}

	bool number_line::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(pres::root(s) + "Geometry (11).png");
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
			auto fraction = time_in_slide / 1500.f;
			auto x1 = show::delta(945.f, 180.f, fraction);
			auto x2 = show::delta(945.f, 1710.f, fraction);
			path.new_figure(point_2d{ x1, 600.f });
			path.line(point_2d{ x2, 600.f });
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
