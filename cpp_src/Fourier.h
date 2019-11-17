#pragma once

#include <math.h>
#include <vector>
#include <algorithm>

#include "util.h"

#include "TorusTest.h"

#include "CyclicSpline.h"

#include "Mapping.h"

inline int sgn(int val) {
  return (0 < val) - (val < 0);
}

struct QuaternionFourier {

  QuaternionFourier(size_t max_spin) : max_spin(max_spin) {

    u.reserve(max_spin*max_spin*4);
    int bound = max_spin;
    for (int spin_u = -bound; spin_u < bound + 1; spin_u ++) {
      for (int spin_v = -bound; spin_v < bound + 1; spin_v ++) {
        u.push_back(V2(spin_u, spin_v));
      }
    }
    std::sort(u.begin(), u.end(), [](V2 const & a, V2 const & b) {
      return a.squaredNorm() < b.squaredNorm();
    });

    num_terms = u.size();

    f_hat.resize(num_terms, Q(0.0,0.0,0.0,0.0));

  }

  void interpolate(std::function<Q(const V2 &)> f, size_t interpolation_res = 100) {

    double du = 1.0 / static_cast<double>(interpolation_res * interpolation_res);

    V2 x;
    for (size_t n = 0; n < interpolation_res; n++) {
      x[0] = static_cast<double>(n) / static_cast<double>(interpolation_res);

      for (size_t m = 0; m < interpolation_res; m++) {
        x[1] = static_cast<double>(m) / static_cast<double>(interpolation_res);

        Q q = f(x);

        for (size_t t = 0; t < num_terms; t ++) {

          V2 phase = - 2.0 * PI * x.cwiseProduct(u[t]);

          Q left(cos(phase[0]), sin(phase[0]), 0.0, 0.0);
          Q right(cos(phase[1]), 0.0, sin(phase[1]), 0.0);

          q_add(f_hat[t], left * q * right, du);
        }
      }
    }
  }

  void interpolate(CyclicSurface3d const& cyclicSurface3d, Mapping const & mapping = Mapping()) {
    double du = 1.0
      / static_cast<double>(cyclicSurface3d.num_samples[0])
      / static_cast<double>(cyclicSurface3d.num_samples[1])
    ;

    V2 x;
    for (size_t n = 0; n < cyclicSurface3d.num_samples[0]; n++) {
      x[0] = static_cast<double>(n) / static_cast<double>(cyclicSurface3d.num_samples[0]);

      for (size_t m = 0; m < cyclicSurface3d.num_samples[1]; m++) {
        x[1] = static_cast<double>(m) / static_cast<double>(cyclicSurface3d.num_samples[1]);

        Q q = mapping.get_quaternion(cyclicSurface3d.sample_points[{n,m}]);

        for (size_t t = 0; t < num_terms; t ++) {

          V2 phase = - 2.0 * PI * x.cwiseProduct(u[t]);

          Q left(cos(phase[0]), sin(phase[0]), 0.0, 0.0);
          Q right(cos(phase[1]), 0.0, sin(phase[1]), 0.0);

          q_add(f_hat[t], left * q * right, du);

        }
      }
    }
  }

  inline Q get_term(size_t n, V2 x) const {

    V2 phase = 2.0 * PI * x.cwiseProduct(u[n]);

    Q left(cos(phase[0]), sin(phase[0]), 0.0, 0.0);
    Q right(cos(phase[1]), 0.0, sin(phase[1]), 0.0);

    return left * f_hat[n] * right;

  }

  inline Q get_base(size_t n, V2 x) const {

    V2 phase = 2.0 * PI * x.cwiseProduct(u[n]);

    Q left(cos(phase[0]), sin(phase[0]), 0.0, 0.0);
    Q right(cos(phase[1]), 0.0, sin(phase[1]), 0.0);

    return left * Q(1.0,0.0,0.0,0.0) * right;

  }

  void test_interpolate() {
    interpolate(torus_test);
  }

  size_t num_terms;
  size_t max_spin;

  std::vector<V2> u;
  std::vector<Q> f_hat;

};
