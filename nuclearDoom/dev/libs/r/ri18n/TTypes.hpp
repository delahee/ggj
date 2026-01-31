#pragma once

#include "str/Str.h"
#include "rd/BitArray.hpp"

namespace rd {
	namespace parse {
		class CondParser;
		struct CondContext;
	}
}

namespace ri18n {
	struct	AstNode;
	class	RandText;

	struct UISheetLine {
		Str			text;
		AstNode*	ast = 0;
		RandText*	gen = 0;

		void		im();
	};

	typedef std::function<Str(const Str&)> TextReplacer;
}