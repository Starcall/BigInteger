#include "optimized_vector.h"

optimized_vector::optimized_vector() : optimized_vector(0) {}

optimized_vector::optimized_vector(size_t size) : data(size) {}

void optimized_vector::push_back(uint32_t a) {
	data.copy_original();
	if (data.size == 0) {
		data.make_small(a);
	} else {
		if (data.size == 1) {
			uint32_t tmp = data.value;
			data.make_big(std::make_shared<std::vector<uint32_t>>(0));
			data.values->push_back(tmp);
		}
		data.values->push_back(a);
	}
	data.size++;
}

void optimized_vector::pop_back() {
	data.copy_original();
	if (data.size == 2) {
		uint32_t tmp = data.values->operator[](0);
		data.make_small(tmp);
	} else if (data.size > 2) {
		data.values->pop_back();
	}
	data.size--;
}

const uint32_t optimized_vector::back() const {
	if (data.size <= 1) {
		return data.value;
	} else {
		return data.values->back();
	}
}

uint32_t &optimized_vector::back() {
	data.copy_original();
	if (data.size <= 1) {
		return data.value;
	} else {
		return data.values->back();
	}
}

bool optimized_vector::empty() const {
	return data.size == 0;
}

size_t optimized_vector::size() const {
	return data.size;
}

void optimized_vector::resize(const size_t size) {
	data.copy_original();
	if (size <= 1) {
		uint32_t tmp;
		if (data.size > 1) {
			tmp = data.values->operator[](0);
		} else {
			tmp = data.value;
		}
		data.make_small(tmp);
		data.size = size;
	} else {
		uint32_t tmp = 0;
		if (data.size <= 1) {
			if (data.size == 1)
				tmp = data.value;
			data = small_data(size);
			data.values->operator[](0) = tmp;
		} else {
			data.values->resize(size);
		}
		data.size = size;
	}
}

uint32_t &optimized_vector::operator[](const size_t index) {
	data.copy_original();
	if (data.size <= 1) {
		return data.value;
	} else {
		return data.values->operator[](index);
	}
}

const uint32_t optimized_vector::operator[](const size_t index) const {
	if (data.size <= 1) {
		return data.value;
	} else {
		return data.values->operator[](index);
	}
}

optimized_vector::optimized_vector(const optimized_vector &other) {
	data = other.data;
}

optimized_vector &optimized_vector::operator=(const optimized_vector &other) {
	data = other.data;
	return (*this);
}