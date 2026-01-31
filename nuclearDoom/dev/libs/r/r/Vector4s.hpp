#pragma once

namespace r {
	struct Vector4s {
	public:
		int x = 0;
		int y = 0;
		int z = 0;
		int w = 0;

		explicit Vector4s(int _x = 0,int _y = 0,int _z = 0,int _w=0) {
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		};

		inline int*			ptr() { return &x; };
		inline const int*	ptr() const { return &x; };
		inline bool			isZero() const { return 0 == x == y == z == w; };

		bool operator==(const Vector4s& v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); };
		bool operator!=(const Vector4s& v) const { return !(*this == v); }
	};
	typedef Vector4s					vec4s;
}
 
namespace std {
	template<> struct hash<r::Vector4s> {
		inline std::size_t operator()(const r::Vector4s& v) const noexcept {
			return std::size_t(v.x) + (std::size_t(v.y) << 16) + (std::size_t(v.z) << 32) + (std::size_t(v.w) << 48);
		};
	};
}