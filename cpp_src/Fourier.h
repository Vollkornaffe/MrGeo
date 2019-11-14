#pragma once

#include <math.h>
#include <vector>
#include <algorithm>

#include "util.h"

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

  size_t num_terms;
  size_t max_spin;

  std::vector<V2> u;
  std::vector<Q> f_hat;

};
