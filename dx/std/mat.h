#pragma once
#include "vec.h"

template<class T, uint x>
struct mat
{
	union
	{
		T data[x][x];
		vec<T, x> row[x];
		cvec<T, x> col[x];
		dvec<T, x> diag;
	};

	mat();
	mat(T n) : data{}
	{	
		for (uint i = 0; i < x; ++i)
			data[i][i] = n;
		data[x - 1][x -1] = 1.f;
	}
	template<class...Ts>
	mat(T h, Ts ...t);
	template<uint...a, uint...b, class...Ts>
	mat(const base<T, a...>& h, const base<T, b...>& h2, const Ts& ...t);
	mat& operator=(const mat& m);
};

typedef mat<float, 2> mat2;
typedef mat<float, 3> mat3;
typedef mat<float, 4> mat4;


template<class T, uint x>
inline mat<T, x>::mat() :
	data{} 
{
	diag = 1;
}

template<class T, uint x>
template<class...Ts>
inline mat<T, x>::mat(T h, Ts...t) :
	data{ h, T(t)... }
{
}

template<class T, uint x>
template<uint...a, uint...b, class...Ts>
inline mat<T, x>::mat(const base<T, a...>& h, const base<T, b...>& h2, const Ts& ...t) :
	row{ h, h2, vec<T,  x>(t)... }
{
}

template<class T, uint x>
inline mat<T, x>& mat<T, x>::operator = (const mat& m)
{
	memcpy(data, m.data, x * x * sizeof(T));
	return *this;
}

template<class T, uint...x>
inline vec<T, sizeof...(x)> operator * (const mat<T, sizeof...(x)>& m, const base<T, x...>& v)
{
	vec<T, sizeof...(x)> re;
	for (uint i = 0; i < sizeof...(x); ++i)
		re[i] = dot(m.row[i], v);
	return re;
}

template<class T, uint...x>
inline vec<T, sizeof...(x)> operator * (const base<T, x...>& v, const mat<T, sizeof...(x)>& m)
{
	vec<T, sizeof...(x)> re;
	for (uint i = 0; i < sizeof...(x); ++i)
		re[i] = dot(m.col[i], v);
	return re;
}

template<class T, uint...x>
inline vec<T, sizeof...(x)> operator * (const mat<T, sizeof...(x) + 1>& m, const base<T, x...>& v)
{
	using row = vec<T, sizeof...(x)>;
	vec<T, sizeof...(x)> re;
	for (uint i = 0; i < sizeof...(x); ++i)
		re[i] = dot((row&)m.row[i], v);
	return re;
}

template<class T, uint...x>
inline vec<T, sizeof...(x)> operator * (const base<T, x...>& v, const mat<T, sizeof...(x) + 1>& m)
{
	using col = dvec<T, sizeof...(x)>;
	vec<T, sizeof...(x)> re;
	for (uint i = 0; i < sizeof...(x); ++i)
		re[i] = dot((col&)m.col[i], v);
	return re;
}

template<class T, uint x>
inline mat<T, x> operator * (const mat<T, x>& l, const mat<T, x>& r)
{
	T re[x][x];
	for (uint i = 0; i < x; ++i)
		for (uint j = 0; j < x; ++j)
			re[i][j] = dot(l.row[i], r.col[j]);
	return (mat<T, x>&)re;
}

template<class T, uint x>
inline mat<T, x> operator * (const mat<T, x>& l, const mat<T, x + 1>& r)
{
	T re[x][x];
	for (uint i = 0; i < x; ++i)
		for (uint j = 0; j < x; ++j)
			re[i][j] = dot(l.row[i], (dvec<T, x>&)r.col[j]);
	return (mat<T, x>&)re;
}

template<class T, uint x>
inline mat<T, x> operator * (const mat<T, x + 1>& l, const mat<T, x>& r)
{
	T re[x][x];
	for (uint i = 0; i < x; ++i)
		for (uint j = 0; j < x; ++j)
			re[i][j] = dot((vec<T, x>&)l.row[i], r.col[j]);
	return (mat<T, x>&)re;
}


template<class T>
inline T det(const mat<T, 3>& m)
{
	return dot(m.row[0], cross(m.row[1], m.row[2]));
}

template<class T>
inline T det(const mat<T, 4>& m)
{
	return
		m.data[0][0] * dot(m.row[1].yzw, cross(m.row[2].yzw, m.row[3].yzw)) -
		m.data[0][1] * dot(m.row[1].xzw, cross(m.row[2].xzw, m.row[3].xzw)) +
		m.data[0][2] * dot(m.row[1].xyw, cross(m.row[2].xyw, m.row[3].xyw)) -
		m.data[0][3] * dot(m.row[1].xyz, cross(m.row[2].xyz, m.row[3].xyz));
}

template<class T>
inline mat<T, 3> inverse(const mat<T, 3>& m)
{
	T a = m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1];
	T b = m.data[1][2] * m.data[2][0] - m.data[1][0] * m.data[2][2];
	T c = m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0];
	T det = 1 / (m.data[0][0] * a + m.data[0][1] * b + m.data[0][2] * c);
	return
	{
		det * a, det * (m.data[0][2] * m.data[2][1] - m.data[0][1] * m.data[2][2]), det * (m.data[0][1] * m.data[1][2] - m.data[0][2] * m.data[1][1]),
		det * b, det * (m.data[0][0] * m.data[2][2] - m.data[0][2] * m.data[2][0]), det * (m.data[0][2] * m.data[1][0] - m.data[0][0] * m.data[1][2]),
		det * c, det * (m.data[0][1] * m.data[2][0] - m.data[0][0] * m.data[2][1]), det * (m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0])
	};
}

template<class T>
inline mat<T, 4> inverse(const mat<T, 4>& m)
{
	T d01_01 = m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
	T d01_02 = m.data[0][0] * m.data[1][2] - m.data[0][2] * m.data[1][0];
	T d01_03 = m.data[0][0] * m.data[1][3] - m.data[0][3] * m.data[1][0];
	T d01_12 = m.data[0][1] * m.data[1][2] - m.data[0][2] * m.data[1][1];
	T d01_13 = m.data[0][1] * m.data[1][3] - m.data[0][3] * m.data[1][1];
	T d01_23 = m.data[0][2] * m.data[1][3] - m.data[0][3] * m.data[1][2];
	T d03_01 = m.data[0][0] * m.data[3][1] - m.data[0][1] * m.data[3][0];
	T d03_02 = m.data[0][0] * m.data[3][2] - m.data[0][2] * m.data[3][0];
	T d03_03 = m.data[0][0] * m.data[3][3] - m.data[0][3] * m.data[3][0];
	T d03_12 = m.data[0][1] * m.data[3][2] - m.data[0][2] * m.data[3][1];
	T d03_13 = m.data[0][1] * m.data[3][3] - m.data[0][3] * m.data[3][1];
	T d03_23 = m.data[0][2] * m.data[3][3] - m.data[0][3] * m.data[3][2];
	T d13_01 = m.data[1][0] * m.data[3][1] - m.data[1][1] * m.data[3][0];
	T d13_02 = m.data[1][0] * m.data[3][2] - m.data[1][2] * m.data[3][0];
	T d13_03 = m.data[1][0] * m.data[3][3] - m.data[1][3] * m.data[3][0];
	T d13_12 = m.data[1][1] * m.data[3][2] - m.data[1][2] * m.data[3][1];
	T d13_13 = m.data[1][1] * m.data[3][3] - m.data[1][3] * m.data[3][1];
	T d13_23 = m.data[1][2] * m.data[3][3] - m.data[1][3] * m.data[3][2];
	T d201_012 = m.data[2][0] * d01_12 - m.data[2][1] * d01_02 + m.data[2][2] * d01_01;
	T d201_013 = m.data[2][0] * d01_13 - m.data[2][1] * d01_03 + m.data[2][3] * d01_01;
	T d201_023 = m.data[2][0] * d01_23 - m.data[2][2] * d01_03 + m.data[2][3] * d01_02;
	T d201_123 = m.data[2][1] * d01_23 - m.data[2][2] * d01_13 + m.data[2][3] * d01_12;
	T d203_012 = m.data[2][0] * d03_12 - m.data[2][1] * d03_02 + m.data[2][2] * d03_01;
	T d203_013 = m.data[2][0] * d03_13 - m.data[2][1] * d03_03 + m.data[2][3] * d03_01;
	T d203_023 = m.data[2][0] * d03_23 - m.data[2][2] * d03_03 + m.data[2][3] * d03_02;
	T d203_123 = m.data[2][1] * d03_23 - m.data[2][2] * d03_13 + m.data[2][3] * d03_12;
	T d213_012 = m.data[2][0] * d13_12 - m.data[2][1] * d13_02 + m.data[2][2] * d13_01;
	T d213_013 = m.data[2][0] * d13_13 - m.data[2][1] * d13_03 + m.data[2][3] * d13_01;
	T d213_023 = m.data[2][0] * d13_23 - m.data[2][2] * d13_03 + m.data[2][3] * d13_02;
	T d213_123 = m.data[2][1] * d13_23 - m.data[2][2] * d13_13 + m.data[2][3] * d13_12;
	T d301_012 = m.data[3][0] * d01_12 - m.data[3][1] * d01_02 + m.data[3][2] * d01_01;
	T d301_013 = m.data[3][0] * d01_13 - m.data[3][1] * d01_03 + m.data[3][3] * d01_01;
	T d301_023 = m.data[3][0] * d01_23 - m.data[3][2] * d01_03 + m.data[3][3] * d01_02;
	T d301_123 = m.data[3][1] * d01_23 - m.data[3][2] * d01_13 + m.data[3][3] * d01_12;
	T idet = 1 /(-d201_123 * m.data[3][0] + d201_023 * m.data[3][1] - d201_013 * m.data[3][2] + d201_012 * m.data[3][3]);
	return 
	{
		 -d213_123 * idet,
		  d203_123 * idet,
		  d301_123 * idet,
		 -d201_123 * idet,
		  d213_023 * idet,
		 -d203_023 * idet,
		 -d301_023 * idet,
		  d201_023 * idet,
		 -d213_013 * idet,
		  d203_013 * idet,
		  d301_013 * idet,
		 -d201_013 * idet,
		  d213_012 * idet,
		 -d203_012 * idet, 	 
		 -d301_012 * idet, 
		  d201_012 * idet
	};
}

template<class T, uint x>
mat<T, x> transpose(const mat<T, x>& m)
{
	T re[x][x];
	for (uint i = 0; i < x; ++i)
		(vec<T, x>&)re[i] = m.col[i];
	return (mat<T, x>&)re;
}

template<class T, uint x>
inline std::ostream& operator <<(std::ostream& stream, const mat<T, x>& m)
{
	for (auto& v : m.row) stream << v;
	return stream;
}

inline mat4 rot(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	return
	{
		c,  -s,   0.f,  0.f,
		s,   c,   0.f,  0.f,
		0.f, 0.f, 1.f,  0.f,
		0.f, 0.f, 0.f,  1.f,
	};
}

inline mat3 rotx(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	return
	{
		1.f, 0.f, 0.f,
		0.f,  c,  -s,
		0.f,  s,   c
	};
}
inline mat3 roty(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	return
	{
		c,		0.f,	 s,
		0.f,	1.f,	0.f,
		-s,		0.f,	 c
	};
}

inline mat4 perspective(float fov, float aspect, float n, float f) {
	float tan = 1.f / float(tanf(fov * 0.00872664625f));
	float fn = 1.f / (f - n);
	return
	{
		tan * aspect,	0,			0,			0,
			0,		tan,			0,			0,
			0,		0,		fn * (f + n),	 -2.f * f * n * fn,
			0,		0,				1,			0
	};
}

inline mat4 ortho(float l, float r, float b, float t) {
	float rl = 1 / (r - l);
	float tb = 1 / (t - b);
	return
	{
		2 * rl,			0,			 0,		-rl * (r + l),
			 0,	   2 * tb,			 0,		-tb * (t + b),
			 0,			0,			 1,		 0,
			 0,			0,			 0,		 1
	};
}