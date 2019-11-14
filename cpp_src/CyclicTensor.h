#pragma once

#include <array>
#include <vector>

template<size_t Order, typename T>
struct CyclicTensor {

  CyclicTensor(std::array<size_t, Order> const & dimensions) : dimensions(dimensions) {
    size_t alloc = 1;
    for (size_t i = 0; i < Order; i++) {
      alloc *= dimensions[i];
    }
    values.resize(alloc);
  }

  static inline size_t get_canon( std::array<int, Order> const & not_canon) {

    size_t res = 0;
    size_t stride = 1;
    for (size_t i = Order-1; i >= 0; i--) {

      int mod = not_canon[i] % (int)dimensions[i];
        if (value < 0) mod += (int)dimensions[i];

      res += mod * stride;

      stride *= dimensions[i];
    }

    return res;

  }

  inline T& operator[](std::array<int, Order> const & not_canon) {
    return values[get_canon(not_canon)];
  }
  inline T const& operator[](std::array<int, Order> const & not_canon) const {
    return values[get_canon(not_canon)];
  }

  std::array<size_t, Order> dimensions;
  std::vector<T> values;
};

