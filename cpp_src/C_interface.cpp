#include "Fourier.h"
#include "AnimationData.h"

extern "C" {

  QuaternionFourier* QF_construct(size_t max_spin) { return new QuaternionFourier(max_spin); }

  void QF_torus_test(QuaternionFourier* quaternionFourier) { quaternionFourier->test_interpolate(); }

  AnimationData* AD_construct() {return new AnimationData; }

  void AD_test_bases(
    AnimationData* data,
    QuaternionFourier* quaternionFourier,
    size_t num_lines,
    size_t num_frames
  ) { data->test_bases( *quaternionFourier, num_lines, num_frames ); }

  void AD_fill(
    AnimationData* data,
    QuaternionFourier* quaternionFourier,
    size_t num_lines,
    size_t num_frames
  ) { data->fill( *quaternionFourier, num_lines, num_frames ); }

  size_t AD_get_num_objects(AnimationData* data) { return data->num_objects; }
  size_t AD_get_num_frames(AnimationData* data) { return data->num_frames; }

  double * AD_get_scales(AnimationData* data, size_t i) { return data->scales_s[i][0].data(); }
  double * AD_get_locations(AnimationData* data, size_t i) { return data->locations_s[i][0].data(); }
  double * AD_get_rotations(AnimationData* data, size_t i) { return data->rotations_s[i][0].data(); }

};
