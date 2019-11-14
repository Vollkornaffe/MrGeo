#pragma once

#include "util.h"

struct Mapping {

  Mapping(
    V3 a = V3(1,1,1),
    V3 b = V3(-1,-1,1),
    V3 c = V3(-1,1,-1),
    V3 d = V3(1,-1,-1)
  ) : a(a), b(b), c(c), d(d) {

    Eigen::Matrix4d tmp;
    tmp <<
      a[0], b[0], c[0], d[0],
      a[1], b[1], c[1], d[1],
      a[2], b[2], c[2], d[2],
      1.0 , 1.0 , 1.0 , 1.0;

    inv = tmp.inverse();

  }

  inline V3 get_point(const V4 & p) const {
    return  p[0] * a + p[1] * b + p[2] * c + p[3] * d;
  }

  inline V3 get_point(const Q & q) const {
    return q.w() * a + q.x() * b + q.y() * c + q.z() * d;
  }

  inline V4 get_coords(const V3 & p) const {
    return inv * V4(p[0],p[1],p[2],1.0);
  }

  inline Q get_quaternion(const V3 & p) const {
    return sane_conv(get_coords(p));
  }

  V3 a,b,c,d;

  Eigen::Matrix4d inv;

};

