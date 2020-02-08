#pragma once
#include <iosfwd>

using uint = unsigned;
template<class T, uint...>	struct base;
template<class T, uint...>	struct swizz;
template<class T, uint>		struct vec;

template<class T>
struct base<T> {};
template<class T, uint...x>
struct alignas(sizeof(T)) base
{
	operator vec<T, sizeof...(x)>() const;
	swizz<T, x...>& operator = (const base & v);
	template<uint ...y>
	swizz<T, x...>& operator =  (const base<T, y...> & v);
	swizz<T, x...>& operator =  (const T n);
	T& operator [](const uint n);
	const T& operator [](const uint n) const;
	static constexpr uint is[] = { x... };
};

template<template<class, uint...> class V, class T, uint h, uint... t>
struct type_proxy : type_proxy<V, T, h - 1, h - 1, t...> { };
template<template<class, uint...> class V, class T, uint...t>
struct type_proxy <V, T, 0, t...>
{
	using type = V<T, t...>;
	template<uint n> struct mul { using type = V<T, n * t...>; };
};

template<class T, uint x>
struct alignas(sizeof(T)) swizz<T, x>
{
	operator const T& () const;
	operator T& ();
	T& operator = (const T&);
};

template<class T, uint...x>
struct swizz : base<T, x...>
{
	using base<T, x...>::operator=;
	swizz<T, x...>& operator= (const swizz& v);
};

template<class T, uint a, uint b>
struct swizz<T, a, b> : base<T, a, b>
{
	using base<T, a, b>::operator=;
	swizz<T, a, b>& operator= (const swizz& v);
	union
	{
		swizz<T, a> x;
		swizz<T, b> y;
		base <T, a, b> xy;
		base <T, b, a> yx;
	};
};

template<class T, uint a, uint b, uint c>
struct swizz<T, a, b, c> : base<T, a, b, c>
{
	using base<T, a, b, c>::operator=;
	swizz<T, a, b, c>& operator= (const swizz& v);
	union
	{
		swizz<T, a> x;
		swizz<T, b> y;
		swizz<T, c> z;
		swizz<T, a, b> xy;
		swizz<T, a, c> xz;
		swizz<T, b, a> yx;
		swizz<T, b, c> yz;
		swizz<T, c, a> zx;
		swizz<T, c, b> zy;
		base <T, a, b, c> xyz;
		base <T, a, c, b> xzy;
		base <T, b, a, c> yxz;
		base <T, b, c, a> yzx;
		base <T, c, a, b> zxy;
		base <T, c, b, a> zyx;
	};
};

template<class T, uint a, uint b, uint c, uint d>
struct swizz<T, a, b, c, d> : base<T, a, b, c, d>
{
	using base<T, a, b, c, d>::operator=;
	swizz<T, a, b, c, d>& operator= (const swizz& v);
	union
	{
		swizz<T, a> x;
		swizz<T, b> y;
		swizz<T, c> z;
		swizz<T, d> w;
		swizz<T, a, b> xy;
		swizz<T, a, c> xz;
		swizz<T, a, d> xw;
		swizz<T, b, a> yx;
		swizz<T, b, c> yz;
		swizz<T, b, d> yw;
		swizz<T, c, a> zx;
		swizz<T, c, b> zy;
		swizz<T, c, d> zw;
		swizz<T, d, a> wx;
		swizz<T, d, b> wy;
		swizz<T, d, c> wz;
		swizz<T, a, b, c> xyz;
		swizz<T, a, b, d> xyw;
		swizz<T, a, c, b> xzy;
		swizz<T, a, c, d> xzw;
		swizz<T, a, d, b> xwy;
		swizz<T, a, d, c> xwz;
		swizz<T, b, a, c> yxz;
		swizz<T, b, a, d> yxw;
		swizz<T, b, c, a> yzx;
		swizz<T, b, c, d> yzw;
		swizz<T, b, d, a> ywx;
		swizz<T, b, d, c> ywz;
		swizz<T, c, a, b> zxy;
		swizz<T, c, a, d> zxw;
		swizz<T, c, b, a> zyx;
		swizz<T, c, b, d> zyw;
		swizz<T, c, d, a> zwx;
		swizz<T, c, d, b> zwy;
		swizz<T, d, a, b> wxy;
		swizz<T, d, a, c> wxz;
		swizz<T, d, b, a> wyx;
		swizz<T, d, b, c> wyz;
		swizz<T, d, c, a> wzx;
		swizz<T, d, c, b> wzy;
		base <T, a, b, c, d> xyzw;
		base <T, a, b, d, c> xywz;
		base <T, a, c, b, d> xzyw;
		base <T, a, c, d, b> xzwy;
		base <T, a, d, b, c> xwyz;
		base <T, a, d, c, b> xwzy;
		base <T, b, a, c, d> yxzw;
		base <T, b, a, d, c> yxwz;
		base <T, b, c, a, d> yzxw;
		base <T, b, c, d, a> yzwx;
		base <T, b, d, a, c> ywxz;
		base <T, b, d, c, a> ywzx;
		base <T, c, a, b, d> zxyw;
		base <T, c, a, d, b> zxwy;
		base <T, c, b, a, d> zyxw;
		base <T, c, b, d, a> zywx;
		base <T, c, d, a, b> zwxy;
		base <T, c, d, b, a> zwyx;
		base <T, d, a, b, c> wxyz;
		base <T, d, a, c, b> wxzy;
		base <T, d, b, a, c> wyxz;
		base <T, d, b, c, a> wyzx;
		base <T, d, c, a, b> wzxy;
		base <T, d, c, b, a> wzyx;
	};
};

template<class T, uint x>
struct vec : type_proxy<base, T, x>::type
{
	using base = typename type_proxy<base, T, x>::type;
	using base::operator=;
	union
	{
		T data[x];
		struct { T x, y, z, w; };
		swizz<T, 0, 1> xy;
		swizz<T, 0, 2> xz;
		swizz<T, 0, 3> xw;
		swizz<T, 1, 0> yx;
		swizz<T, 1, 2> yz;
		swizz<T, 1, 3> yw;
		swizz<T, 2, 0> zx;
		swizz<T, 2, 1> zy;
		swizz<T, 2, 3> zw;
		swizz<T, 3, 0> wx;
		swizz<T, 3, 1> wy;
		swizz<T, 3, 2> wz;
		swizz<T, 0, 1, 2> xyz;
		swizz<T, 0, 1, 3> xyw;
		swizz<T, 0, 2, 1> xzy;
		swizz<T, 0, 2, 3> xzw;
		swizz<T, 0, 3, 1> xwy;
		swizz<T, 0, 3, 2> xwz;
		swizz<T, 1, 0, 2> yxz;
		swizz<T, 1, 0, 3> yxw;
		swizz<T, 1, 2, 0> yzx;
		swizz<T, 1, 2, 3> yzw;
		swizz<T, 1, 3, 0> ywx;
		swizz<T, 1, 3, 2> ywz;
		swizz<T, 2, 0, 1> zxy;
		swizz<T, 2, 0, 3> zxw;
		swizz<T, 2, 1, 0> zyx;
		swizz<T, 2, 1, 3> zyw;
		swizz<T, 2, 3, 0> zwx;
		swizz<T, 2, 3, 1> zwy;
		swizz<T, 3, 0, 1> wxy;
		swizz<T, 3, 0, 2> wxz;
		swizz<T, 3, 1, 0> wyx;
		swizz<T, 3, 1, 2> wyz;
		swizz<T, 3, 2, 0> wzx;
		swizz<T, 3, 2, 1> wzy;
		swizz<T, 0, 1, 2, 3> xyzw;
		swizz<T, 0, 1, 3, 2> xywz;
		swizz<T, 0, 2, 1, 3> xzyw;
		swizz<T, 0, 2, 3, 1> xzwy;
		swizz<T, 0, 3, 1, 2> xwyz;
		swizz<T, 0, 3, 2, 1> xwzy;
		swizz<T, 1, 0, 2, 3> yxzw;
		swizz<T, 1, 0, 3, 2> yxwz;
		swizz<T, 1, 2, 0, 3> yzxw;
		swizz<T, 1, 2, 3, 0> yzwx;
		swizz<T, 1, 3, 0, 2> ywxz;
		swizz<T, 1, 3, 2, 0> ywzx;
		swizz<T, 2, 0, 1, 3> zxyw;
		swizz<T, 2, 0, 3, 1> zxwy;
		swizz<T, 2, 1, 0, 3> zyxw;
		swizz<T, 2, 1, 3, 0> zywx;
		swizz<T, 2, 3, 0, 1> zwxy;
		swizz<T, 2, 3, 1, 0> zwyx;
		swizz<T, 3, 0, 1, 2> wxyz;
		swizz<T, 3, 0, 2, 1> wxzy;
		swizz<T, 3, 1, 0, 2> wyxz;
		swizz<T, 3, 1, 2, 0> wyzx;
		swizz<T, 3, 2, 0, 1> wzxy;
		swizz<T, 3, 2, 1, 0> wzyx;
	};

	vec& operator =(const vec& v);
	vec();
	vec(T x);
	template<class...Ts>
	vec(T x, Ts... t);
};

template<class T>
struct vec<T, 2> : base<T, 0, 1> {
	using base<T, 0, 1>::operator=;
	union
	{
		T data[2];
		struct { T x, y; };
		swizz<T, 0, 1> xy;
		swizz<T, 1, 0> yx;
	};
	vec& operator =(const vec&);
	vec();
	vec(T x);
	vec(T x, T y);
	template< uint a, uint b>
	vec(const base<T, a, b>& v);
};

template<class T>
struct vec<T, 3> : base<T, 0, 1, 2> {
	using base<T, 0, 1, 2>::operator=;
	union
	{
		T data[3];
		struct { T x, y, z; };
		swizz<T, 0, 1> xy;
		swizz<T, 0, 2> xz;
		swizz<T, 1, 0> yx;
		swizz<T, 1, 2> yz;
		swizz<T, 2, 0> zx;
		swizz<T, 2, 1> zy;
		swizz<T, 0, 1, 2> xyz;
		swizz<T, 0, 2, 1> xzy;
		swizz<T, 1, 0, 2> yxz;
		swizz<T, 1, 2, 0> yzx;
		swizz<T, 2, 0, 1> zxy;
		swizz<T, 2, 1, 0> zyx;
	};
	vec& operator =(const vec&);
	vec();
	vec(T x);
	vec(T x, T y, T z);

	template<uint a, uint b, uint c>
	vec(const base<T, a, b, c>& v);

	template<uint a, uint b>
	vec(T x, const base<T, a, b>& v);

	template<uint a, uint b>
	vec(const base<T, a, b>& v, T z);
};

template<class T>
struct vec<T, 4> : base<T, 0, 1, 2, 3>
{
	using base<T, 0, 1, 2, 3>::operator=;
	union
	{
		T data[4];
		struct { T x, y, z, w; };
		swizz<T, 0, 1> xy;
		swizz<T, 0, 2> xz;
		swizz<T, 0, 3> xw;
		swizz<T, 1, 0> yx;
		swizz<T, 1, 2> yz;
		swizz<T, 1, 3> yw;
		swizz<T, 2, 0> zx;
		swizz<T, 2, 1> zy;
		swizz<T, 2, 3> zw;
		swizz<T, 3, 0> wx;
		swizz<T, 3, 1> wy;
		swizz<T, 3, 2> wz;
		swizz<T, 0, 1, 2> xyz;
		swizz<T, 0, 1, 3> xyw;
		swizz<T, 0, 2, 1> xzy;
		swizz<T, 0, 2, 3> xzw;
		swizz<T, 0, 3, 1> xwy;
		swizz<T, 0, 3, 2> xwz;
		swizz<T, 1, 0, 2> yxz;
		swizz<T, 1, 0, 3> yxw;
		swizz<T, 1, 2, 0> yzx;
		swizz<T, 1, 2, 3> yzw;
		swizz<T, 1, 3, 0> ywx;
		swizz<T, 1, 3, 2> ywz;
		swizz<T, 2, 0, 1> zxy;
		swizz<T, 2, 0, 3> zxw;
		swizz<T, 2, 1, 0> zyx;
		swizz<T, 2, 1, 3> zyw;
		swizz<T, 2, 3, 0> zwx;
		swizz<T, 2, 3, 1> zwy;
		swizz<T, 3, 0, 1> wxy;
		swizz<T, 3, 0, 2> wxz;
		swizz<T, 3, 1, 0> wyx;
		swizz<T, 3, 1, 2> wyz;
		swizz<T, 3, 2, 0> wzx;
		swizz<T, 3, 2, 1> wzy;
		swizz<T, 0, 1, 2, 3> xyzw;
		swizz<T, 0, 1, 3, 2> xywz;
		swizz<T, 0, 2, 1, 3> xzyw;
		swizz<T, 0, 2, 3, 1> xzwy;
		swizz<T, 0, 3, 1, 2> xwyz;
		swizz<T, 0, 3, 2, 1> xwzy;
		swizz<T, 1, 0, 2, 3> yxzw;
		swizz<T, 1, 0, 3, 2> yxwz;
		swizz<T, 1, 2, 0, 3> yzxw;
		swizz<T, 1, 2, 3, 0> yzwx;
		swizz<T, 1, 3, 0, 2> ywxz;
		swizz<T, 1, 3, 2, 0> ywzx;
		swizz<T, 2, 0, 1, 3> zxyw;
		swizz<T, 2, 0, 3, 1> zxwy;
		swizz<T, 2, 1, 0, 3> zyxw;
		swizz<T, 2, 1, 3, 0> zywx;
		swizz<T, 2, 3, 0, 1> zwxy;
		swizz<T, 2, 3, 1, 0> zwyx;
		swizz<T, 3, 0, 1, 2> wxyz;
		swizz<T, 3, 0, 2, 1> wxzy;
		swizz<T, 3, 1, 0, 2> wyxz;
		swizz<T, 3, 1, 2, 0> wyzx;
		swizz<T, 3, 2, 0, 1> wzxy;
		swizz<T, 3, 2, 1, 0> wzyx;
	};
	vec& operator =(const vec&);
	vec();
	vec(T x);
	vec(T x, T y, T z, T w);

	template<uint a, uint b, uint c, uint d>
	vec(const base<T, a, b, c, d>&);

	template<uint a, uint b, uint c>
	vec(const base<T, a, b, c>&, T w);

	template<uint b, uint c, uint d>
	vec(T x, const base<T, b, c, d>&);

	template<uint a, uint b>
	vec(const base<T, a, b>&, T z, T w);

	template<uint b, uint c>
	vec(T x, const base<T, b, c>&, T w);

	template<uint c, uint d>
	vec(T x, T y, const base<T, c, d>&);
};


typedef vec<float, 2> vec2;
typedef vec<float, 3> vec3;
typedef vec<float, 4> vec4;
typedef vec<int, 2> vec2i;
typedef vec<int, 3> vec3i;
typedef vec<int, 4> vec4i;
typedef vec<uint, 2> vec2u;
typedef vec<uint, 3> vec3u;
typedef vec<uint, 4> vec4u;
typedef vec<short, 2> vec2s;
typedef vec<short, 3> vec3s;
typedef vec<short, 4> vec4s;

template<class T, uint n>
using cvec = typename type_proxy<swizz, T, n>::template mul<n>::type;
template<class T, uint n>
using dvec = typename type_proxy<swizz, T, n>::template mul<n + 1>::type;

typedef cvec<float, 2> cvec2;
typedef cvec<float, 3> cvec3;
typedef cvec<float, 4> cvec4;
typedef dvec<float, 2> dvec2;
typedef dvec<float, 3> dvec3;
typedef dvec<float, 4> dvec4;

template<class T, uint...x>
inline base<T, x...>::operator vec<T, sizeof...(x)>() const
{
	return { *(T*)(this + x)... };
}

template<class T, uint...x>
inline swizz<T, x...>& base<T, x...>::operator = (const base& v)
{
	((*(T*)(this + x) = *(T*)(&v + x)), ...);
	return *(swizz<T, x...>*)this;
}

template<class T, uint...x>
template<uint...y>
inline swizz<T, x...>& base<T, x...>::operator =  (const base<T, y...>& v)
{
	((*(T*)(this + x) = *(T*)(&v + y)), ...);
	return *(swizz<T, x...>*)this;
}

template<class T, uint...x>
inline swizz<T, x...>& base<T, x...>::operator =  (const T n)
{
	((*(T*)(this + x) = n), ...);
	return *(swizz<T, x...>*)this;
}

template<class T, uint...x>
inline T& base<T, x...>::operator [](const uint n)
{
	return *(T*)(this + is[n]);
}

template<class T, uint...x>
inline const T& base<T, x...>::operator [](const uint n) const
{
	return *(const T*)(this + is[n]);
}

#include "vecs.inl"
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////UNARY//////////////////////////////////////////////////
template<class T, uint...x>
inline vec<T, sizeof...(x)> operator -(base<T, x...>& v)
{
	return { -*(T*)(&v + x)... };
}
template<class T, uint...x>
inline swizz<T, x...>& operator ~(base<T, x...>& v)
{
	T len = 1 / ((*(T*)(&v + x) * *(T*)(&v + x)) + ...);
	((*(T*)(&v + x) *= len), ...);
	return *(swizz<T, x...>*) & v;
}
template<class T, uint...x>
inline vec<T, sizeof...(x)> norm(const base<T, x...>& v)
{
	T len = 1 / ((*(T*)(&v + x) * *(T*)(&v + x)) + ...);
	return { *(T*)(&v + x) * len... };
}
template<class T, uint...x>
inline T operator !(const base<T, x...>& v)
{
	return 	((*(T*)(&v + x) * *(T*)(&v + x)) + ...);
}
template<class T, uint...x>
inline T len(const base<T, x...>& v)
{
	return sqrtf(((*(T*)(&v + x) * *(T*)(&v + x)) + ...));
}
//////////////////////////////////////////////////UNARY//////////////////////////////////////////////////
/////////////////////////////////////////////////BINARY/////////////////////////////////////////////////
template<class T, uint ...x, uint ...y>
inline swizz<T, x...>& operator %(const base<T, x...>& l, const base<T, y...>& r)
{
	return ~(l - r);
}
template<class T, uint ...x, uint ...y>
inline T operator ^ (const base<T, x...>& l, const base<T, y...>& r)
{
	return dot(l, r) / (len(l) * len(r));
}
template<class T, uint ...x, uint ...y>
inline T dot(const base<T, x...>& l, const base<T, y...>& r)
{
	return ((*(T*)(&l + x) * *(T*)(&r + y)) + ...);
}
template<class T, uint x, uint y, uint z, uint x1, uint y1, uint z1>
inline vec<T, 3> cross(const base<T, x, y, z>& l, const base<T, x1, y1, z1>& r)
{
	return
	{
		*(T*)(&l + y) * *(T*)(&r + z1) - *(T*)(&l + z) * *(T*)(&r + y1),
		*(T*)(&l + z) * *(T*)(&r + x1) - *(T*)(&l + x) * *(T*)(&r + z1),
		*(T*)(&l + x) * *(T*)(&r + y1) - *(T*)(&l + y) * *(T*)(&r + x1)
	};
}
/////////////////////////////////////////////////BINARY/////////////////////////////////////////////////
///////////////////////////////////////////////ARITHMETIC///////////////////////////////////////////////
template<class T, uint ...x, uint ...y>
inline swizz<T, x...>& operator+=(base<T, x...>& l, const base<T, y...>& r)
{
	((*(T*)(&l + x) += *(T*)(&r + y)), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x, uint ...y>
inline vec<T, sizeof...(x)> operator+(const base<T, x...>& l, const base<T, y...>& r) 
{
	return { *(T*)(&l + x) + *(T*)(&r + y)... };
}
template<class T, uint ...x>
inline swizz<T, x...>& operator+=(base<T, x...>& l, const T r)
{
	((*(T*)(&l + x) += r), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator+(const base<T, x...>& l, const T r)
{
	return { *(T*)(&l + x) + r... };
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator+(const T l, const base<T, x...>& r)
{
	return {  l + *(T*)(&r + x)... };
}
template<class T, uint ...x, uint ...y>
inline swizz<T, x...>& operator-=(base<T, x...>& l, const base<T, y...>& r)
{
	((*(T*)(&l + x) -= *(T*)(&r + y)), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x, uint ...y>
inline vec<T, sizeof...(x)> operator-(const base<T, x...>& l, const base<T, y...>& r)
{
	return { *(T*)(&l + x) - *(T*)(&r + y)... };
}
template<class T, uint ...x>
inline swizz<T, x...>& operator-=(base<T, x...>& l, const T r)
{
	((*(T*)(&l + x) -= r), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator-(const base<T, x...>& l, const T r)
{
	return { *(T*)(&l + x) - r... };
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator-(const T l, const base<T, x...>& r)
{
	return { l - *(T*)(&r + x)... };
}
template<class T, uint ...x, uint ...y>
inline swizz<T, x...>& operator*=(base<T, x...>& l, const base<T, y...>& r)
{
	((*(T*)(&l + x) *= *(T*)(&r + y)), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x, uint ...y>
inline vec<T, sizeof...(x)> operator*(const base<T, x...>& l, const base<T, y...>& r)
{
	return { *(T*)(&l + x) * *(T*)(&r + y)... };
}
template<class T, uint ...x>
inline swizz<T, x...>& operator*=(base<T, x...>& l, const T r)
{
	((*(T*)(&l + x) *= r), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator*(const base<T, x...>& l, const T r)
{
	return { *(T*)(&l + x) * r... };
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator*(const T l, const base<T, x...>& r)
{
	return { l * *(T*)(&r + x)... };
}
template<class T, uint ...x, uint ...y>
inline swizz<T, x...>& operator/=(base<T, x...>& l, const base<T, y...>& r)
{
	((*(T*)(&l + x) /= *(T*)(&r + y)), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x, uint ...y>
inline vec<T, sizeof...(x)> operator/(const base<T, x...>& l, const base<T, y...>& r)
{
	return { *(T*)(&l + x) / *(T*)(&r + y)... };
}
template<class T, uint ...x>
inline swizz<T, x...>& operator/=(base<T, x...>& l, T r)
{
	r = 1 / r;
	((*(T*)(&l + x) *= r), ...);
	return (swizz<T, x...>&)l;
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator/(const base<T, x...>& l, T r)
{
	r = 1 / r;
	return { *(T*)(&l + x) * r... };
}
template<class T, uint ...x>
inline vec<T, sizeof...(x)> operator/(const T l, const base<T, x...>& r)
{
	return { l / *(T*)(&r + x)... };
}
///////////////////////////////////////////////ARITHMETIC///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


template<class T, uint...x>
inline std::ostream& operator <<(std::ostream& stream, const base<T, x...>& v)
{
	((stream << *(T*)(&v + x) << ' '), ...);
	return stream << '\n';
}
