#include "Geometry.h"

#include "Slide.h"

namespace
{
	using namespace std::experimental::io2d;

	class credits : public show::slide
	{
	public:
		credits(show::presentation);
		bool enter(show::show const&) override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
		std::chrono::time_point<std::chrono::steady_clock> m_entry_point;
	};

	credits::credits(show::presentation p)
		: show::slide(p)
	{}

	bool credits::enter(show::show const& s)
	{
		m_entry_point = std::chrono::steady_clock::now();
		m_bg.prepare(s.res_root() + "Geometry (123).png");
		return true;
	}

	void credits::render(unmanaged_output_surface& uos)
	{
		auto time_in_slide = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_entry_point).count();

		m_bg.render(uos);

		auto r = std::uint8_t( (time_in_slide / 16) % 256 );
		auto g = std::uint8_t( ((time_in_slide + 1280) / 16) % 256 );
		auto b = std::uint8_t( ((time_in_slide + 2560) / 16) % 256 );
		auto colour = rgba_color(r, g, b);

		auto tri = path_builder{};
		tri.new_figure({ 1820.f, 0.f });
		tri.line({ 1920.f, 0.f });
		tri.line({ 1920.f, 100.f });
		tri.close_figure();

		uos.stroke(brush{ colour }, tri); // Need to fill this...
	}

	bool credits::exit()
	{
		return true;
	}

	credits s123{ show::presentation::SLIDE_123 };
}
