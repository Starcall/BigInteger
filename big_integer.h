#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cstdint>
#include <string>
#include <vector>
#include "optimized_vector.h"

using namespace std;

struct big_integer {
	big_integer();
	big_integer(big_integer const& other);
	big_integer(int a);
	explicit big_integer(std::string const& str);
	~big_integer();

	big_integer& operator=(big_integer const& other);

	big_integer& operator+=(big_integer const& rhs);
	big_integer& operator-=(big_integer const& rhs);
	big_integer& operator*=(big_integer const& rhs);
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs);

	big_integer& operator&=(big_integer const& rhs);
	big_integer& operator|=(big_integer const& rhs);
	big_integer& operator^=(big_integer const& rhs);

	big_integer& operator<<=(int rhs);
	big_integer& operator>>=(int rhs);

	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;

	big_integer& operator++();
	big_integer operator++(int);

	big_integer& operator--();
	big_integer operator--(int);

	friend bool operator==(big_integer const& a, big_integer const& b);
	friend bool operator!=(big_integer const& a, big_integer const& b);
	friend bool operator<(big_integer const& a, big_integer const& b);
	friend bool operator>(big_integer const& a, big_integer const& b);
	friend bool operator<=(big_integer const& a, big_integer const& b);
	friend bool operator>=(big_integer const& a, big_integer const& b);

	friend std::string to_string(big_integer const& a);

private:
	void remove_leading_zeros();

	static big_integer add_abs(optimized_vector const& a, optimized_vector const& b);
	static big_integer sub_abs(optimized_vector const& a, optimized_vector const& b);
	static big_integer compare_abs(optimized_vector const& a, optimized_vector const& b);
	big_integer mul(uint32_t const& rhs) const;

    template<typename FunctorT>
    big_integer& apply_unary(big_integer const & b, FunctorT functor);

	uint32_t div_mod(uint32_t const& a);
	uint32_t norm_ratio() const;
	void to_twos_complement();
	void from_twos_complement();

	const static uint64_t BASE = static_cast<uint64_t>(UINT32_MAX) + 1;

	int8_t sign;
	optimized_vector data;
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator >> (big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif // BIG_INTEGER_H

