#include "Slide.h"

#include <random>
#include <array>
#include <cstddef>

namespace
{
	using namespace std::experimental::io2d;

	point_2d screen_space(point_2d v);

	////////////////////////////////////////////////////////////////////////////////
	// Constants
	////////////////////////////////////////////////////////////////////////////////

	constexpr auto playing_field_width{ 640.0f };
	constexpr auto playing_field_height{ 480.0f };
	constexpr auto max_missiles{ 4 };

	constexpr auto big_asteroid_score{ 10 };
	constexpr auto medium_asteroid_score{ 20 };
	constexpr auto small_asteroid_score{ 40 };

	constexpr auto big_asteroid_size{ 5.0f };
	constexpr auto medium_asteroid_size{ 2.5f };
	constexpr auto small_asteroid_size{ 1.25f };

	constexpr auto initial_speed{ 25.0f };

	constexpr auto spin_left{ std::experimental::io2d::tau<float> };
	constexpr auto spin_right{ -std::experimental::io2d::tau<float> };
	constexpr auto thrust{ 150.0f };
	constexpr auto max_ship_speed{ 300.0f };

	constexpr auto missile_speed{ 250.0f };
	constexpr auto missile_lifespan{ 1.5f };

	constexpr auto asteroid_sweep_lookahead{ 2.0f };

	render_props	scale_to_screen;

	////////////////////////////////////////////////////////////////////////////////
	// Input
	////////////////////////////////////////////////////////////////////////////////

	struct keypress
	{
		bool anti_clockwise_key_press = false;
		bool clockwise_key_press = false;
		bool thrust_key_press = false;
		bool fire_key_press = false;
		bool pause_key_press = false;
	};

	auto anti_clockwise_key = 'Q';
	auto clockwise_key = 'W';
	auto thrust_key = 'U';
	auto fire_key = 'I';
	auto pause_key = 'P';
	auto fire_down = false;
	auto pause_down = false;

	bool bounce_check(const bool& key, bool& down)
	{
		if (key && !down)
		{
			down = true;
			return true;
		}
		down = key;
		return false;
	}

	keypress& press()
	{
		static keypress k;
		return k;
	}

	bool do_anti_clockwise()
	{
		return press().anti_clockwise_key_press;
	}

	bool do_clockwise()
	{
		return press().clockwise_key_press;
	}

	bool do_thrust()
	{
		return press().thrust_key_press;
	}

	bool do_fire()
	{
		return bounce_check(press().fire_key_press, fire_down);
	}

	bool do_pause()
	{
		return bounce_check(press().pause_key_press, pause_down);
	}

	void get_key_states()
	{
		press() = { GetKeyState(anti_clockwise_key) < 0, GetKeyState(clockwise_key) < 0, GetKeyState(thrust_key) < 0, GetKeyState(fire_key) < 0, GetKeyState(pause_key) < 0 };
	}

	////////////////////////////////////////////////////////////////////////////////
	// Maths
	////////////////////////////////////////////////////////////////////////////////

	class polar_2d
	{
	public:
		polar_2d(float r_in, float theta_in);
		float r() const;
		float theta() const;

	private:
		float m_r;
		float m_theta;
	};

	class stadium
	{
	public:
		stadium(point_2d c1, point_2d c2, float radius);
		bool intersects(circle) const;
		path_builder path() const;

	private:
		point_2d m_c1;
		point_2d m_c2;
		float m_radius;
	};

	template <class T> constexpr T	eighth_tau = T(std::experimental::io2d::tau<T> / T(8.0L));

	polar_2d::polar_2d(float r_in, float theta_in)
		: m_r(r_in)
		, m_theta(theta_in)
	{}

	point_2d pol_to_car(const polar_2d& p)
	{
		return{ p.r() * std::cos(p.theta()), p.r() * std::sin(p.theta()) };
	}

	polar_2d car_to_pol(point_2d v)
	{
		return{ v.magnitude(), std::atan2(v.y(), v.x()) };
	}

	float minimum_distance_from_line_segment(point_2d e1, point_2d e2, point_2d p)
	{
		const auto d_sq = (e1 - e2).magnitude_squared();
		if (d_sq == 0.0f)
		{
			return (e1 - p).magnitude();
		}
		const auto t = std::max(0.0f, std::min(1.0f, (p - e1).dot(e2 - e1) / d_sq));
		const auto projection = e1 + t * (e2 - e1);
		return (p - projection).magnitude();
	}

	point_2d rotate(point_2d point, float theta, point_2d origin)
	{
		const auto translation = point - origin;
		const auto s = std::sin(theta);
		const auto c = std::cos(theta);

		return point_2d{ translation.x() * c - translation.y() * s, translation.x() * s + translation.y() * c } +origin;
	}

	point_2d extend_segment(point_2d begin, point_2d end, float extension)
	{
		return end - (begin - end).to_unit() * extension;
	}

	bool contains(const bounding_box& r, point_2d p)
	{
		return p.x() >= r.x() &&
			p.x() <= r.x() + r.width() &&
			p.y() >= r.y() &&
			p.y() <= r.y() + r.height();
	}

	bool intersects(point_2d a1, point_2d a2, point_2d b1, point_2d b2)
	{
		// First test: do they share an end point?
		if ((a1 == b1) ||
			(a2 == b1) ||
			(a1 == b2) ||
			(a2 == b2))
			return false;

		// Translate the system so that point a1 is on the origin.
		a2 -= a1;
		b1 -= a1;
		b2 -= a1;

		// Rotate the system so that point a2 is on the positive x axis.
		auto a2_length = a2.magnitude();
		auto a2_cos = a2.x() / a2_length;
		auto a2_sin = a2.y() / a2_length;
		auto new_x = b1.x() * a2_cos + b1.y() * a2_sin;
		b1.y(b1.y() * a2_cos - b1.x() * a2_sin);
		b1.x(new_x);
		new_x = b2.x() * a2_cos + b2.y() * a2_sin;
		b2.y(b2.y() * a2_cos - b2.x() * a2_sin);
		b2.x(new_x);

		// Second test: do the endpoints of b both lie above or both lie below the x axis?
		if ((b1.y() < 0.0 && b2.y() < 0.0) ||
			(b1.y() >= 0.0 && b2.y() >= 0.0))
			return false;

		// Discover the position of the intersection point along line a.
		auto intersection_point = b2.x() + (b1.x() - b2.x()) * b2.y() / (b2.y() - b1.y());

		// Final test: does segment b crosses line a outside of segment a?
		return !(intersection_point < 0.0 || intersection_point > a2_length);
	}

	bool intersects(const bounding_box& a, const bounding_box& b)
	{
		return contains(a, b.top_left()) ||
			contains(a, b.bottom_right()) ||
			contains(a, point_2d(b.y(), b.x() + b.width())) ||
			contains(a, point_2d(b.y() + b.height(), b.x())) ||
			contains(b, a.top_left()) ||
			contains(b, a.bottom_right()) ||
			contains(b, point_2d(a.y(), a.x() + a.width())) ||
			contains(b, point_2d(a.y() + a.height(), a.x()));
	}

	float radius(const bounding_box& r)
	{
		return (r.top_left() - r.bottom_right()).magnitude() / 2.0f;
	}

	bounding_box translate(const bounding_box& r, point_2d v)
	{
		return{ r.x() + v.x(), r.y() + v.y(), r.width(), r.height() };
	}

	float polar_2d::r() const
	{
		return m_r;
	}

	float polar_2d::theta() const
	{
		return m_theta;
	}

	stadium::stadium(point_2d c1, point_2d c2, float radius)
		: m_c1(c1)
		, m_c2(c2)
		, m_radius(radius)
	{}

	bool stadium::intersects(circle c) const
	{
		return minimum_distance_from_line_segment(m_c1, m_c2, c.center()) < (m_radius + c.radius());
	}

	path_builder stadium::path() const
	{
		auto c1_c2_extent = extend_segment(m_c1, m_c2, m_radius);
		auto c1_c2e = extend_segment(m_c1, m_c2, m_radius * 2);
		auto c2_c1_extent = extend_segment(m_c2, m_c1, m_radius);
		auto c2_c1e = extend_segment(m_c2, m_c1, m_radius * 2);
		auto path = path_builder{};
		path.clear();
		path.new_figure(screen_space(rotate(c2_c1_extent, -half_pi<float>, m_c1)));
		path.quadratic_curve(screen_space(rotate(c2_c1e, -half_pi<float>, c2_c1_extent)), screen_space(c2_c1_extent));
		path.quadratic_curve(screen_space(rotate(c2_c1e, half_pi<float>, c2_c1_extent)), screen_space(rotate(c2_c1_extent, half_pi<float>, m_c1)));
		path.line(screen_space(rotate(c1_c2_extent, -half_pi<float>, m_c2)));
		path.quadratic_curve(screen_space(rotate(c1_c2e, -half_pi<float>, c1_c2_extent)), screen_space(c1_c2_extent));
		path.quadratic_curve(screen_space(rotate(c1_c2e, half_pi<float>, c1_c2_extent)), screen_space(rotate(c1_c2_extent, half_pi<float>, m_c2)));
		path.close_figure();
		return path;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Physics
	////////////////////////////////////////////////////////////////////////////////

	using pos = point_2d;
	using vel = point_2d;
	using acc = point_2d;
	using path_vertices = std::array<point_2d, 14>;

	struct path_buffer
	{
		std::size_t		m_count;
		path_vertices	m_vertices;
	};

	struct collision
	{
		const pos& m_position;
		float			m_radius;
		path_buffer& m_path;
	};

	class physics
	{
	public:
		physics(const pos& position, const vel& velocity);
		void					update(float seconds);
		void					update(float seconds, const acc&);

		std::array<physics, 2>	divide(std::mt19937&, std::uniform_real_distribution<float>&) const;
		const pos& position() const;
		const vel& velocity() const;

	private:
		pos		m_position;
		vel		m_velocity;
	};

	class controllable_physics
	{
	public:
		controllable_physics(const physics& phy, float ori);
		void		reset(const physics& phy, float ori);
		void		update(float spin, float thrust, float seconds);

		const pos& position() const;
		const vel& velocity() const;
		float		orientation() const;

	private:
		physics		m_physics;
		float		m_orientation;
	};

	physics::physics(const pos& position, const vel& velocity)
		: m_position(position)
		, m_velocity(velocity)
	{};

	const pos& physics::position() const
	{
		return m_position;
	}

	const vel& physics::velocity() const
	{
		return m_velocity;
	}

	controllable_physics::controllable_physics(const physics& phy, float ori)
		: m_physics(phy)
		, m_orientation(ori)
	{}

	void controllable_physics::reset(const physics& phy, float ori)
	{
		m_physics = phy;
		m_orientation = ori;
	}

	const pos& controllable_physics::position() const
	{
		return m_physics.position();
	}

	const vel& controllable_physics::velocity() const
	{
		return m_physics.velocity();
	}

	float controllable_physics::orientation() const
	{
		return m_orientation;
	}

	point_2d screen_space(point_2d v)
	{
		return{ v.x(), playing_field_height - v.y() };
	}

	void constrain_pos(pos& p)
	{
		while (p.x() >= playing_field_width) p.x(p.x() - playing_field_width);
		while (p.y() >= playing_field_height) p.y(p.y() - playing_field_height);
		while (p.x() < 0.0f) p.x(p.x() + playing_field_width);
		while (p.y() < 0.0f) p.y(p.y() + playing_field_height);
	}

	void constrain_vel(vel& v)
	{
		auto polar = car_to_pol(v);
		if (polar.r() > max_ship_speed)
		{
			v = pol_to_car({ max_ship_speed, polar.theta() });
		}
	}

	void physics::update(float seconds)
	{
		m_position += m_velocity * seconds;
		constrain_pos(m_position);
	}

	void physics::update(float seconds, const acc& a)
	{
		m_velocity += a * seconds;
		constrain_vel(m_velocity);
		update(seconds);
	}

	std::array<physics, 2> physics::divide(std::mt19937 & gen, std::uniform_real_distribution<float> & zero_to_one) const
	{
		auto polar = car_to_pol(m_velocity);
		auto r = polar.r();
		auto r1 = r * (1.2F + zero_to_one(gen));
		auto r2 = r * (1.2F + zero_to_one(gen));
		auto theta1 = polar.theta() + (zero_to_one(gen) - eighth_tau<float>);
		auto theta2 = polar.theta() + (zero_to_one(gen) - eighth_tau<float>);
		return{ { physics(m_position, pol_to_car({r1, theta1})), physics(m_position, pol_to_car({r2, theta2})) } };
	}

	void controllable_physics::update(float spin, float thrust_val, float seconds)
	{
		m_orientation += spin * seconds;
		auto acceleration = pol_to_car(polar_2d{ thrust_val, m_orientation });
		m_physics.update(seconds, acceleration);
	}

	std::pair<path_buffer, float> path_from_prototype(const path_buffer& path, float s)
	{
		auto pv = path_vertices{ path.m_vertices };
		auto radius = 0.0F;
		std::for_each(&pv[0], &pv[path.m_count], [&](auto& v) {v *= s; radius = std::max(radius, v.magnitude()); });
		return std::make_pair(path_buffer{ path.m_count, pv }, radius);
	}

	bool collides(const collision& col, const std::array<point_2d, 2> & line)
	{
		auto delta = col.m_position - line[0];
		if (delta.magnitude() > col.m_radius)
		{
			return false;
		}

		auto segment_start = col.m_path.m_vertices[col.m_path.m_count - 1];
		return &col.m_path.m_vertices[col.m_path.m_count] != std::find_if(&col.m_path.m_vertices[0], &col.m_path.m_vertices[col.m_path.m_count], [&](const auto& v)
			{
				if (intersects(segment_start + col.m_position, v + col.m_position, line[0], line[1]))
				{
					return true;
				}
				segment_start = v;
				return false;
			});
	}

	bool collides(const collision& col_a, const collision& col_b)
	{
		auto delta = col_a.m_position - col_b.m_position;
		if (delta.magnitude() > (col_a.m_radius + col_b.m_radius))
		{
			return false;
		}

		auto segment_start = col_a.m_path.m_vertices[col_a.m_path.m_count - 1];
		return &col_a.m_path.m_vertices[col_a.m_path.m_count] != std::find_if(&col_a.m_path.m_vertices[0], &col_a.m_path.m_vertices[col_a.m_path.m_count], [&](const auto& v)
			{
				if (collides(col_b, { { segment_start + col_a.m_position, v + col_a.m_position } }))
				{
					return true;
				}
				segment_start += v;
				return false;
			});
	}

	////////////////////////////////////////////////////////////////////////////////
	// Asteroid
	////////////////////////////////////////////////////////////////////////////////

	struct asteroid_destruction
	{
		int				m_score;
		float			m_size;
		const physics* m_physics;
	};

	class asteroid
	{
	public:
		asteroid(physics&& phys, std::pair<const path_buffer&, float> path, float s);
		bool						update(float seconds);
		asteroid_destruction		destroy();

		void						draw(unmanaged_output_surface& ds, bool draw_sweep) const;
		bool						active() const;
		const collision& collision_data() const;
		stadium						sweep(float duration) const;

		static const path_buffer	a1;
		static const path_buffer	a2;
		static const path_buffer	a3;
		static const path_buffer	a4;

	private:
		physics			m_physics;
		path_buffer		m_path;
		float			m_size;
		collision		m_collision;
		bool			m_active;
	};

	const path_vertices a1_vb = { { point_2d{ -2, 4 },{ 2, -2 },{ 2, 2 },{ 2, -2 },{ -1, -2 },{ 1, -2 },{ -3, -2 },{ -3, 0 },{ -2, 2 },{ 0, 4 },{ 2, 2 } } };
	const path_vertices a2_vb = { { point_2d{ -2, 4 },{ 2, -1 },{ 2, 1 },{ 2, -2 },{ -2, -1 },{ 2, -2 },{ -2, -3 },{ -3, 1 },{ -1, -1 },{ -2, 2 },{ 1, 2 },{ -1, 2 },{ 2, 2 } } };
	const path_vertices a3_vb = { { point_2d{ -2, 4 },{ 3, 0 },{ 3, -2 },{ 0, -1 },{ -3, -1 },{ 3, -2 },{ -2, -2 },{ -4, 0 },{ -2, 3 },{ 0, 3 },{ 3, 0 },{ -1, 2 } } };
	const path_vertices a4_vb = { { point_2d{ -1, 4 },{ 3, 0 },{ 2, -3 },{ 0, -2 },{ -2, -3 },{ -2, 0 },{ 0, 3 },{ -2, -3 },{ -2, 3 },{ 2, 1 },{ -2, 1 },{ 3, 3 } } };
	const path_buffer asteroid::a1 = { 11, a1_vb };
	const path_buffer asteroid::a2 = { 13, a2_vb };
	const path_buffer asteroid::a3 = { 12, a3_vb };
	const path_buffer asteroid::a4 = { 12, a4_vb };

	asteroid::asteroid(physics&& phys, std::pair<const path_buffer&, float> path, float s)
		: m_physics(phys)
		, m_path(path.first)
		, m_size(s)
		, m_collision{ m_physics.position(), path.second, m_path }
		, m_active(true)
	{};

	bool asteroid::active() const
	{
		return m_active;
	}

	const collision& asteroid::collision_data() const
	{
		return m_collision;
	}

	stadium asteroid::sweep(float duration) const
	{
		return { m_physics.position(), m_physics.position() + m_physics.velocity() * duration, m_collision.m_radius };
	}

	void asteroid::draw(unmanaged_output_surface& ds, bool draw_sweep) const
	{
		using namespace std::experimental::io2d;

		if (!m_active) return;

		auto path = path_builder{};
		path.clear();
		auto v = m_physics.position() + (m_path.m_vertices[0]);
		path.new_figure(screen_space(v));
		std::for_each(&m_path.m_vertices[1], &m_path.m_vertices[m_path.m_count], [&](const auto& vert)
			{
				v += vert;
				path.line(screen_space(v));
			});
		path.close_figure();

		ds.stroke(brush{ rgba_color::gray }, path, std::nullopt, std::nullopt, std::nullopt, scale_to_screen);
		if (draw_sweep)
		{
			ds.stroke(brush{ rgba_color::cornflower_blue }, sweep(asteroid_sweep_lookahead).path(), std::nullopt, std::nullopt, std::nullopt, scale_to_screen /*, clip props*/);
		}
	}

	bool asteroid::update(float seconds)
	{
		if (!m_active)
		{
			return false;
		}
		m_physics.update(seconds);
		return true;
	}

	asteroid_destruction asteroid::destroy()
	{
		m_active = false;
		if (m_size == big_asteroid_size)	return{ big_asteroid_score, medium_asteroid_size, &m_physics };
		if (m_size == medium_asteroid_size) return{ medium_asteroid_score, small_asteroid_size, &m_physics };
		return{ small_asteroid_score, 0.0, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////
	// Ship
	////////////////////////////////////////////////////////////////////////////////

	struct ship_update
	{
		bool		m_launch;
		point_2d	m_position;
		float		m_orientation;
		path_buffer	m_path;
	};

	class ship
	{
	public:
		ship(const controllable_physics&);
		ship_update		update(float seconds);
		void			destroy();
		void			draw(unmanaged_output_surface& ds);
		circle			sweep() const;
		bool			active() const;

	private:
		enum class ship_state
		{
			waiting,
			active,
			exploding
		};
		controllable_physics					m_physics;
		path_buffer								m_path;
		ship_state								m_state;
		std::chrono::steady_clock::time_point	m_state_change;
	};

	class missile
	{
	public:
		missile(point_2d = { 0.0, 0.0 }, float = 0.0, bool = false);
		bool					update(float seconds);
		void					destroy();
		bool					active() const;
		void					draw(unmanaged_output_surface& ds);
		std::array<point_2d, 2>	collision_data() const;

	private:
		physics		m_physics;
		float		m_age;
	};

	const path_vertices ship_vb{ { point_2d{ 8, 0 },{ -15, -5 },{ 2, 3 },{ 0, 5 },{ -2, 3 },{ 15, -5 } } };
	const auto ship_path_radius{ 9.7f };
	const auto ship_shape = path_buffer{ 6, ship_vb };
	const auto wait_period{ 1 };
	const auto destruct_period{ 1 };

	float ship_radius()
	{
		return ship_path_radius;
	}

	void ship::draw(unmanaged_output_surface& ds)
	{
		using namespace std::experimental::io2d;

		switch (m_state)
		{
		case ship_state::waiting:
		{
			break;
		}
		case ship_state::active:
		{
			auto path = path_builder{};
			path.clear();
			auto v = m_physics.position() + (m_path.m_vertices[0]);
			path.new_figure(screen_space(v));
			std::for_each(&m_path.m_vertices[1], &m_path.m_vertices[m_path.m_count], [&](const auto& vert)
				{
					v += vert;
					path.line(screen_space(v));
				});

			ds.stroke(brush{ rgba_color::white }, path, std::nullopt, std::nullopt, std::nullopt, scale_to_screen);
			break;
		}
		case ship_state::exploding:
		{
			break;
		}
		}
	}

	void missile::draw(unmanaged_output_surface& ds)
	{
		using namespace std::experimental::io2d;

		if (!active()) return;

		auto path = path_builder{};
		path.new_figure(screen_space(m_physics.position()));
		path.line(screen_space(m_physics.position() - (m_physics.velocity() / missile_speed)));

		ds.stroke(brush{ rgba_color::white }, path, std::nullopt, std::nullopt, std::nullopt, scale_to_screen);
	}

	std::array<point_2d, 2> missile::collision_data() const
	{
		return{ { m_physics.position(), m_physics.position() - m_physics.velocity() } };
	}

	std::experimental::io2d::circle ship::sweep() const
	{
		return circle(m_physics.position(), ship_radius());
	}

	bool ship::active() const
	{
		return m_state == ship_state::active;
	}

	ship::ship(const controllable_physics& cp)
		: m_physics(cp)
		, m_path(ship_shape)
		, m_state(ship_state::waiting)
		, m_state_change(std::chrono::steady_clock::now())
	{}

	ship_update ship::update(float seconds)
	{
		switch (m_state)
		{
		case ship_state::waiting:
		{
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::seconds>(now - m_state_change).count() > wait_period)
			{
				m_state = ship_state::active;
				m_state_change = now;
			}
			return{ false, m_physics.position(), m_physics.orientation(), m_path };
		}
		case ship_state::active:
		{
			auto spin_val = do_anti_clockwise() ? spin_left
				: (do_clockwise() ? spin_right : 0.0f);
			auto thrust_val = do_thrust() ? thrust : 0.0f;
			m_physics.update(spin_val, thrust_val, seconds);

			std::transform(std::begin(ship_vb), std::next(std::begin(ship_vb), ship_shape.m_count), std::begin(m_path.m_vertices), [&](const auto& v_in)
				{
					return rotate(v_in, m_physics.orientation(), { 0.0f, 0.0f });
				});
			return{ do_fire(), m_physics.position(), m_physics.orientation(), m_path };
		}
		case ship_state::exploding:
		{
			auto now = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::seconds>(now - m_state_change).count() > destruct_period)
			{
				m_physics.reset(physics{ point_2d{ playing_field_width / 2, playing_field_height / 2 },{ point_2d{ 0, 0 } } }, 0.0f);
				m_state = ship_state::waiting;
				m_state_change = now;
			}
			return{ false, m_physics.position(), m_physics.orientation(), m_path };
		}
		default:
		{
			throw ::std::system_error(::std::make_error_code(::std::errc::invalid_argument));
		} break;
		}
	}

	void ship::destroy()
	{
		m_state = ship_state::exploding;
	}

	missile::missile(point_2d position, float orientation, bool active)
		: m_physics(position, pol_to_car({ missile_speed, orientation }))
		, m_age(active ? 0.0F : missile_lifespan)
	{}

	bool missile::update(float seconds)
	{
		if (!active()) return true;

		m_physics.update(seconds);
		m_age += seconds;
		return active();
	}

	void missile::destroy()
	{
		m_age = missile_lifespan;
	}

	bool missile::active() const
	{
		return (m_age < missile_lifespan);
	}

	////////////////////////////////////////////////////////////////////////////////
	// Game
	////////////////////////////////////////////////////////////////////////////////

	constexpr auto level_transition_period = 1;
	constexpr auto new_ship_transition_period = 2;

	class game
	{
	public:
		game();

		void	update(unmanaged_output_surface&);

	private:
		enum class game_state
		{
			new_level,
			new_ship,
			active,
		};
		bool	update_asteroids(float seconds, std::vector<asteroid_destruction>&);
		void	update_ship(float seconds, std::vector<asteroid_destruction>&);
		void	update_missiles(float seconds, std::vector<asteroid_destruction>&);

		void	draw_asteroids(unmanaged_output_surface&);
		void	draw_ship(unmanaged_output_surface&);
		void	draw_missiles(unmanaged_output_surface&);

		void	generate_level();

		int										m_level;
		int										m_score;
		ship									m_ship;
		std::vector<asteroid>					m_asteroids;
		size_t									m_next_ship_missile;
		std::vector<missile>					m_ship_missiles;
		std::random_device						m_rd;
		std::mt19937							m_gen;
		std::uniform_int_distribution<>			m_0_to_3;
		std::uniform_real_distribution<float>	m_0_to_1;
		game_state								m_state;
		std::chrono::steady_clock::time_point	m_state_change;
		bool									m_pause;
	};

	void game::update(unmanaged_output_surface& ds)
	{
		using namespace std::experimental::io2d;

		static auto last_update = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();

		get_key_states();

		if (do_pause())
		{
			m_pause = !m_pause;
		}

		if (!m_pause && m_state == game_state::new_level)
		{
			if (std::chrono::duration_cast<std::chrono::seconds>(now - m_state_change).count() > level_transition_period)
			{
				++m_level;
				generate_level();
				m_state = game_state::active;
				m_state_change = now;
			}
		}

		if (!m_pause && m_state == game_state::new_ship)
		{
			if (std::chrono::duration_cast<std::chrono::seconds>(now - m_state_change).count() > new_ship_transition_period &&
				std::find_if(std::begin(m_asteroids), std::end(m_asteroids), [&](const auto& a)
					{
						// Will the asteroid collide with the ship in the next two seconds?
						// Does the stadium described by the path of the asteroid's bounding circle
						// over the next two seconds intersect the ship's bounding circle?
						return a.sweep(asteroid_sweep_lookahead).intersects(m_ship.sweep());
					}) == std::end(m_asteroids))
			{
				m_state = game_state::active;
				m_state_change = now;
			}
		}

		auto ad = std::vector<asteroid_destruction>{};
		ad.reserve(max_missiles);

		auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update).count() / 1000.0f;
		if (interval > 0)
		{
			if (!m_pause)
			{
				update_ship(interval, ad);
				update_missiles(interval, ad);
				if (!update_asteroids(interval, ad))
				{
					m_state = game_state::new_level;
					m_state_change = now;
				}
			}
			last_update = now;
		}

		ds.paint(brush{ rgba_color::black });
		draw_asteroids(ds);
		draw_ship(ds);
		draw_missiles(ds);
	}

	void game::draw_asteroids(unmanaged_output_surface& ds)
	{
		for (auto& a : m_asteroids) { a.draw(ds, m_pause); }
	}

	void game::draw_ship(unmanaged_output_surface& ds)
	{
		m_ship.draw(ds);
	}

	void game::draw_missiles(unmanaged_output_surface& ds)
	{
		for (auto& m : m_ship_missiles) { m.draw(ds); }
	}

	const path_buffer* asteroid_vbs[] = { &asteroid::a1, &asteroid::a2, &asteroid::a3, &asteroid::a4 };

	game::game()
		: m_level{ 0 }
		, m_score{ 0 }
		//, m_lives{ 0 }
		, m_ship(controllable_physics{
		physics{ point_2d{ playing_field_width / 2, playing_field_height / 2 },{ point_2d{ 0, 0 } } },
		0.0f })
		//, m_ship_missile_count{ 0 }
		, m_next_ship_missile(0)
		, m_ship_missiles(max_missiles)
		, m_rd()
		, m_gen(m_rd())
		, m_0_to_3(0, 3)
		, m_0_to_1(0, 1)
		, m_state(game_state::new_level)
		, m_state_change(std::chrono::steady_clock::now())
		, m_pause(false)
	{
	}

	void game::generate_level()
	{
		auto count = m_level * 2 + 2;
		std::uniform_int_distribution<> edge(1, 4);
		std::uniform_real_distribution<float> x(0.0F, playing_field_width);
		std::uniform_real_distribution<float> y(0.0F, playing_field_height);
		std::uniform_real_distribution<float> theta(0.0F, std::experimental::io2d::tau<float>);

		m_asteroids.clear();
		m_asteroids.reserve(static_cast<::std::size_t>(count * 7));

		while (count-- != 0)
		{
			switch (edge(m_gen))
			{
			case 1:	m_asteroids.emplace_back(physics{ point_2d{ 0.0F, y(m_gen) },					pol_to_car(polar_2d{ initial_speed, theta(m_gen) }) }, path_from_prototype(*asteroid_vbs[m_0_to_3(m_gen)], big_asteroid_size), big_asteroid_size); break;
			case 2:	m_asteroids.emplace_back(physics{ point_2d{ playing_field_width, y(m_gen) },	pol_to_car(polar_2d{ initial_speed, theta(m_gen) }) }, path_from_prototype(*asteroid_vbs[m_0_to_3(m_gen)], big_asteroid_size), big_asteroid_size); break;
			case 3:	m_asteroids.emplace_back(physics{ point_2d{ x(m_gen), 0.0F },					pol_to_car(polar_2d{ initial_speed, theta(m_gen) }) }, path_from_prototype(*asteroid_vbs[m_0_to_3(m_gen)], big_asteroid_size), big_asteroid_size); break;
			case 4:	m_asteroids.emplace_back(physics{ point_2d{ x(m_gen), playing_field_height },	pol_to_car(polar_2d{ initial_speed, theta(m_gen) }) }, path_from_prototype(*asteroid_vbs[m_0_to_3(m_gen)], big_asteroid_size), big_asteroid_size); break;
			}
		}
	}

	void game::update_ship(float seconds, std::vector<asteroid_destruction>& ad)
	{
		auto ship_update = m_ship.update(seconds);
		if (ship_update.m_launch && std::count_if(std::begin(m_ship_missiles), std::end(m_ship_missiles), [](const auto& m) { return m.active(); }) < max_missiles)
		{
			m_ship_missiles[m_next_ship_missile] = { ship_update.m_position, ship_update.m_orientation, true };
			m_next_ship_missile = ++m_next_ship_missile % max_missiles;
		}
		if (m_ship.active())
		{
			for (auto& a : m_asteroids)
			{
				if (a.active() && collides(a.collision_data(), { ship_update.m_position, ship_radius(), ship_update.m_path }))
				{
					m_ship.destroy();
					ad.push_back(a.destroy());
				}
			}
		}
	}

	void game::update_missiles(float seconds, std::vector<asteroid_destruction>& ad)
	{
		for (auto& m : m_ship_missiles)
		{
			if (!m.active()) continue;
			if (!m.update(seconds))
			{
				m.destroy();
			}
			for (auto& a : m_asteroids)
			{
				if (a.active() && collides(a.collision_data(), m.collision_data()))
				{
					ad.push_back(a.destroy());
					m.destroy();
				}
			}
		}
	}

	bool game::update_asteroids(float seconds, std::vector<asteroid_destruction>& ad)
	{
		if (m_state == game_state::new_level)
		{
			return true;
		}
		for (auto& next_asteroids : ad)
		{
			m_score += next_asteroids.m_score;
			if (!next_asteroids.m_physics) continue;

			auto new_physics = next_asteroids.m_physics->divide(m_gen, m_0_to_1);
			auto path1 = path_from_prototype(*asteroid_vbs[m_0_to_3(m_gen)], next_asteroids.m_size);
			m_asteroids.emplace_back(std::move(new_physics[0]), path1, next_asteroids.m_size);
			auto path2 = path_from_prototype(*asteroid_vbs[m_0_to_3(m_gen)], next_asteroids.m_size);
			m_asteroids.emplace_back(std::move(new_physics[1]), path2, next_asteroids.m_size);
		}
		return std::count_if(std::begin(m_asteroids), std::end(m_asteroids), [=](auto& a) { return a.update(seconds); }) > 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Slide
	////////////////////////////////////////////////////////////////////////////////

	class rocks_in_space : public show::slide
	{
	public:
		rocks_in_space(show::presentation);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		game m_game;
	};

	rocks_in_space::rocks_in_space(show::presentation p)
		: show::slide(p)
	{}

	bool rocks_in_space::enter()
	{
//		scale_to_screen.surface_matrix(matrix_2d::create_scale(point_2d{ 3.f, 2.25f }));
		scale_to_screen.surface_matrix(matrix_2d::create_scale(point_2d{ 2.f, 2.f }));
		return true;
	}

	void rocks_in_space::render(unmanaged_output_surface& s)
	{
		m_game.update(s);
	}

	bool rocks_in_space::exit()
	{
		return true;
	}

	rocks_in_space s079(show::presentation::SLIDE_079);
}

