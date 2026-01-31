#pragma once

namespace rd {
	namespace perf {
		struct TimeTracer {
			Str		msg;
			double	t=0;
					TimeTracer(const char * msg=0);
					~TimeTracer();
		};
	}
}