#ifndef TOTALPAIRING_H
#define TOTALPAIRING_H


template<class Random_Access_Iterator>
class total_pairing {
	typedef Random_Access_Iterator Iterator;
	typedef typename Iterator::difference_type Int;
	Iterator symbols;
	Int m_size;
	Int repeat_count;
	Int iteration_count;
	bool algorithm;

	class Circulator {
		Iterator m_begin, current, m_end;
	public:
		Circulator(Iterator begin, Iterator end, Iterator current) :m_begin(begin), current(current), m_end(end) {}
		Circulator(Iterator begin, Int size, Int offset) : m_begin(begin), current(begin + offset), m_end(m_begin + size - 1) {}
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
		typename Iterator::reference operator*() {
			return *current;
		}
	};

	typedef typename total_pairing::Circulator circulator;

	void even_polygon_pairing(Iterator source,Iterator dest) {
		Int n = m_size / 2;
		for (Int i = 0; i <= repeat_count; ++i) {
			circulator left(dest, n, n - 1 - iteration_count);
			circulator right(dest + n, n, iteration_count);
			circulator left_symb(source, n, 0);
			circulator right_symb(source + n, n, n - 1);
			for (Int j = 0; j < n; ++j) {
				*right = *left_symb;
				*left = *right_symb;
				++right;
				++left_symb;
				--left;
				--right_symb;
			}
			source += m_size;
			dest += m_size;
		}
	}
	void odd_polygon_pairing(Iterator source,Iterator dest) {
		Int n = m_size / 2;

		for (Int i = 0; i <= repeat_count; ++i) {
			circulator left_write(dest, n, iteration_count);
			circulator left_read(source, n, iteration_count);
			circulator right_write(dest + n, n, (n - 1 + iteration_count) % n);
			circulator right_read(source + n, n, (n - 1 + iteration_count) % n);

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
	void final_pairing(Iterator source,Iterator dest) {
		for (Int i = 0; i <= repeat_count; ++i) {
			dest[0] = source[1];
			dest[1] = source[0];
			source += 2;
			dest += 2;
		}
	}

public:
	total_pairing(Iterator symbols, Int size) :symbols(symbols), m_size(size) {
		if (m_size % 2) throw "Number of symbols must be even!";
		repeat_count = 0;
		iteration_count = 0;
		algorithm = true;
	}
	total_pairing(Iterator symbols_begin, Iterator symbols_end) :symbols(symbols_begin), m_size(symbols_end - symbols_begin) {
		if (m_size % 2) throw "Number of symbols must be even!";
		repeat_count = 0;
		iteration_count = 0;
		algorithm = true;
	}
	bool operator()(Iterator dest) {
		if (m_size == 0) {
			return false;
		}
		if (m_size == 2) {
			final_pairing(symbols,dest);
			m_size = 0;
		}
		if (algorithm) {
			even_polygon_pairing(symbols,dest);
			++iteration_count;
			if (iteration_count == m_size / 2 - 1 && m_size % 4) {
				algorithm = false;
				iteration_count = 0;
			} else if (iteration_count == m_size / 2) {
				iteration_count = 0;
				m_size /= 2;
				repeat_count = 2 * repeat_count + 1;
			}
		} else {
			odd_polygon_pairing(symbols,dest);
			++iteration_count;
			if (iteration_count == m_size / 2) {
				m_size = 0;
			}
		}
		return true;
	}
};

#endif // TOTALPAIRING_H
