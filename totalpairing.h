#ifndef _TOTAL_PAIRING_H_
#define _TOTAL_PAIRING_H_

#include <cstddef>//for std::size_t

template<class T>
struct __reference {
	typedef typename T::reference type;
};
template<class T>
struct __reference<T*> {
	typedef T& type;
};

template<class Iterator>
class Circulator {
	using Int = std::size_t;
	Iterator m_begin, current, m_end;
public:
	Circulator(Iterator begin, Int size, Int offset): m_begin(begin), current(begin + offset), m_end(m_begin + (size - 1)) {}
	Circulator& operator++() {
		if (current == m_end) {
			current = m_begin;
		} else {
			++current;
		}
		return *this;
	}
	Circulator& operator--() {
		if (current == m_begin) {
			current = m_end;
		} else {
			--current;
		}
		return *this;
	}
	typename __reference<Iterator>::type operator*() {
		return *current;
	}
};

template<class T> Circulator(T a, std::size_t b,std::size_t c)->Circulator<T>;

template<class const_Iterator>
class total_pairing {
	using Int = std::size_t;
	const_Iterator symbols;
	Int m_size;
	Int repeat_count = 0;
	Int offset = 0;
	bool algorithm = true;

	template<class Iterator>
	void even_polygon_pairing(const_Iterator source, Iterator dest) {
		Int n = m_size / 2;
		for (Int i = 0; i <= repeat_count; ++i) {
			Circulator left_write(dest, n, n - 1 - offset);
			Circulator right_write(dest + n, n, offset);
			Circulator left_read(source, n, 0);
			Circulator right_read(source + n, n, n - 1);
			for (Int j = 0; j < n; ++j) {
				*right_write = *left_read;
				*left_write = *right_read;
				++right_write;
				++left_read;
				--left_write;
				--right_read;
			}
			source += m_size;
			dest += m_size;
		}
	}
	template<class Iterator>
	void odd_polygon_pairing(const_Iterator source, Iterator dest) {
		Int n = m_size / 2;

		for (Int i = 0; i <= repeat_count; ++i) {
			Circulator left_write(dest, n, offset);
			Circulator left_read(source, n, offset);
			Circulator right_write(dest + n, n, (n - 1 + offset) % n);
			Circulator right_read(source + n, n, (n - 1 + offset) % n);

			*left_write = *right_read;
			*right_write = *left_read;

			for (Int j = 0; j < n - 1; ++j) {
				++left_write;
				--left_read;
				*left_write = *left_read;
				--right_write;
				++right_read;
				*right_write = *right_read;
			}
			source += m_size;
			dest += m_size;
		}
	}
	template<class Iterator>
	void final_pairing(const_Iterator source, Iterator dest) {
		for (Int i = 0; i <= repeat_count; ++i) {
			dest[0] = source[1];
			dest[1] = source[0];
			source += 2;
			dest += 2;
		}
	}

public:
	total_pairing(const_Iterator symbols, Int size): symbols(symbols), m_size(size) {
		if (m_size % 2) throw "Number of symbols must be even!";
	}
	total_pairing(const_Iterator symbols_begin, const_Iterator symbols_end): symbols(symbols_begin), m_size(symbols_end - symbols_begin) {
		if (m_size % 2) throw "Number of symbols must be even!";
	}
	template<class Iterator>
	bool operator()(Iterator dest) {
		if (m_size == 0) {
			return false;
		}
		if (m_size == 2) {
			final_pairing(symbols, dest);
			m_size = 0;
		} else if (algorithm) {
			even_polygon_pairing(symbols, dest);
			++offset;
			if (offset == m_size / 2 - 1 && m_size % 4) {
				algorithm = false;
				offset = 0;
			} else if (offset == m_size / 2) {
				offset = 0;
				m_size /= 2;
				repeat_count = 2 * repeat_count + 1;
			}
		} else {
			odd_polygon_pairing(symbols, dest);
			++offset;
			if (offset == m_size / 2) {
				m_size = 0;
			}
		}
		return true;
	}
};

template<class Iterator>
total_pairing(Iterator a, Iterator b)->total_pairing<Iterator>;
template<class Iterator>
total_pairing(Iterator a, std::size_t)->total_pairing<Iterator>;

#endif // !_TOTAL_PAIRING_H_
