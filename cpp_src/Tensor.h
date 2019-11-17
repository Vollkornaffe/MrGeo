#pragma once

#include <array>
#include <vector>

template<size_t Order, typename T>
struct Tensor {

  using V_d_o = Eigen::Matrix<double, Order, 1>;
  using V_i_o = Eigen::Matrix<int, Order, 1>;
  using V_ui_o = Eigen::Matrix<size_t, Order, 1>;

  Tensor() {}

  Tensor(V_ui_o const & dimensions) : dimensions(dimensions) {

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

  inline T& operator[](size_t const & idx) {
    return values[idx];
  }
  inline T const& operator[](size_t const & idx) const {
    return values[idx];
  }

  inline T& operator[](V_ui_o const & idx) {
    return values[get_idx(idx)];
  }
  inline T const& operator[](V_ui_o const & idx) const {
    return values[get_idx(idx)];
  }

  V_ui_o dimensions;
  V_ui_o strides;
  std::vector<T> values;
};
