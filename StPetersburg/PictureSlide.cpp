#include "StPetersburg.h"

#include "Slide.h"

#include <io2d.h>

#include <map>

namespace
{
	using namespace std::experimental::io2d;

	class picture_slide : public show::slide
	{
	public:
		picture_slide(show::presentation, std::string const&);
		bool enter() override;
		void render(unmanaged_output_surface&) override;
		bool exit() override;
	private:
		show::background_image m_bg;
	};


	picture_slide::picture_slide(show::presentation slide_ID, std::string const& filename)
		: slide{ slide_ID }
		, m_bg(filename)
	{}

	bool picture_slide::enter()
	{
		m_bg.prepare();
		return true;
	}

	void picture_slide::render(unmanaged_output_surface& uos)
	{
		m_bg.render(uos);
	}

	bool picture_slide::exit()
	{
		return true;
	}

	picture_slide s001{ show::presentation::SLIDE_001, pres::res + "Geometry St Petersburg (1).png" };
	//picture_slide s002{ show::presentation::SLIDE_002, pres::res + "Geometry St Petersburg (2).png" };
	picture_slide s003{ show::presentation::SLIDE_003, pres::res + "Geometry St Petersburg (3).png" };
	picture_slide s004{ show::presentation::SLIDE_004, pres::res + "Geometry St Petersburg (4).png" };
	picture_slide s005{ show::presentation::SLIDE_005, pres::res + "Geometry St Petersburg (5).png" };
	picture_slide s006{ show::presentation::SLIDE_006, pres::res + "Geometry St Petersburg (6).png" };
	picture_slide s007{ show::presentation::SLIDE_007, pres::res + "Geometry St Petersburg (7).png" };
	picture_slide s008{ show::presentation::SLIDE_008, pres::res + "Geometry St Petersburg (8).png" };
	picture_slide s009{ show::presentation::SLIDE_009, pres::res + "Geometry St Petersburg (9).png" };
	picture_slide s010{ show::presentation::SLIDE_010, pres::res + "Geometry St Petersburg (10).png" };
	//picture_slide s011{ show::presentation::SLIDE_011, pres::res + "Geometry St Petersburg (11).png" }; // NumberLine
	//picture_slide s012{ show::presentation::SLIDE_012, pres::res + "Geometry St Petersburg (12).png" }; // Axes
	//picture_slide s013{ show::presentation::SLIDE_013, pres::res + "Geometry St Petersburg (13).png" }; // Polar
	//picture_slide s014{ show::presentation::SLIDE_014, pres::res + "Geometry St Petersburg (14).png" }; // StraightLine
	//picture_slide s015{ show::presentation::SLIDE_015, pres::res + "Geometry St Petersburg (15).png" }; // StraightLine2
	//picture_slide s016{ show::presentation::SLIDE_016, pres::res + "Geometry St Petersburg (16).png" }; // Parabola
	//picture_slide s017{ show::presentation::SLIDE_017, pres::res + "Geometry St Petersburg (17).png" }; // Circle
	//picture_slide s018{ show::presentation::SLIDE_018, pres::res + "Geometry St Petersburg (18).png" }; // Circle2
	picture_slide s019{ show::presentation::SLIDE_019, pres::res + "Geometry St Petersburg (19).png" };
	picture_slide s020{ show::presentation::SLIDE_020, pres::res + "Geometry St Petersburg (20).png" };
	picture_slide s021{ show::presentation::SLIDE_021, pres::res + "Geometry St Petersburg (21).png" };
	picture_slide s022{ show::presentation::SLIDE_022, pres::res + "Geometry St Petersburg (22).png" };
	picture_slide s023{ show::presentation::SLIDE_023, pres::res + "Geometry St Petersburg (23).png" };
	picture_slide s024{ show::presentation::SLIDE_024, pres::res + "Geometry St Petersburg (24).png" };
	picture_slide s025{ show::presentation::SLIDE_025, pres::res + "Geometry St Petersburg (25).png" };
	picture_slide s026{ show::presentation::SLIDE_026, pres::res + "Geometry St Petersburg (26).png" };
	picture_slide s027{ show::presentation::SLIDE_027, pres::res + "Geometry St Petersburg (27).png" };
	picture_slide s028{ show::presentation::SLIDE_028, pres::res + "Geometry St Petersburg (28).png" };
	picture_slide s029{ show::presentation::SLIDE_029, pres::res + "Geometry St Petersburg (29).png" };
	//picture_slide s030{ show::presentation::SLIDE_030, pres::res + "Geometry St Petersburg (30).png" }; // Translate
	//picture_slide s031{ show::presentation::SLIDE_031, pres::res + "Geometry St Petersburg (31).png" }; // Scale
	//picture_slide s032{ show::presentation::SLIDE_032, pres::res + "Geometry St Petersburg (32).png" }; // Shear
	//picture_slide s033{ show::presentation::SLIDE_033, pres::res + "Geometry St Petersburg (33).png" }; // Reflect
	//picture_slide s034{ show::presentation::SLIDE_034, pres::res + "Geometry St Petersburg (34).png" }; // Rotate
	picture_slide s035{ show::presentation::SLIDE_035, pres::res + "Geometry St Petersburg (35).png" };
	picture_slide s036{ show::presentation::SLIDE_036, pres::res + "Geometry St Petersburg (36).png" };
	picture_slide s037{ show::presentation::SLIDE_037, pres::res + "Geometry St Petersburg (37).png" };
	picture_slide s038{ show::presentation::SLIDE_038, pres::res + "Geometry St Petersburg (38).png" };
	picture_slide s039{ show::presentation::SLIDE_039, pres::res + "Geometry St Petersburg (39).png" };
	picture_slide s040{ show::presentation::SLIDE_040, pres::res + "Geometry St Petersburg (40).png" };
	picture_slide s041{ show::presentation::SLIDE_041, pres::res + "Geometry St Petersburg (41).png" };
	//picture_slide s042{ show::presentation::SLIDE_042, pres::res + "Geometry St Petersburg (42).png" }; // LineSegment
	picture_slide s043{ show::presentation::SLIDE_043, pres::res + "Geometry St Petersburg (43).png" };
	picture_slide s044{ show::presentation::SLIDE_044, pres::res + "Geometry St Petersburg (44).png" };
	picture_slide s045{ show::presentation::SLIDE_045, pres::res + "Geometry St Petersburg (45).png" };
	picture_slide s046{ show::presentation::SLIDE_046, pres::res + "Geometry St Petersburg (46).png" };
	picture_slide s047{ show::presentation::SLIDE_047, pres::res + "Geometry St Petersburg (47).png" };
	picture_slide s048{ show::presentation::SLIDE_048, pres::res + "Geometry St Petersburg (48).png" };
	picture_slide s049{ show::presentation::SLIDE_049, pres::res + "Geometry St Petersburg (49).png" };
	picture_slide s050{ show::presentation::SLIDE_050, pres::res + "Geometry St Petersburg (50).png" };
	picture_slide s051{ show::presentation::SLIDE_051, pres::res + "Geometry St Petersburg (51).png" };
	picture_slide s052{ show::presentation::SLIDE_052, pres::res + "Geometry St Petersburg (52).png" };
	picture_slide s053{ show::presentation::SLIDE_053, pres::res + "Geometry St Petersburg (53).png" };
	picture_slide s054{ show::presentation::SLIDE_054, pres::res + "Geometry St Petersburg (54).png" };
	//picture_slide s055{ show::presentation::SLIDE_055, pres::res + "Geometry St Petersburg (55).png" }; // ParabolaDescription
	picture_slide s056{ show::presentation::SLIDE_056, pres::res + "Geometry St Petersburg (56).png" };
	picture_slide s057{ show::presentation::SLIDE_057, pres::res + "Geometry St Petersburg (57).png" };
	picture_slide s058{ show::presentation::SLIDE_058, pres::res + "Geometry St Petersburg (58).png" };
	picture_slide s059{ show::presentation::SLIDE_059, pres::res + "Geometry St Petersburg (59).png" };
	picture_slide s060{ show::presentation::SLIDE_060, pres::res + "Geometry St Petersburg (60).png" };
	picture_slide s061{ show::presentation::SLIDE_061, pres::res + "Geometry St Petersburg (61).png" };
	picture_slide s062{ show::presentation::SLIDE_062, pres::res + "Geometry St Petersburg (62).png" };
	picture_slide s063{ show::presentation::SLIDE_063, pres::res + "Geometry St Petersburg (63).png" };
	//picture_slide s064{ show::presentation::SLIDE_064, pres::res + "Geometry St Petersburg (64).png" }; // BezierAnimation
	picture_slide s065{ show::presentation::SLIDE_065, pres::res + "Geometry St Petersburg (65).png" };
	picture_slide s066{ show::presentation::SLIDE_066, pres::res + "Geometry St Petersburg (66).png" };
	//picture_slide s067{ show::presentation::SLIDE_067, pres::res + "Geometry St Petersburg (67).png" }; // Triangle
	picture_slide s068{ show::presentation::SLIDE_068, pres::res + "Geometry St Petersburg (68).png" };
	//picture_slide s069{ show::presentation::SLIDE_069, pres::res + "Geometry St Petersburg (69).png" }; // Square
	picture_slide s070{ show::presentation::SLIDE_070, pres::res + "Geometry St Petersburg (70).png" };
	picture_slide s071{ show::presentation::SLIDE_071, pres::res + "Geometry St Petersburg (71).png" };
	picture_slide s072{ show::presentation::SLIDE_072, pres::res + "Geometry St Petersburg (72).png" };
	//picture_slide s073{ show::presentation::SLIDE_073, pres::res + "Geometry St Petersburg (73).png" }; // NonOrthogonalAxes
	//picture_slide s074{ show::presentation::SLIDE_074, pres::res + "Geometry St Petersburg (74).png" }; // Circle3
	picture_slide s075{ show::presentation::SLIDE_075, pres::res + "Geometry St Petersburg (75).png" };
	//picture_slide s076{ show::presentation::SLIDE_076, pres::res + "Geometry St Petersburg (76).png" }; // Ellipse
	picture_slide s077{ show::presentation::SLIDE_077, pres::res + "Geometry St Petersburg (77).png" };
	//picture_slide s078{ show::presentation::SLIDE_078, pres::res + "Geometry St Petersburg (78).png" }; // Stadium
	//picture_slide s079{ show::presentation::SLIDE_079, pres::res + "Geometry St Petersburg (79).png" }; // Asteroids
	picture_slide s080{ show::presentation::SLIDE_080, pres::res + "Geometry St Petersburg (80).png" };
	picture_slide s081{ show::presentation::SLIDE_081, pres::res + "Geometry St Petersburg (81).png" };
	picture_slide s082{ show::presentation::SLIDE_082, pres::res + "Geometry St Petersburg (82).png" };
	picture_slide s083{ show::presentation::SLIDE_083, pres::res + "Geometry St Petersburg (83).png" };
	picture_slide s084{ show::presentation::SLIDE_084, pres::res + "Geometry St Petersburg (84).png" };
	picture_slide s085{ show::presentation::SLIDE_085, pres::res + "Geometry St Petersburg (85).png" };
	picture_slide s086{ show::presentation::SLIDE_086, pres::res + "Geometry St Petersburg (86).png" };
	picture_slide s087{ show::presentation::SLIDE_087, pres::res + "Geometry St Petersburg (87).png" };
	picture_slide s088{ show::presentation::SLIDE_088, pres::res + "Geometry St Petersburg (88).png" };
	picture_slide s089{ show::presentation::SLIDE_089, pres::res + "Geometry St Petersburg (89).png" };
	picture_slide s090{ show::presentation::SLIDE_090, pres::res + "Geometry St Petersburg (90).png" };
	picture_slide s091{ show::presentation::SLIDE_091, pres::res + "Geometry St Petersburg (91).png" };
	picture_slide s092{ show::presentation::SLIDE_092, pres::res + "Geometry St Petersburg (92).png" };
	picture_slide s093{ show::presentation::SLIDE_093, pres::res + "Geometry St Petersburg (93).png" };
	picture_slide s094{ show::presentation::SLIDE_094, pres::res + "Geometry St Petersburg (94).png" };
	picture_slide s095{ show::presentation::SLIDE_095, pres::res + "Geometry St Petersburg (95).png" };
	picture_slide s096{ show::presentation::SLIDE_096, pres::res + "Geometry St Petersburg (96).png" };
	picture_slide s097{ show::presentation::SLIDE_097, pres::res + "Geometry St Petersburg (97).png" };
	picture_slide s098{ show::presentation::SLIDE_098, pres::res + "Geometry St Petersburg (98).png" };
	picture_slide s099{ show::presentation::SLIDE_099, pres::res + "Geometry St Petersburg (99).png" };
	picture_slide s100{ show::presentation::SLIDE_100, pres::res + "Geometry St Petersburg (100).png" };
	picture_slide s101{ show::presentation::SLIDE_101, pres::res + "Geometry St Petersburg (101).png" };
	picture_slide s102{ show::presentation::SLIDE_102, pres::res + "Geometry St Petersburg (102).png" };
	picture_slide s103{ show::presentation::SLIDE_103, pres::res + "Geometry St Petersburg (103).png" };
	picture_slide s104{ show::presentation::SLIDE_104, pres::res + "Geometry St Petersburg (104).png" };
	picture_slide s105{ show::presentation::SLIDE_105, pres::res + "Geometry St Petersburg (105).png" };
	picture_slide s106{ show::presentation::SLIDE_106, pres::res + "Geometry St Petersburg (106).png" };
	picture_slide s107{ show::presentation::SLIDE_107, pres::res + "Geometry St Petersburg (107).png" };
	picture_slide s108{ show::presentation::SLIDE_108, pres::res + "Geometry St Petersburg (108).png" };
	picture_slide s109{ show::presentation::SLIDE_109, pres::res + "Geometry St Petersburg (109).png" };
	picture_slide s110{ show::presentation::SLIDE_110, pres::res + "Geometry St Petersburg (110).png" };
	picture_slide s111{ show::presentation::SLIDE_111, pres::res + "Geometry St Petersburg (111).png" };
	picture_slide s112{ show::presentation::SLIDE_112, pres::res + "Geometry St Petersburg (112).png" };
	picture_slide s113{ show::presentation::SLIDE_113, pres::res + "Geometry St Petersburg (113).png" };
	picture_slide s114{ show::presentation::SLIDE_114, pres::res + "Geometry St Petersburg (114).png" };
	picture_slide s115{ show::presentation::SLIDE_115, pres::res + "Geometry St Petersburg (115).png" };
	picture_slide s116{ show::presentation::SLIDE_116, pres::res + "Geometry St Petersburg (116).png" };
	picture_slide s117{ show::presentation::SLIDE_117, pres::res + "Geometry St Petersburg (117).png" };
	picture_slide s118{ show::presentation::SLIDE_118, pres::res + "Geometry St Petersburg (118).png" };
	picture_slide s119{ show::presentation::SLIDE_119, pres::res + "Geometry St Petersburg (119).png" };
	picture_slide s120{ show::presentation::SLIDE_120, pres::res + "Geometry St Petersburg (120).png" };
	picture_slide s121{ show::presentation::SLIDE_121, pres::res + "Geometry St Petersburg (121).png" };
	picture_slide s122{ show::presentation::SLIDE_122, pres::res + "Geometry St Petersburg (122).png" };
}
