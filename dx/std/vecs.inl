
template<class T, uint x>
inline swizz<T, x>::operator const T& () const
{
	return *(T*)(this + x);
}

template<class T, uint x>
inline swizz<T, x>::operator T& ()
{
	return *(T*)(this + x);
}

template<class T, uint x>
inline T& swizz<T, x>::operator=(const T& n)
{
	return *(T*)(this + x) = n;
}

template<class T, uint...x>
inline swizz<T, x...>& swizz<T, x...>::operator= (const swizz& v)
{
	((*(T*)(this + x) = (T*)(&v + x)), ...);
	return *this;
}

template<class T, uint a, uint b>
inline swizz<T, a, b>& swizz<T, a, b>::operator= (const swizz& v)
{
	*(T*)(this + a) = *(T*)(&v + a);
	*(T*)(this + b) = *(T*)(&v + b);
	return *this;
}

template<class T, uint a, uint b, uint c>
inline swizz<T, a, b, c>& swizz<T, a, b, c>::operator= (const swizz& v)
{
	*(T*)(this + a) = *(T*)(&v + a);
	*(T*)(this + b) = *(T*)(&v + b);
	*(T*)(this + c) = *(T*)(&v + c);
	return *this;
}

template<class T, uint a, uint b, uint c, uint d>
inline swizz<T, a, b, c, d>& swizz<T, a, b, c, d>::operator= (const swizz& v)
{
	*(T*)(this + a) = *(T*)(&v + a);
	*(T*)(this + b) = *(T*)(&v + b);
	*(T*)(this + c) = *(T*)(&v + c);
	*(T*)(this + d) = *(T*)(&v + d);
	return *this;
}

template<class T, uint x>
vec<T, x>& vec<T, x>::operator =(const vec& v)
{
	memcpy(data, v.data, x * sizeof(T));
	return *this;
}

template<class T, uint x>
vec<T, x>::vec() : 
	data{} 
{
}

template<class T, uint x>
vec<T, x>::vec(T x) 
{ 
	base::operator=(x); 
}

template<class T, uint x>
template<class...Ts>
vec<T, x>::vec(T x,Ts... t) : 
	data{ x, T(t)... } 
{
}

template<class T>
inline vec<T, 2>& vec<T, 2>::operator=(const vec& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<class T>
inline vec<T, 2>::vec() :
	data{ }
{
}

template<class T>
inline vec<T, 2>::vec(T x) :
	data{ x, x }
{
}

template<class T>
inline vec<T, 2>::vec(T x, T y) :
	data{ x, y }
{
}

template<class T>
template<uint a, uint b>
vec<T, 2>::vec(const base<T, a, b>& v) :
	data{ *(T*)(&v + a), *(T*)(&v + b) }
{
}

template<class T>
inline vec<T, 3>& vec<T, 3>::operator=(const vec& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<class T>
inline vec<T, 3>::vec() :
	data{ }
{
}

template<class T>
inline vec<T, 3>::vec(T x) :
	data{ x, x, x }
{
}

template<class T>
inline vec<T, 3>::vec(T x, T y, T z) :
	data{ x, y, z }
{
}

template<class T>
template<uint a, uint b, uint c>
vec<T, 3>::vec(const base<T, a, b, c>& v) :
	data{ (*(T*)(&v + a)), (*(T*)(&v + b)), (*(T*)(&v + c)) }
{
}

template<class T>
template<uint b, uint c>
vec<T, 3>::vec(T x, const base<T, b, c>& v) :
	data{ x, (*(T*)(&v + b)), (*(T*)(&v + c)) }
{
}

template<class T>
template<uint a, uint b>
vec<T, 3>::vec(const base<T, a, b>& v, T z) :
	data{ (*(T*)(&v + a)), (*(T*)(&v + b)), z }
{
}

template<class T>
inline vec<T, 4>& vec<T, 4>::operator=(const vec& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template<class T>
vec<T, 4>::vec() :
	data{}
{
}

template<class T>
inline vec<T, 4>::vec(T x) :
	data{ x, x, x, x }
{
}

template<class T>
vec<T, 4>::vec(T x, T y, T z, T w) :
	data{ x, y, z, w }
{
}

template<class T>
template<uint a, uint b, uint c, uint d>
vec<T, 4>::vec(const base<T, a, b, c, d>& v) :
	data{ *(T*)(&v + a), *(T*)(&v + b), *(T*)(&v + c), *(T*)(&v + d) }
{
}

template<class T>
template<uint a, uint b, uint c>
vec<T, 4>::vec(const base<T, a, b, c>& v, T w) :
	data{ *(T*)(&v + a), *(T*)(&v + b), *(T*)(&v + c), w }
{
}

template<class T>
template<uint b, uint c, uint d>
vec<T, 4>::vec(T x, const base<T, b, c, d>& v) :
	data{ x, *(T*)(&v + b), *(T*)(&v + c), *(T*)(&v + d) }
{
}

template<class T>
template<uint a, uint b>
vec<T, 4>::vec(const base<T, a, b>& v, T z, T w) :
	data{ *(T*)(&v + a), *(T*)(&v + b), z, w }
{
}

template<class T>
template<uint b, uint c>
vec<T, 4>::vec(T x, const base<T, b, c>& v, T w) :
	data{ x, *(T*)(&v + b), *(T*)(&v + c), w }
{
}

template<class T>
template<uint c, uint d>
vec<T, 4>::vec(T x, T y, const base<T, c, d>& v) :
	data{ x, y, *(T*)(&v + c), *(T*)(&v + d) }
{
}