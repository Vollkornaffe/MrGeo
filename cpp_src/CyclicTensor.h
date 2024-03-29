#pragma once

#include <vector>

template<size_t Order, typename T>
struct CyclicTensor {

  using V_d_o = Eigen::Matrix<double, Order, 1>;
  using V_i_o = Eigen::Matrix<int, Order, 1>;
  using V_ui_o = Eigen::Matrix<size_t, Order, 1>;

  CyclicTensor() {}

  CyclicTensor(V_ui_o const & dimensions) : dimensions(dimensions) {

    size_t stride = 1;
    for (size_t i = 0; i < Order; i++) {
      strides[Order - i - 1] = stride;
      stride *= dimensions[Order - i - 1];
    }

    values.resize(dimensions[0] * strides[0]);

  }

  inline size_t get_idx( V_ui_o const & idx) {

    size_t res = 0;
    for (size_t i = 0; i < Order; i++) {
      res += idx[i] * strides[i];
    }

    return res;

  }

  inline size_t get_canon( int const & not_canon) const {

    int mod = not_canon % values.size();
    if (mod < 0) mod += values.size();

    return size_t(mod);

  }

  inline size_t get_canon( V_i_o const & not_canon) {

    size_t res = 0;
    for (size_t i = 0; i < Order; i++) {

      int mod = not_canon[i] % (int)dimensions[i];
      if (mod < 0) mod += (int)dimensions[i];

      res += mod * strides[i];

    }

    return res;

  }

  inline T& operator[](size_t const & idx) {
    return values[idx];
  }
  inline T const& operator[](size_t const & idx) const {
    return values[idx];
  }

  inline T& operator[](int const & not_canon) {
    return values[get_canon(not_canon)];
  }
  inline T const& operator[](int const & not_canon) const {
    return values[get_canon(not_canon)];
  }

  inline T& operator[](V_ui_o const & idx) {
    return values[get_idx(idx)];
  }
  inline T const& operator[](V_ui_o const & idx) const {
    return values[get_idx(idx)];
  }

  inline T& operator[](V_i_o const & not_canon) {
    return values[get_canon(not_canon)];
  }
  inline T const& operator[](V_i_o const & not_canon) const {
    return values[get_canon(not_canon)];
  }

  V_ui_o dimensions;
  V_ui_o strides;
  std::vector<T> values;
};



