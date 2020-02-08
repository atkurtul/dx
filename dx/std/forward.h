#pragma once

using uint = unsigned;
template<class, uint>	struct vec;
template<class, uint>	struct mat;
template<class>			struct quaternion;

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
typedef mat<float, 2> mat2;
typedef mat<float, 3> mat3;
typedef mat<float, 4> mat4;
typedef quaternion<float> quat;

class String;
template<class> class Array;
template<class> class List;
template<class> class Stack;
template<class> class Queue;
template<class, class> struct Pair;
template<class, class, template<class> class = Array> class Map;
template<class key, class T> using Dict = Map<key, T, List>;
template<class key, class T> using Gloss = Map<key, T, Stack>;
