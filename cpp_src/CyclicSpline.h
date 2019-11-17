#pragma once

#include <array>

#include <Eigen/Dense>

#include <iostream>

#include "Tensor.h"
#include "CyclicTensor.h"
#include "meta_for.h"

#include "util.h"

class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException () : std::logic_error{"Function not yet implemented."} {}
};

inline void advance(
  CyclicTensor<1, double> const & intervals,
  double const & t,
  size_t & i,
  double & offset
) {
  // advance interval if needed
  // with safeguard against floating point errors
  if (offset + intervals[i] < t && i < intervals.values.size()-1) {
    offset += intervals[i];
    i++;
  }
}

inline void advance(
  CyclicTensor<1, double> const & intervals,
  double const & t,
  size_t const & degree,
  size_t & i,
  double & offset,
  std::vector<double> & left,
  std::vector<double> & right
) {

  // advance interval if needed
  // with safeguard against floating point errors
  if (offset + intervals[i] < t && i < intervals.values.size()-1) {
    offset += intervals[i];
    i++;
  }

  left[0] = t - offset - intervals[i];
  right[0] = offset - t;
  for (size_t j = 1; j < degree+1; j++) {
    left[j] = left[j-1] + intervals[(int)i - (int)j + 1];
    right[j] = right[j-1] + intervals[(int)i + (int)j - 1];
  }

}

inline void compute_bases(
  CyclicTensor<1, double> const & intervals,
  size_t const & degree,
  size_t const & i,
  std::vector<double> const & left,
  std::vector<double> const & right,
  std::vector<double> & bases
) {

  bases[0] = 1.0;
  for (size_t j = 1; j < degree + 1; j++) {
    double saved = 0.0;
    for (size_t r = 0; r < j; r++) {
      double temp = bases[r] / (right[r+1] + left[j-r]);
      bases[r] = saved + right[r+1] * temp;
      saved = left[j-r] * temp;
    }
    bases[j] = saved;
  }

}

template<size_t Order, size_t Dimension>
struct CyclicSpline {

  using V_d_o = Eigen::Matrix<double, Order, 1>;
  using V_d_d = Eigen::Matrix<double, Dimension, 1>;

  using V_i_o = Eigen::Matrix<int, Order, 1>;
  using V_i_d = Eigen::Matrix<int, Dimension, 1>;

  using V_ui_o = Eigen::Matrix<size_t, Order, 1>;
  using V_ui_d = Eigen::Matrix<size_t, Dimension, 1>;

  CyclicSpline(
    V_ui_o const & degrees,
    V_ui_o const & num_controls,
    V_ui_o const & num_samples
  ) : degrees(degrees), num_controls(num_controls), num_samples(num_samples) {

    for (size_t i = 0; i < Order; i++) {
      if (num_controls[i] < 3) {
        throw std::invalid_argument("Number of control points MUST be at least 3 (in each dimension).");
      }
      if (degrees[i] + 1 > num_controls[i]) {
        throw std::invalid_argument("Number of control points MUST be at least degree (in each dimension).");
      }
      if (num_samples[i] < 1) {
        throw std::invalid_argument("Number of sample points MUST be at least 1 (in each dimension).");
      }
    }

    for (size_t i = 0; i < Order; i++) {
      control_intervals[i].values.resize(num_controls[i], 1.0/static_cast<double>(num_controls[i]));
    }

    control_points = CyclicTensor<Order, V_d_d>(num_controls);
    sample_points = Tensor<Order, V_d_d>(num_samples);

    std::cout << "Number Control Points: " << control_points.values.size() << std::endl;
    std::cout << "Number Sample Points: " << sample_points.values.size() << std::endl;

    for (size_t o = 0; o < Order; o++) {
      bases_s[o].resize(num_samples[o], std::vector<double>(degrees[o]+1));
    }

  }

  V_ui_o degrees;
  V_ui_o num_controls;
  V_ui_o num_samples;

  std::array<CyclicTensor<1, double>, Order> control_intervals;
  CyclicTensor<Order, V_d_d> control_points;
  Tensor<Order, V_d_d> sample_points;

  std::array<std::vector<std::vector<double>>, Order> bases_s;

  // Most of this algorithm is inspired by The NURBS Book, Algorithm A2.2
  void compute_samples() {

    for (size_t o = 0; o < Order; o++) {
      size_t i = 0;
      double offset = 0;
      std::vector<double> left(degrees[o]+1);
      std::vector<double> right(degrees[o]+1);
      for (size_t s = 0; s < num_samples[o]; s++) {
        auto & bases = bases_s[o][s];
        double sd = static_cast<double>(s)/ static_cast<double>(num_samples[o]);
        advance(control_intervals[o], sd, degrees[o], i, offset, left, right);
        compute_bases(control_intervals[o], degrees[o], i, left, right, bases);
      }
    }

    std::cout << "bases computed." << std::endl;

    V_ui_o sample_idx;
    V_ui_o interval_idx;
    V_d_o offsets;
    V_ui_o degree_idx;
    V_i_o control_idx;

    if constexpr (Order == 1) {

      for (
        sample_idx[0] = 0, interval_idx[0] = 0, offsets[0] = 0.0;
        sample_idx[0] < num_samples[0];
        sample_idx[0]++
      ) {
        double sd0 = static_cast<double>(sample_idx[0])/ static_cast<double>(num_samples[0]);
        advance(control_intervals[0], sd0, interval_idx[0], offsets[0]);

        sample_points[sample_idx] = V_d_d::Zero();

        for (degree_idx[0] = 0; degree_idx[0] < degrees[0]+1; degree_idx[0]++) {

          control_idx =
            interval_idx.template cast<int>()
            - degrees.template cast<int>()
            + degree_idx.template cast<int>();
          double weight =
            bases_s[0][sample_idx[0]][degree_idx[0]];

          sample_points[sample_idx] += weight * control_points[control_idx];

        }
      }

    } else if constexpr (Order == 2) {

      for (
        sample_idx[0] = 0, interval_idx[0] = 0, offsets[0] = 0.0;
        sample_idx[0] < num_samples[0];
        sample_idx[0]++
      ) {
        double sd0 = static_cast<double>(sample_idx[0])/ static_cast<double>(num_samples[0]);
        advance(control_intervals[0], sd0, interval_idx[0], offsets[0]);

        for (
          sample_idx[1] = 0, interval_idx[1] = 0, offsets[1] = 0.0;
          sample_idx[1] < num_samples[1];
          sample_idx[1]++
        ) {
          double sd1 = static_cast<double>(sample_idx[1])/ static_cast<double>(num_samples[1]);
          advance(control_intervals[1], sd1, interval_idx[1], offsets[1]);

          sample_points[sample_idx] = V_d_d::Zero();

          for (degree_idx[0] = 0; degree_idx[0] < degrees[0]+1; degree_idx[0]++) {
            for (degree_idx[1] = 0; degree_idx[1] < degrees[1]+1; degree_idx[1]++) {

              control_idx =
                interval_idx.template cast<int>()
                - degrees.template cast<int>()
                + degree_idx.template cast<int>();
              double weight =
                bases_s[0][sample_idx[0]][degree_idx[0]] *
                bases_s[1][sample_idx[1]][degree_idx[1]];

              sample_points[sample_idx] += weight * control_points[control_idx];

            }
          }
        }
      }

    } else if constexpr (Order == 3) {

      for (
        sample_idx[0] = 0, interval_idx[0] = 0, offsets[0] = 0.0;
        sample_idx[0] < num_samples[0];
        sample_idx[0]++
      ) {
        double sd0 = static_cast<double>(sample_idx[0])/ static_cast<double>(num_samples[0]);
        advance(control_intervals[0], sd0, interval_idx[0], offsets[0]);

        for (
          sample_idx[1] = 0, interval_idx[1] = 0, offsets[1] = 0.0;
          sample_idx[1] < num_samples[1];
          sample_idx[1]++
        ) {
          double sd1 = static_cast<double>(sample_idx[1])/ static_cast<double>(num_samples[1]);
          advance(control_intervals[1], sd1, interval_idx[1], offsets[1]);

          for (
            sample_idx[2] = 0, interval_idx[2] = 0, offsets[2] = 0.0;
            sample_idx[2] < num_samples[2];
            sample_idx[2]++
          ) {
            double sd2 = static_cast<double>(sample_idx[2])/ static_cast<double>(num_samples[2]);
            advance(control_intervals[2], sd2, interval_idx[2], offsets[2]);

            sample_points[sample_idx] = V_d_d::Zero();

            for (degree_idx[0] = 0; degree_idx[0] < degrees[0]+1; degree_idx[0]++) {
              for (degree_idx[1] = 0; degree_idx[1] < degrees[1]+1; degree_idx[1]++) {
                for (degree_idx[2] = 0; degree_idx[2] < degrees[2]+1; degree_idx[2]++) {

                  control_idx =
                    interval_idx.template cast<int>()
                    - degrees.template cast<int>()
                    + degree_idx.template cast<int>();
                  double weight =
                    bases_s[0][sample_idx[0]][degree_idx[0]] *
                    bases_s[1][sample_idx[1]][degree_idx[1]] *
                    bases_s[2][sample_idx[2]][degree_idx[2]];

                  sample_points[sample_idx] += weight * control_points[control_idx];

                }
              }
            }
          }
        }
      }

    } else {
      throw NotImplementedException();
    }

  }

};

using CyclicCurve3d = CyclicSpline<1, 3>;
using CyclicSurface3d = CyclicSpline<2, 3>;
