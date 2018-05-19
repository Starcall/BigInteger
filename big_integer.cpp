#include <iostream>
#include "big_integer.h"

using namespace std;


big_integer::big_integer() {
    this->sign = false; // plus
    this->data.resize(1);
}

big_integer::big_integer(ui a) {
    this->sign = false;
    this->data.resize(1);
    this->data[0] = a;
}

big_integer::big_integer(int a) {
    this->sign = (a < 0);
    this->data.resize(1);
    this->data[0] = static_cast<unsigned int>(a < 0 ? -a : a);
}

big_integer::big_integer(ull a) {
    this->sign = false;
    this->data.resize(2);
    this->data[0] = static_cast<unsigned int>(a % BASE);
    this->data[1] = static_cast<unsigned int>(a / BASE);
    this->make_fit();
}

big_integer::big_integer(std::string const &str) {
    //TODO
}

big_integer::big_integer(big_integer const &other) {
    this->sign = other.sign;
    this->data = other.data;
}

big_integer::big_integer(bool sign, vector<ui> const &data) {
    this->sign = sign;
    this->data = data;
}

void big_integer::swap(big_integer &other) {
    std::swap(this->sign, other.sign);
    std::swap(this->data, other.data);
}

big_integer& big_integer::operator=(big_integer const &other) {
    big_integer temp(other);
    this->swap(temp);
    return *this;
}

void big_integer::make_fit(big_integer &value) {
    while (!value.data.empty() && value.data.back() == 0)
        value.data.pop_back();
    if (value.data.empty())
        value.data.push_back(0);
}

void big_integer::make_fit() {
    this->make_fit(*this);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign < b.sign)
        return false;
    if (a.sign > b.sign)
        return true;
    if (!a.sign && !b.sign) {
        if (a.data.size() > b.data.size()) {
            return false;
        } else {
            if (a.data.size() < b.data.size()) {
                return true;
            } else {
                for (size_t i = a.data.size() - 1; i + 1 != 0; i--) {
                    if (a.data[i] < b.data[i])
                        return true;
                    if (a.data[i] > b.data[i])
                        return false;
                }
                return false;
            }
        }
    } else {
        if (a.data.size() > b.data.size()) {
            return true;
        } else {
            if (a.data.size() < b.data.size()) {
                return false;
            } else {
                for (size_t i = a.data.size() - 1; i + 1 != 0; i--) {
                    if (a.data[i] < b.data[i])
                        return false;
                    if (a.data[i] > b.data[i])
                        return true;
                }
                return true;
            }
        }
    }
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

bool operator==(big_integer const &a, big_integer const &b) {
    return !(a < b) && !(b < a);
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator>(big_integer const &a, big_integer const &b) {
    return (a >= b) && (a != b);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer ret = big_integer(!this->sign, this->data);
    return ret;
}

big_integer& big_integer::operator+=(big_integer const &b) {
    if (b.isZero()) {
        return *this;
    }
    if (this->isZero()) {
        *this = b;
        return *this;
    }
    if (this->sign == b.sign) {
        big_integer tmp = this->addAbs(*this, b);
        this->data = tmp.data;
    } else {
        big_integer tmp = subAbs(*this, b);
        this->sign = tmp.sign != this->sign;
        this->data = tmp.data;
    }
    return *this;
}

big_integer big_integer::addAbs(big_integer const &a, big_integer const &b) {
    size_t max_size = std::max(a.data.size(), b.data.size());
    big_integer c;
    c.data.resize(max_size);
    c.sign = false;
    uint64_t r = 0;
    for (size_t i = 0; i < max_size; i++) {
        r = (i < a.data.size() ? static_cast<ull>(a.data[i]) : 0) + (i < b.data.size() ? static_cast<ull>(b.data[i]) : 0) + r;
        c.data[i] = static_cast<uint32_t>(r % BASE);
        r /= BASE;
    }
    if (r) {
        c.data.push_back(static_cast<uint32_t>(r));
    }
    return c;
}

big_integer big_integer::subAbs(big_integer const &a, big_integer const &b) {
    big_integer c = compareAbs(a, b);
    if (c.sign) {
        c.data = compareAbs(b, a).data;
    }
    return c;
}

big_integer big_integer::compareAbs(big_integer const &a, big_integer const &b) {
    size_t max_size = std::max(a.data.size(), b.data.size());
    big_integer c;
    c.data.resize(max_size);
    int64_t r = 0;
    for (size_t i = 0; i < max_size; i++) {
        r = (i < a.data.size() ? static_cast<int64_t>(a.data[i]) : 0) - (i < b.data.size() ? static_cast<int64_t>(b.data[i]) : 0) + r;
        if (r < 0) {
            r += static_cast<int64_t>(BASE);
            c.data[i] = static_cast<ui>(r);
            r = -1;
        } else {
            c.data[i] = static_cast<ui>(r);
            r = 0;
        }
    }
    if (r) {
        c.sign = true;
    } else {
        c.sign = false;
    }
    make_fit();
    return c;
}
big_integer& big_integer::operator-=(big_integer const &rhs) {
    if (rhs.isZero()) {
        return *this;
    }
    if (this->isZero()) {
        *this = rhs;
        this->sign ^= true;
        return *this;
    }
    if (this->sign == rhs.sign) {
        big_integer tmp = subAbs(*this, rhs);
        this->sign = this->sign != tmp.sign;
        this->data = tmp.data;
    } else {
        this->data = addAbs(*this, rhs).data;
    }
    return *this;
}

big_integer operator-(big_integer a, big_integer const &b) {
    a -= b;
    return a;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer& big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {

    big_integer ret = *this;
    *this += 1;
    return ret;
}

big_integer& big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {

    big_integer ret = *this;
    *this -= 1;
    return *this;
}

bool big_integer::isZero() const {
    return this->data.size() == 1 && this->data[0] == 0;
}

big_integer big_integer::mul(ui const& rhs) const  {
    uint64_t r = 0;
    big_integer ans;
    ans.sign = this->sign;
    for (ui i : this->data) {
        r = static_cast<ull>(i) * rhs + r;
        ans.data.push_back(static_cast<ui>(r % BASE));
        r /= BASE;
    }
    if (r) {
        ans.data.push_back(static_cast<ui>(r));
    }
    return ans;
}

big_integer& big_integer::operator*=(big_integer const &t) {
    bool sign = t.sign != this->sign;
    this->sign = true;
    big_integer rhs = t;
    rhs.sign = true;
    big_integer tmp = 0;
    for (size_t i = rhs.data.size() - 1; i + 1 != 0; i--) {
        tmp <<= 32;
        ui j = rhs.data[i];
        tmp += this->mul(j);
    }
    this->data = tmp.data;
    this->sign = sign;
    this->make_fit();
    return *this;
}

inline void big_integer::toTwosCompliment() {
    if (this->data.back() >> 31) {
        this->data.push_back(0);
    }
    if (this->sign) {
        *this += 1;
        for (ui &i : this->data) {
            i = ~i;
        }
    }
}

inline void big_integer::fromTwosCompliment() {
    if (this->isZero()) {
        return;
    }
    if (this->data.back() >> 31) {
        this->sign = true;
    } else {
        this->sign = false;
    }
    if (this->sign) {
        *this += 1;
        for (ui &i : this->data) {
            i = ~i;
        }
    }
}

big_integer& big_integer::operator>>=(ui rhs) {
    if (rhs == 0 || this->isZero()) {
        return *this;
    }
    ui digits = static_cast<ui>(rhs) / 32;
    ui mod = static_cast<ui>(rhs) % 32;
    vector<ui> tmp;

    this->toTwosCompliment();

    if (mod) {
        for (size_t i = 0; i < data.size(); i++) {
            if (!tmp.empty()) {
                tmp.back() |= (data[i] << (static_cast<ui>(32) - mod));
            }
            if (i == data.size() - 1 && this->sign) {
                auto a = static_cast<int32_t>(data[i]);
                a >>= mod;
                tmp.push_back(static_cast<ui>(a));
            } else {
                tmp.push_back(data[i] >> mod);
            }
        }
    } else {
        tmp = this->data;
    }

    this->data.resize(0);
    for (size_t i = digits; i < tmp.size(); i++) {
        this->data.push_back(tmp[i]);
    }

    this->fromTwosCompliment();
    this->make_fit();
    return *this;
}

big_integer& big_integer::operator<<=(ui rhs) {
    if (rhs == 0 || this->isZero()) {
        return *this;
    }
    ui digits = static_cast<ui>(rhs) / 32;
    ui mod = static_cast<ui>(rhs) % 32;
    vector<ui> tmp;

    this->toTwosCompliment();

    if (mod) {
        tmp.push_back(0);
        for (unsigned int i : data) {
            tmp.back() |= (i << mod);
            tmp.push_back(i >> (static_cast<uint32_t>(32) - mod));
        }
    } else {
        tmp = this->data;
    }

    this->data.resize(0);
    for (size_t i = 0; i < digits; i++) {
        this->data.push_back(0);
    }
    for (ui i : tmp) {
        this->data.push_back(i);
    }

    this->fromTwosCompliment();
    this->make_fit();
    return *this;
}

big_integer operator<<(big_integer a, uint32_t b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, uint32_t b) {
    return a >>= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer& big_integer::operator%=(big_integer const &rhs) {
    return *this -= (*this / rhs) * rhs;
}

big_integer& big_integer::operator&=(big_integer const &rhs) {
    big_integer tmp(rhs);
    if (tmp.data.size() > this->data.size()) {
        std::swap(*this, tmp);
    }
    while (this->data.size() > tmp.data.size()) {
        tmp.data.push_back(0);
    }

    this->toTwosCompliment();
    tmp.toTwosCompliment();

    for (size_t i = 0; i < std::min(tmp.data.size(), this->data.size()); i++) {
        this->data[i] &= tmp.data[i];
    }

    this->fromTwosCompliment();
    this->make_fit();
    return *this;
}

big_integer& big_integer::operator|=(big_integer const &rhs) {
    big_integer tmp(rhs);
    if (tmp.data.size() > this->data.size()) {
        std::swap(*this, tmp);
    }
    while (this->data.size() > tmp.data.size()) {
        tmp.data.push_back(0);
    }

    this->toTwosCompliment();
    tmp.toTwosCompliment();

    for (size_t i = 0; i < std::min(tmp.data.size(), this->data.size()); i++) {
        this->data[i] |= tmp.data[i];
    }

    this->fromTwosCompliment();
    this->make_fit();
    return *this;
}

big_integer& big_integer::operator^=(big_integer const &rhs) {
    big_integer tmp(rhs);
    if (tmp.data.size() > this->data.size()) {
        std::swap(*this, tmp);
    }
    while (this->data.size() > tmp.data.size()) {
        tmp.data.push_back(0);
    }

    this->toTwosCompliment();
    tmp.toTwosCompliment();

    for (size_t i = 0; i < std::min(tmp.data.size(), this->data.size()); i++) {
        this->data[i] ^= tmp.data[i];
    }

    this->fromTwosCompliment();
    this->make_fit();
    return *this;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    big_integer a(0);
    return a;
}

big_integer big_integer::operator~() const {
    return big_integer();
}

big_integer operator/(big_integer const &a, big_integer const &b) {
    return big_integer();
}

big_integer operator%(big_integer const &a, big_integer const &b) {
    return big_integer();
}

std::string to_string(big_integer const &a) {
    return std::string();
}


int main() {

    cout << (big_integer(2) + big_integer(2) == big_integer(4);

    return 0;
}