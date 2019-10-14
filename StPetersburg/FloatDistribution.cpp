#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class float_distribution : public show::slide
	{
	public:
		float_distribution(show::presentation name);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image								m_bg;
		std::chrono::time_point<std::chrono::steady_clock>	m_entry_point;

	};

	float_distribution::float_distribution(show::presentation p)
		: show::slide(p)
	{}

	bool float_distribution::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry St Petersburg (93).png");
		return true;
	}

	void float_distribution::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		path_builder major_grid[18];
		float major_fractions[9] = { std::min(time_in_slide / 1000.f, 1.f),
									std::min((time_in_slide - 100) / 1000.f, 1.f),
									std::min((time_in_slide - 200) / 1000.f, 1.f),
									std::min((time_in_slide - 300) / 1000.f, 1.f),
									std::min((time_in_slide - 400) / 1000.f, 1.f),
									std::min((time_in_slide - 500) / 1000.f, 1.f),
									std::min((time_in_slide - 600) / 1000.f, 1.f),
									std::min((time_in_slide - 700) / 1000.f, 1.f),
									std::min((time_in_slide - 800) / 1000.f, 1.f) };
		for (auto i = 0; i < 9; ++i)
		{
			if (major_fractions[i] > 0.f)
			{
				major_grid[2 * i].new_figure({ 600.f, i * 90.f + 250.f });
				major_grid[2 * i].line({ 600.f + major_fractions[i] * 720.f, i * 90.f + 250.f });
				major_grid[1 + 2 * i].new_figure({ i * 90.f + 600.f, 250.f });
				major_grid[1 + 2 * i].line({ i * 90.f + 600.f, 250.f + major_fractions[i] * 720.f });
			}
		}
		for (const auto& g1 : major_grid) uos.stroke(brush{ rgba_color::black }, g1, std::nullopt, stroke_props{ 1 });

		path_builder minor_grid[18];
		float minor_fractions[9] = { std::min((time_in_slide - 200) / 1000.f, 1.f),
									std::min((time_in_slide - 250) / 1000.f, 1.f),
									std::min((time_in_slide - 300) / 1000.f, 1.f),
									std::min((time_in_slide - 350) / 1000.f, 1.f),
									std::min((time_in_slide - 400) / 1000.f, 1.f),
									std::min((time_in_slide - 450) / 1000.f, 1.f),
									std::min((time_in_slide - 500) / 1000.f, 1.f),
									std::min((time_in_slide - 550) / 1000.f, 1.f),
									std::min((time_in_slide - 600) / 1000.f, 1.f) };
		for (auto i = 0; i < 9; ++i)
		{
			if (minor_fractions[i] > 0.f)
			{
				minor_grid[2 * i].new_figure({ 780.f, i * 45.f + 430.f });
				minor_grid[2 * i].line({ 780.f + minor_fractions[i] * 360.f, i * 45.f + 430.f });
				minor_grid[1 + 2 * i].new_figure({ i * 45.f + 780.f, 430.f });
				minor_grid[1 + 2 * i].line({ i * 45.f + 780.f, 430.f + minor_fractions[i] * 360.f });
			}
		}
		for (const auto& g1 : minor_grid) uos.stroke(brush{ rgba_color::dark_grey }, g1, std::nullopt, stroke_props{ 1 });

		path_builder minimus_grid[18];
		float minimus_fractions[9] = { std::min((time_in_slide - 300) / 1000.f, 1.f),
										std::min((time_in_slide - 325) / 1000.f, 1.f),
										std::min((time_in_slide - 350) / 1000.f, 1.f),
										std::min((time_in_slide - 375) / 1000.f, 1.f),
										std::min((time_in_slide - 400) / 1000.f, 1.f),
										std::min((time_in_slide - 425) / 1000.f, 1.f),
										std::min((time_in_slide - 450) / 1000.f, 1.f),
										std::min((time_in_slide - 475) / 1000.f, 1.f),
										std::min((time_in_slide - 500) / 1000.f, 1.f) };
		for (auto i = 0; i < 9; ++i)
		{
			if (minimus_fractions[i] > 0.f)
			{
				minimus_grid[2 * i].new_figure({ 870.f, i * 22.5f + 520.f });
				minimus_grid[2 * i].line({ 870.f + minimus_fractions[i] * 180.f, i * 22.5f + 520.f });
				minimus_grid[1 + 2 * i].new_figure({ i * 22.5f + 870.f, 520.f });
				minimus_grid[1 + 2 * i].line({ i * 22.5f + 870.f, 520.f + minimus_fractions[i] * 180.f });
			}
		}
		for (const auto& g1 : minimus_grid) uos.stroke(brush{ rgba_color::grey }, g1, std::nullopt, stroke_props{ 1 });
	}

	bool float_distribution::exit()
	{
		return true;
	}

	float_distribution s093{ show::presentation::SLIDE_093 };
}
