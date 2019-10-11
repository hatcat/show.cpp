#include "Slide.h"

show::slide::slide(presentation id)
{
	slide_show()[id] = this;
}

show::slide::~slide() {}

std::vector<show::slide*>& show::slide_show()
{
	static std::vector<slide*> slides{ SLIDE_LAST };
	return slides;
}

show::background_image::background_image(std::string const& filename)
	: m_image_brush{ rgba_color::black }
	, m_filename(filename)
{}

show::background_image::background_image(std::string&& filename)
	: m_image_brush{ rgba_color::black }
	, m_filename(std::move(filename))
{}

void show::background_image::prepare()
{
	m_image_brush = image_surface{ m_filename,
									std::experimental::io2d::image_file_format::png,
									std::experimental::io2d::format::argb32 };
	auto image_size = display_point{ 960, 540 };
	auto x_scale = 1920.f / image_size.x();
	auto y_scale = 1080.f / image_size.y();
	m_rp.surface_matrix(matrix_2d::create_scale(point_2d{ x_scale, y_scale }));
}

void show::background_image::render(unmanaged_output_surface& uos)
{
	uos.paint(m_image_brush, std::nullopt, m_rp);
}