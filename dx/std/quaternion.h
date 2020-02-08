#pragma once
#include "mat.h"

template<class T>
struct quaternion
{
	union
	{
		T data[4];
		struct { T i, j, k, s; };
		vec<T, 3> v;
		vec<T, 4> q;
	};

	quaternion& operator= (const quaternion& q);
	quaternion();
	quaternion(T x, T y, T z, T w);
	template<uint a, uint b, uint c>
	quaternion(const base<T, a, b, c>& v, T w);
	quaternion(const mat<T, 3>& m);
	quaternion(const mat<T, 4>& m);

	explicit operator mat<T, 4>() const;
	explicit operator mat<T, 3>() const;
};

typedef quaternion<float> quat;

template<class T, uint a, uint b, uint c>
inline vec<T, 3> operator *(const base<T, a, b, c>& v, const quaternion<T>& q)
{
	T _x = *(T*)(&v + b) * q.k - *(T*)(&v + c) * q.j + *(T*)(&v + a) * q.s;
	T _y = *(T*)(&v + c) * q.i - *(T*)(&v + a) * q.k + *(T*)(&v + b) * q.s;
	T _z = *(T*)(&v + a) * q.j - *(T*)(&v + b) * q.i + *(T*)(&v + c) * q.s;
	T _w = *(T*)(&v + a) * q.i + *(T*)(&v + b) * q.j + *(T*)(&v + c) * q.k;
	return
	{
		_y * q.k - _z * q.j + _x * q.s + q.i * _w,
		_z * q.i - _x * q.k + _y * q.s + q.j * _w,
		_x * q.j - _y * q.i + _z * q.s + q.k * _w
	};
}

template<class T,uint a, uint b, uint c>
inline vec<T, 3> operator *(const quaternion<T>& q, const base<T, a, b, c>& v)
{
	T _x = q.j * *(T*)(&v + c) - q.k * *(T*)(&v + b) + q.s * *(T*)(&v + a);
	T _y = q.k * *(T*)(&v + a) - q.i * *(T*)(&v + c) + q.s * *(T*)(&v + b);
	T _z = q.i * *(T*)(&v + b) - q.j * *(T*)(&v + a) + q.s * *(T*)(&v + c);
	T _w = q.i * *(T*)(&v + a) + q.j * *(T*)(&v + b) + q.k * *(T*)(&v + c);
	return
	{
		q.j* _z - q.k * _y + q.s * _x + q.i * _w,
		q.k* _x - q.i * _z + q.s * _y + q.j * _w,
		q.i* _y - q.j * _x + q.s * _z + q.k * _w
	};
}

template<class T>
inline quaternion<T>& quaternion<T>::operator=(const quaternion& q)
{
	this->q = q.q;
	return *this;
}

template<class T>
inline quaternion<T>::quaternion() :
	data{ 0, 0, 0, 1 }
{
}

template<class T>
inline quaternion<T>::quaternion(T x, T y, T z, T w) :
	data{ x, y, z, w }
{
}

template<class T>
template<uint a, uint b, uint c>
inline quaternion<T>::quaternion(const base<T, a, b, c>& v, T w) :
	data{ *(T*)(&v + a), *(T*)(&v + b), *(T*)(&v + c), w }
{}

template<class T>
inline quaternion<T>::quaternion(const mat<T, 3>& m)
{
	T tr = m.data[0][0] + m.data[1][1] + m.data[2][2];
	if (tr > 0.f) {
		T S = 0.5f / sqrt(tr + 1.f);
		s = 0.25f / S;
		i = (m.data[2][1] - m.data[1][2]) * S;
		j = (m.data[0][2] - m.data[2][0]) * S;
		k = (m.data[1][0] - m.data[0][1]) * S;
	}
	else if (m.data[0][0] > m.data[1][1] && m.data[0][0] > m.data[2][2]) {
		T S = 0.5f / sqrt(1.f + m.data[0][0] - m.data[1][1] - m.data[2][2]);
		s = (m.data[2][1] - m.data[1][2]) * S;
		i = 0.25f / S;
		j = (m.data[0][1] + m.data[1][0]) * S;
		k = (m.data[0][2] + m.data[2][0]) * S;
	}
	else if (m.data[1][1] > m.data[2][2]) {
		T S = 0.5f / sqrt(1.f + m.data[1][1] - m.data[0][0] - m.data[2][2]);
		s = (m.data[0][2] - m.data[2][0]) * S;
		i = (m.data[0][1] + m.data[1][0]) * S;
		j = 0.25f / S;
		k = (m.data[1][2] + m.data[2][1]) * S;
	}
	else {
		T S = 0.5f / sqrt(1.f + m.data[2][2] - m.data[0][0] - m.data[1][1]);
		s = (m.data[1][0] - m.data[0][1]) * S;
		i = (m.data[0][2] + m.data[2][0]) * S;
		j = (m.data[1][2] + m.data[2][1]) * S;
		k = 0.25f / S;
	}
}


template<class T>
inline quaternion<T>::quaternion(const mat<T, 4>& m) 
{
	T tr = m.data[0][0] + m.data[1][1] + m.data[2][2];
	if (tr > 0.f) 
	{
		T S = 0.5f / sqrtf(tr + 1.f);
		s = 0.25f / S;
		i = (m.data[2][1] - m.data[1][2]) * S;
		j = (m.data[0][2] - m.data[2][0]) * S;
		k = (m.data[1][0] - m.data[0][1]) * S;
	}
	else if (m.data[0][0] > m.data[1][1] && m.data[0][0] > m.data[2][2]) 
	{
		T S = 0.5f / sqrtf(1.f + m.data[0][0] - m.data[1][1] - m.data[2][2]);
		s = (m.data[2][1] - m.data[1][2]) * S;
		i = 0.25f / S;
		j = (m.data[0][1] + m.data[1][0]) * S;
		k = (m.data[0][2] + m.data[2][0]) * S;
	}
	else if (m.data[1][1] > m.data[2][2]) 
	{
		T S = 0.5f / sqrt(1.f + m.data[1][1] - m.data[0][0] - m.data[2][2]);
		s = (m.data[0][2] - m.data[2][0]) * S;
		i = (m.data[0][1] + m.data[1][0]) * S;
		j = 0.25f / S;
		k = (m.data[1][2] + m.data[2][1]) * S;
	}
	else 
	{
		T S = 0.5f / sqrtf(1.f + m.data[2][2] - m.data[0][0] - m.data[1][1]);
		s = (m.data[1][0] - m.data[0][1]) * S;
		i = (m.data[0][2] + m.data[2][0]) * S;
		j = (m.data[1][2] + m.data[2][1]) * S;
		k = 0.25f / S;
	}
}


template<class T>
inline quaternion<T>::operator mat<T, 3>() const
{
	T i2 = i + i,  j2 = j + j,  k2 = k + k;
	T ii = i * i2, ij = i * j2, ik = i * k2;
	T jj = j * j2, jk = j * k2, kk = k * k2;
	T si = s * i2, sj = s * j2, sk = s * k2;
	return
	{
		1 - (jj + kk), ij - sk, ik + sj,
		ij + sk, 1 - (ii + kk), jk - si,
		ik - sj, jk + si, 1 - (ii + jj)
	};
}

template<class T>
inline quaternion<T>::operator mat<T, 4>() const
{
	T i2 = i + i, j2 = j + j, k2 = k + k;
	T ii = i * i2, ij = i * j2, ik = i * k2;
	T jj = j * j2, jk = j * k2, kk = k * k2;
	T si = s * i2, sj = s * j2, sk = s * k2;
	return
	{
		1 - (jj + kk), ij - sk, ik + sj, 0,
		ij + sk, 1 - (ii + kk), jk - si, 0,
		ik - sj, jk + si, 1 - (ii + jj),
		0, 0, 0, 0, 1
	};
}

template<class T, int a, int b, int c>
inline mat<T, 4> compose(const quaternion<T>& q, const base<T, a, b, c>& v)
{
	T i2 = q.i + q.i, j2 = q.j + q.j, k2 = q.k + q.k;
	T ii = q.i * i2, ij = q.i * j2, ik = q.i * k2;
	T jj = q.j * j2, jk = q.j * k2, kk = q.k * k2;
	T si = q.s * i2, sj = q.s * j2, sk = q.s * k2;
	return
	{
		1 - (jj + kk), ij - sk, ik + sj, *(T*)(&v + a),
		ij + sk, 1 - (ii + kk), jk - si, *(T*)(&v + b),
		ik - sj, jk + si, 1 - (ii + jj), *(T*)(&v + c),
		0, 0, 0, 1
	};
}

template<class T>
inline T operator !(const quaternion<T>& q)
{
	return !q.q;
}

template<class T>
inline T len(const quaternion<T>& q)
{
	return len(q.q);
}

template<class T>
inline T dot(const quaternion<T>& l, const quaternion<T>& r)
{
	return dot(l.q, r.q);
}

template<class T>
inline quaternion<T> operator~(const quaternion<T>& q)
{
	return { -q.v, q.s };
}

template<class T>
inline quaternion<T> operator-(const quaternion<T>& q)
{
	return { -q.v, -q.s };
}

template<class T>
inline quaternion<T> operator*(const quaternion<T>& l, const quaternion<T>& r)
{
	return { cross(l.v, r.v) + l.v * r.s + r.v * l.s, l.s * r.s - dot(l.v, r.v)};
}

template<class T>
inline quaternion<T> operator+(const quaternion<T>& l, const quaternion<T>& r)
{
	return { l.v + r.v, l.s + r.s };
}

template<class T>
inline quaternion<T> operator-(const quaternion<T>& l, const quaternion<T>& r)
{
	return { l.v - r.v, l.s - r.s };
}

template<class T>
inline quaternion<T>& operator+=(quaternion<T>& l, const quaternion<T>& r)
{
	l.q += r.q;
	return l;
}

template<class T>
inline quaternion<T>& operator-=(quaternion<T>& l, const quaternion<T>& r)
{
	l.q -= r.q;
	return l;
}
template<class T>
inline quaternion<T> operator*(const T n, const quaternion<T>& q)
{
	return { q.v * n, q.s * n };
}

template<class T>
inline quaternion<T> operator*(const quaternion<T>& q, const T n)
{
	return { q.v * n, q.s * n };
}

template<class T>
inline quaternion<T> operator/(const quaternion<T>& q, T n)
{
	n = 1 / n;
	return { q.v * n, q.s * n };
}

template<class T>
inline quaternion<T>& operator*=(quaternion<T>& q, const T n)
{
	q.q *= n;
	return *this;
}

template<class T>
inline quaternion<T>& operator/=(quaternion<T>& q, const T n)
{
	q.q /= n;
	return *this;
}

inline quat axang(float x, float y, float z, float w)
{
	float r = sinf(w *= 0.5f) / sqrtf(x * x + y * y + z * z);
	return { x * r, y * r, z * r, cosf(w) };
}

inline quat axang(const vec3& v, float w)
{
	return { norm(v) * sinf(w * 0.5f), cosf(w * 0.5f) };
}

template<class T>
inline mat<T, 4> operator *(const mat<T, 4>& m, const quaternion<T>& q)
{
	return m * mat<T, 4>(q);
}

template<class T>
inline mat<T, 3> operator *(const mat<T, 3>& m, const quaternion<T>& q)
{
	return m * mat<T, 3>(q);
}