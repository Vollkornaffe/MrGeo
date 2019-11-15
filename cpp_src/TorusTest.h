#pragma once


#include "util.h"
#include "Mapping.h"

inline Q torus_test(const V2 & x) {
  Mapping mapping;
  V2 phase = 2.0*PI*x;
  V3 point(
    (1.0 + 0.5*cos(phase[0])) * cos(phase[1]),
    (1.0 + 0.5*cos(phase[0])) * sin(phase[1]),
    0.5*sin(phase[0])
  );
  return mapping.get_quaternion(point);
}