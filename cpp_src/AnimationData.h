#include <vector>
#include <array>

#include <iostream>

#include "util.h"
#include "Mapping.h"
#include "Fourier.h"

struct AnimationData {

  size_t num_objects = 0;
  size_t num_frames = 0;

  std::vector<std::vector<V3>> scales_s;
  std::vector<std::vector<V3>> locations_s;
  std::vector<std::vector<V4>> rotations_s;

  V3 default_dir = V3::UnitX();

  void test_bases(
    QuaternionFourier const & quaternionFourier,
    size_t requested_lines,
    size_t requested_frames = 1000,
    Mapping const & mapping = Mapping()
  ) {

    clear();
    alloc(quaternionFourier.num_terms * requested_lines, requested_frames);

    size_t object_idx = 0;

    for (size_t line = 0; line < requested_lines; line++) {
      double l = static_cast<double>(line) / static_cast<double>(requested_lines);

      for (size_t term = 0; term < quaternionFourier.num_terms; term++) {

        // location stays the same over the frames
        V3 location(
          (double)line,
          quaternionFourier.u[term][0],
          quaternionFourier.u[term][1]
        );

        for (size_t frame = 0; frame < requested_frames; frame++) {
          double t = static_cast<double>(frame) / static_cast<double>(requested_frames);

          Q q = quaternionFourier.get_base(term, V2(
            l,
            t
          ));

          V3 p = mapping.get_point(q);

          double norm = p.norm();

          scales_s[object_idx][frame][0] = norm;
          scales_s[object_idx][frame][1] = 1.0;
          scales_s[object_idx][frame][2] = 1.0;
          locations_s[object_idx][frame] = location;
          rotations_s[object_idx][frame] = sane_conv(Q::FromTwoVectors(default_dir, p));

        }

        object_idx++;

      }

    }

  }

  void fill(
    QuaternionFourier const & quaternionFourier,
    size_t requested_lines,
    size_t requested_frames = 1000,
    Mapping const & mapping = Mapping()
  ) {

    clear();
    alloc(quaternionFourier.num_terms * requested_lines, requested_frames);

    for (size_t line = 0; line < requested_lines; line++) {
      double l = static_cast<double>(line) / static_cast<double>(requested_lines);

      for (size_t frame = 0; frame < requested_frames; frame++) {
        double t = static_cast<double>(frame) / static_cast<double>(requested_frames);

        V3 offset = V3::Zero();
        for (size_t term = 0; term < quaternionFourier.num_terms; term++) {

          V3 location = offset;

          //double phase = 2.0*PI*t;
          //Q q = quaternionFourier.get_term(term, l*V2(
          //  sin(phase),
          //  cos(phase)
          //));

          Q q = quaternionFourier.get_term(term, V2(
            t,
            t * 10.0 + l
          ));

          V3 p = mapping.get_point(q);

          double norm = p.norm();

          size_t object_idx = line * quaternionFourier.num_terms + term;
          scales_s[object_idx][frame][0] = norm;
          scales_s[object_idx][frame][1] = std::min(1.0, norm);
          scales_s[object_idx][frame][2] = std::min(1.0, norm);
          locations_s[object_idx][frame] = location;
          rotations_s[object_idx][frame] = sane_conv(Q::FromTwoVectors(default_dir, p));

          offset += p;

        }

      }

    }

  }

  void clear() {
    num_objects = 0;
    num_frames = 0;
    for (auto & scales : scales_s) scales.clear();
    for (auto & locations : locations_s) locations.clear();
    for (auto & rotations : rotations_s) rotations.clear();
  }

  void alloc(size_t _num_objects, size_t _num_frames) {
    num_objects = _num_objects;
    num_frames = _num_frames;
    scales_s.resize(num_objects, std::vector<V3>(num_frames));
    locations_s.resize(num_objects, std::vector<V3>(num_frames));
    rotations_s.resize(num_objects, std::vector<V4>(num_frames));
  }

};

