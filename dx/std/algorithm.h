#pragma once

template<class allocator>
struct iterator
{
	allocator* p;
	iterator(allocator* p) : p(p) {}
	operator allocator*& () { return p; }
	operator iterator<const allocator>() { return p; }
};


namespace algorithm
{
	template<class allocator>
	inline static constexpr auto ascending = [](const allocator& x, const allocator& y) { return  x < y; };
	template<class allocator>
	inline static constexpr auto descending = [](const allocator& x, const allocator& y) { return  x > y; };

	template<class allocator>
	inline void swap(allocator* x, allocator* y)
	{
		allocator t = *x;
		*x = *y;
		*y = t;
	};

	namespace detail
	{
		template<class allocator>
		inline int partition(int low, int hi, bool (*f)(const allocator&, const allocator&), iterator<allocator> data)
		{
			allocator& pivot = data[hi];
			int i = (low - 1);
			for (int j = low; j <= hi - 1; j++) {
				if (f(data[j], pivot)) {
					i++;
					swap(data + i, data + j);
				}
			}
			swap(data + i + 1, data + hi);
			return (i + 1);
		}

		template<class allocator>
		inline void qSort(int low, int hi, bool (*func)(const allocator&, const allocator&), iterator<allocator> data)
		{
			if (low < hi) {
				int pi = partition(low, hi, func, data);
				qSort(low, pi - 1, func, data);
				qSort(pi + 1, hi, func, data);
			}
		}
	}

	template<class allocator>
	inline void sort(iterator<allocator> first, iterator<allocator> last, bool (*func)(const allocator&, const allocator&))
	{
		detail::qSort(0, last - first - 1, func, first);
	}

	template<class allocator>
	iterator<allocator> find(allocator item, iterator<allocator> first, iterator<allocator> last)
	{
		for (; first != last; ++first)
			if (*first == item) return first;
		return 0;
	}
}