#pragma once

#include <io2d.h>

#include <string>

namespace show
{
	using namespace std::experimental::io2d;

	/*
	The program class
	*/
	class show
	{
	public:
		show(std::string const& cmd_line);
		void update(unmanaged_output_surface&);

		std::string const& res_root() const;
	private:
		std::string const m_res_root;
	};

	/*
	Useful functions
	*/

	template <typename T, typename U>
	T delta(T begin, T end, U fraction);

	enum presentation
	{
		SLIDE_FIRST,
		SLIDE_001 = SLIDE_FIRST,
		SLIDE_002,
		SLIDE_003,
		SLIDE_004,
		SLIDE_005,
		SLIDE_006,
		SLIDE_007,
		SLIDE_008,
		SLIDE_009,
		SLIDE_010,
		SLIDE_011,
		SLIDE_012,
		SLIDE_013,
		SLIDE_014,
		SLIDE_015,
		SLIDE_016,
		SLIDE_017,
		SLIDE_018,
		SLIDE_019,
		SLIDE_020,
		SLIDE_021,
		SLIDE_022,
		SLIDE_023,
		SLIDE_024,
		SLIDE_025,
		SLIDE_026,
		SLIDE_027,
		SLIDE_028,
		SLIDE_029,
		SLIDE_030,
		SLIDE_031,
		SLIDE_032,
		SLIDE_033,
		SLIDE_034,
		SLIDE_035,
		SLIDE_036,
		SLIDE_037,
		SLIDE_038,
		SLIDE_039,
		SLIDE_040,
		SLIDE_041,
		SLIDE_042,
		SLIDE_043,
		SLIDE_044,
		SLIDE_045,
		SLIDE_046,
		SLIDE_047,
		SLIDE_048,
		SLIDE_049,
		SLIDE_050,
		SLIDE_051,
		SLIDE_052,
		SLIDE_053,
		SLIDE_054,
		SLIDE_055,
		SLIDE_056,
		SLIDE_057,
		SLIDE_058,
		SLIDE_059,
		SLIDE_060,
		SLIDE_061,
		SLIDE_062,
		SLIDE_063,
		SLIDE_064,
		SLIDE_065,
		SLIDE_066,
		SLIDE_067,
		SLIDE_068,
		SLIDE_069,
		SLIDE_070,
		SLIDE_071,
		SLIDE_072,
		SLIDE_073,
		SLIDE_074,
		SLIDE_075,
		SLIDE_076,
		SLIDE_077,
		SLIDE_078,
		SLIDE_079,
		SLIDE_080,
		SLIDE_081,
		SLIDE_082,
		SLIDE_083,
		SLIDE_084,
		SLIDE_085,
		SLIDE_086,
		SLIDE_087,
		SLIDE_088,
		SLIDE_089,
		SLIDE_090,
		SLIDE_091,
		SLIDE_092,
		SLIDE_093,
		SLIDE_094,
		SLIDE_095,
		SLIDE_096,
		SLIDE_097,
		SLIDE_098,
		SLIDE_099,
		SLIDE_100,
		SLIDE_101,
		SLIDE_102,
		SLIDE_103,
		SLIDE_104,
		SLIDE_105,
		SLIDE_106,
		SLIDE_107,
		SLIDE_108,
		SLIDE_109,
		SLIDE_110,
		SLIDE_111,
		SLIDE_112,
		SLIDE_113,
		SLIDE_114,
		SLIDE_115,
		SLIDE_116,
		SLIDE_117,
		SLIDE_118,
		SLIDE_119,
		SLIDE_120,
		SLIDE_121,
		SLIDE_122,
		SLIDE_123,

		SLIDE_LAST
	};
}

template <typename T, typename U>
T show::delta(T begin, T end, U fraction)
{
	return begin + ((end - begin) * fraction);
}

/*
Slide animations

01 *11 NumberLine
02 *12 Axes
03 *13 Polar
04 *14 StraightLine
05 *15 StraightLine2
06 *16 Parabola
07 *17 Circle
08 *18 Circle2
09 *30 Translate
10 *31 Scale
11 *32 Shear
12 *33 Reflect
13 *34 Rotate
14 *42 LineSegment
15 *55 ParabolaDescription
16 *64 BezierAnimation
17 *67 Triangle
18 *69 Square
19 *73 NonOrthogonalAxes
20 *74 Circle3
21 *76 Ellipse
22 *78 Stadium
23 *79 Asteroids
25 *81 WildCurve
26 *93 FloatDistribution
27 *123 Credits
*/