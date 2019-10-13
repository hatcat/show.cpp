#include "StPetersburg.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class wild_curve : public show::slide
	{
	public:
		wild_curve(show::presentation name);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image								m_bg;
		std::chrono::time_point<std::chrono::steady_clock>	m_entry_point;

	};

	wild_curve::wild_curve(show::presentation p)
		: show::slide(p)
		, m_bg(pres::res + "Geometry St Petersburg (81).png")
	{}

	bool wild_curve::enter()
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare();
		return true;
	}

	void wild_curve::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto curve = path_builder{};
		curve.new_figure({ 100.f, 300.f });
		curve.cubic_curve({ 200.f, 500.f }, { 300.f, 400.f }, { 350.f, 450.f });
		curve.cubic_curve({ 500.f, 600.f }, { 550.f, 400.f }, { 600.f, 450.f });
		curve.cubic_curve({ 650.f, 500.f }, { 900.f, 800.f }, { 1000.f, 700.f });
		curve.cubic_curve({ 1100.f, 600.f }, { 1300.f, 900.f }, { 1500.f, 800.f });
		curve.cubic_curve({ 1700.f, 700.f }, { 1750.f, 300.f }, { 1820.f, 800.f });

		auto clip = clip_props{ bounding_box{ 0.f, 0.f, 1920.f, 1080.f } };
		if (time_in_slide <= 1500)
		{
			auto fraction = time_in_slide / 1500.f;
			clip = clip_props{ bounding_box{ 0.f, 0.f, 1920.f * fraction, 1080.f } };
		}

		uos.stroke(brush{ rgba_color::green }, curve, std::nullopt, stroke_props{ 4 }, std::nullopt, std::nullopt, clip);
	}

	bool wild_curve::exit()
	{
		return true;
	}

	wild_curve s081{ show::presentation::SLIDE_081 };
}
