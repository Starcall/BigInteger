#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

struct big_integer {
    typedef uint32_t ui;
    typedef uint64_t ull;

    const ull BASE = static_cast<ull>(1ll << 32);
    big_integer();
    big_integer(big_integer const& other);

    big_integer(int a);

    big_integer(ui a);

    big_integer(ull a);

    big_integer(std::string const& str);

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(ui rhs);
    big_integer& operator>>=(ui rhs);

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

    friend big_integer operator+(big_integer a, big_integer const& b);
    friend big_integer operator-(big_integer a, big_integer const& b);
    friend big_integer operator*(big_integer a, big_integer const& b);
    friend big_integer operator/(big_integer const &a, big_integer const& b);
    friend big_integer operator%(big_integer const &a, big_integer const& b);

    friend big_integer operator&(big_integer a, big_integer const& b);
    friend big_integer operator|(big_integer a, big_integer const& b);
    friend big_integer operator^(big_integer a, big_integer const& b);
    friend big_integer operator<<(big_integer a, ui b);
    friend big_integer operator>>(big_integer a, ui b);

    friend std::string to_string(big_integer const& a);
    void swap(big_integer &other);
    big_integer(bool new_sign, vector<ui> const &new_data);
    bool isZero() const;

private:
    big_integer addAbs(big_integer const &a, big_integer const &b);
    big_integer subAbs(big_integer const &a, big_integer const &b);
    big_integer compareAbs(big_integer const &a, big_integer const &b);
    big_integer mul(ui const &a) const;
    inline void toTwosCompliment();
    inline void fromTwosCompliment();
    void make_fit(big_integer &value);
    void make_fit();
    bool sign;
    vector<ui> data;
};


#endif