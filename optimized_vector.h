#ifndef BIGINT_OPTIMIZED_VECTOR_H
#define BIGINT_OPTIMIZED_VECTOR_H

#include <cstdint>
#include <vector>
#include <memory>
#include <iostream>

struct optimized_vector {
    optimized_vector();

    optimized_vector(size_t size);

    optimized_vector(const optimized_vector &other);

    optimized_vector &operator=(const optimized_vector &other);

    void push_back(uint32_t);

    void pop_back();

    const uint32_t back() const;

    uint32_t &back();

    bool empty() const;

    size_t size() const;

    void resize(const size_t size);

    uint32_t &operator[](const size_t index);

    const uint32_t operator[](const size_t index) const;

private:
    struct  small_data {
        size_t size;
        union {
            uint32_t value;
            std::shared_ptr<std::vector<uint32_t>> values;
        };

        void make_big(std::shared_ptr<std::vector<uint32_t>> original) {
            if (size < 2) {
                new(&values) std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(*original));
            } else {
                values = original;
            }
        }

        void make_small(uint32_t val) {
            if (size > 1) {
                values.~shared_ptr();
            }
            value = val;
        }


        small_data() {
            size = 0;
        }
        small_data(size_t size) {
            this->size = 0;
            if (size < 2) {
                make_small(0);
            } else {
                make_big(std::make_shared<std::vector<uint32_t>>(size));
            }
            this->size = size;
        }

        small_data(small_data const& other) {
            if (other.size > 1) {
                make_big(other.values);
            } else {
                make_small(other.value);
            }
            size = other.size;
        }

        ~small_data() {
            if (size > 1) {
                values.~shared_ptr();
            }
        }

        small_data& operator=(small_data const& other) {
            if (other.size > 1) {
                make_big(other.values);
            } else {
                make_small(other.value);
            }
            size = other.size;
            return *this;
        }

        void copy_original() {
            if (size > 1 && !values.unique()) {
                make_big(std::make_shared<std::vector<uint32_t>>(*values));
            }
        }
    };

    small_data data;
};


#endif //BIGINT_OPTIMIZED_VECTOR_H