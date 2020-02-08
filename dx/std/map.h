#pragma once
#include "list.h"


template<class key, class T>
struct Pair
{
    key Key;
    T Value;
    Pair() : Key(), Value() {}
    Pair(const key& k) : Key(k) {}
    Pair(const key& k, const T& v) : Key(k), Value(v) {}
};

template<class key, class T, template<class> class con = Array>
class Map
{
    con<Pair<key, T>> dat;
public:

    Map();
    Map(const Map& m);
    Map(Map&& m);
    Map<key, T, con>& operator=(const Map& m);
    Map<key, T, con>& operator=(Map&& m);

    unsigned size() const;
    unsigned capacity() const;
    void shrink();
    void reserve(unsigned n);

    void insert(const key& k, const T& v);
    T& operator[](const key& k);
    T* find(const key& k);
    T* findByValue(const T& v);
    Pair<key, T>* begin();
    Pair<key, T>* end();
    Pair<key, T>* data();

    const T& operator[](const key& k) const;
    const T* find(const key& k) const;
    const Pair<key, T>* begin() const;
    const Pair<key, T>* end() const;
    const Pair<key, T>* data() const;
};

template<class key, class T>
using Dict = Map<key, T, List>;
template<class key, class T>
using Gloss = Map<key, T, Stack>;

template<class key, class T, template<class> class con>
inline Map<key, T, con>::Map() :
    dat()
{
}

template<class key, class T, template<class> class con>
inline Map<key, T, con>::Map(const Map& m) :
    dat(m.dat)
{
}

template<class key, class T, template<class> class con>
inline Map<key, T, con>::Map(Map&& m) :
    dat((con<Pair<key, T>>&&)m.dat)
{
}

template<class key, class T, template<class> class con>
inline Map<key, T, con>& Map<key, T, con>::operator = (const Map& m)
{
    dat = m.dat;
}

template<class key, class T, template<class> class con>
inline Map<key, T, con>& Map<key, T, con>::operator = (Map&& m)
{
    dat = (con<Pair<key, T>>&&)m.dat;
    return *this;
}

template<class key, class T, template<class> class con>
inline unsigned Map<key, T, con>::size() const
{
    return dat.size();
}

template<class key, class T, template<class> class con>
inline unsigned Map<key, T, con>::capacity() const
{
    return dat.capacity();
}

template<class key, class T, template<class> class con>
inline void Map<key, T, con>::shrink()
{
    return dat.shrink();
}

template<class key, class T, template<class> class con>
inline void Map<key, T, con>::reserve(unsigned n)
{
    return dat.reserve(n);
}

template<class key, class T, template<class> class con>
inline void Map<key, T, con>::insert(const key& k, const T& v)
{
    auto i = dat.begin(), j = dat.end();
    for (; i != j && i->Key <= k; ++i)
    {
        if (i->Key == k)
        {
            i->Value = v;
            return;
        }
    }
    dat.insert(i, { k, v });
}


template<class key, class T, template<class> class con>
inline T& Map<key, T, con>::operator [](const key& k)
{
    auto i = dat.begin(), j = dat.end();
    for (; i != j && i->Key <= k; ++i)
        if (i->Key == k) return i->Value;
    return dat.insert(i, k)->Value;
}

template<class key, class T, template<class> class con>
inline T* Map<key, T, con>::find(const key& k)
{
    auto i = dat.begin(), j = dat.end();
    for (; i != j && i->Key <= k; ++i)
        if (i->Key == k) return &i->Value;
    return 0;
}

template<class key, class T, template<class> class con>
inline T* Map<key, T, con>::findByValue(const T& v)
{
    auto i = dat.begin(), j = dat.end();
    for (; i != j; ++i)
        if (i->Value == v) return &i->Value;
    return 0;
}

template<class key, class T, template<class> class con>
inline Pair<key, T>* Map<key, T, con>::data()
{
    return dat.data();
}

template<class key, class T, template<class> class con>
inline Pair<key, T>* Map<key, T, con>::begin()
{
    return dat.begin();
}

template<class key, class T, template<class> class con>
inline Pair<key, T>* Map<key, T, con>::end()
{
    return dat.end();
}

template<class key, class T, template<class> class con>
inline const T& Map<key, T, con>::operator [](const key& k) const
{
    auto i = dat.begin(), j = dat.end();
    for (; i != j && i->Key <= k; ++i)
        if (i->Key == k) return i->Value;
    return T();
}

template<class key, class T, template<class> class con>
inline const T* Map<key, T, con>::find(const key& k) const
{
    auto i = dat.begin(), j = dat.end();
    for (; i != j && i->Key <= k; ++i)
        if (i->Key == k) return &i->Value;
    return 0;
}

template<class key, class T, template<class> class con>
inline const Pair<key, T>* Map<key, T, con>::data() const
{
    return dat.data();
}

template<class key, class T, template<class> class con>
inline const Pair<key, T>* Map<key, T, con>::begin() const
{
    return dat.begin();
}

template<class key, class T, template<class> class con>
inline const Pair<key, T>* Map<key, T, con>::end() const
{
    return dat.end();
}
