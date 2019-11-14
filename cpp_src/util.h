#pragma once

#include <Eigen/Dense>

# define PI 3.141592653589793238462643383279502884L

using V2 = Eigen::Vector2d;
using V3 = Eigen::Vector3d;
using V4 = Eigen::Vector4d;
using Q = Eigen::Quaternion<double>;

inline V4 sane_conv(const Q & q) {
  return V4(
    q.w(),
    q.x(),
    q.y(),
    q.z()
  );
}
inline Q sane_conv(const V4 & v) {
  return Q(
    v[0],
    v[1],
    v[2],
    v[3]
  );
}
inline void q_add(Q & a, const Q & b, double s) {
  a.w() += s * b.w();
  a.x() += s * b.x();
  a.y() += s * b.y();
  a.z() += s * b.z();
}
