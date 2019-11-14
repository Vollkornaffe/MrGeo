#include "CyclicTensor.h"
#include <array>

template<size_t Order, size_t Dimension>
struct CyclicSpline {

  using Point = std::array<double, Dimension>;
  using Integers = std::array<size_t, Order>;

  CNURBS(
    Integers const & degrees,
    Integers const & num_controls,
    Integers const & num_samples
  ) : degrees(degrees), num_controls(num_controls), num_samples(num_samples) {

    for (size_t i = 0; i < Order; i++) {
      control_intervals[i] =
    }
  }

  Integers degrees;
  Integers num_controls;
  Integers num_samples;

  std::array<CyclicTensor<1, double>, Order> control_intervals;
  CyclicTensor<Order, std::array<double, Dimension>> control_points;
};