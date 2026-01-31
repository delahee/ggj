#pragma once

namespace rd {
	enum Dir {
		NONE = 0,

		UP = 1,
		DOWN = 2,
		LEFT = 4,
		RIGHT = 8,

		UP_LEFT = (1 | 4),
		UP_RIGHT = (1 | 8),

		DOWN_LEFT = (2 | 4),
		DOWN_RIGHT = (2 | 8),

		DIAG_TL = (UP | LEFT),
		DIAG_TR = (UP | RIGHT),
		DIAG_BR = (DOWN | RIGHT),
		DIAG_BL = (DOWN | LEFT),

		UP_DOWN = (1 | 2),//often aka vert center
		LEFT_RIGHT = (4 | 8),//often aka horiz center

		UP_DOWN_LEFT = (1 | 2 | 4),
		UP_DOWN_RIGHT = (1 | 2 | 8),
		UP_LEFT_RIGHT = (1 | 4 | 8),
		DOWN_LEFT_RIGHT = (2 | 4 | 8),

		TLR = (UP | LEFT |RIGHT),
		DLR = (DOWN | LEFT | RIGHT),

		TLDR = (1 | 2 | 4 | 8),
	};

	class DirLib {
	public:
		static const char*				dirToString(Dir dir);
		static bool						isOppositeDir(Dir a, Dir b);
		static std::vector<Dir>			straightDirs;
		static std::vector<Dir>			allDirs;

		static rd::Dir					invert(Dir dir);
		static vec2						followDir(const vec2& v, Dir dir);
		static vec2i					followDir(const vec2i& v, Dir dir);
		static vec3i					followDir(const vec3i& v, Dir dir);
	};
}

namespace std {
	inline std::string to_string(rd::Dir d) { return rd::DirLib::dirToString(d); };
}