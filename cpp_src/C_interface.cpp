#include "Fourier.h"
#include "AnimationData.h"
#include "CyclicSpline.h"

#define VISIBLE(signature) \
  signature __attribute__ ((visibility ("default") ));\
  signature \

extern "C" {
  VISIBLE(QuaternionFourier* QF_construct(size_t max_spin)) { return new QuaternionFourier(max_spin); }

  VISIBLE(void QF_torus_test(QuaternionFourier* quaternionFourier)) { quaternionFourier->test_interpolate(); }

  VISIBLE(AnimationData* AD_construct()) {return new AnimationData; }

  VISIBLE(void AD_test_bases(
    AnimationData* data,
    QuaternionFourier* quaternionFourier,
    size_t num_lines,
    size_t num_frames
  )) { data->test_bases( *quaternionFourier, num_lines, num_frames ); }

  VISIBLE(void AD_fill(
    AnimationData* data,
    QuaternionFourier* quaternionFourier,
    size_t num_lines,
    size_t num_frames
  )) { data->fill( *quaternionFourier, num_lines, num_frames ); }

  VISIBLE(size_t AD_get_num_objects(AnimationData* data)) { return data->num_objects; }
  VISIBLE(size_t AD_get_num_frames(AnimationData* data)) { return data->num_frames; }

  VISIBLE(double * AD_get_scales(AnimationData* data, size_t i)) { return data->scales_s[i][0].data(); }
  VISIBLE(double * AD_get_locations(AnimationData* data, size_t i)) { return data->locations_s[i][0].data(); }
  VISIBLE(double * AD_get_rotations(AnimationData* data, size_t i)) { return data->rotations_s[i][0].data(); }

  VISIBLE(CyclicSurface3d * CS_construct(
    size_t u_degree,
    size_t v_degree,
    size_t u_numControl,
    size_t v_numControl,
    size_t u_numSamples,
    size_t v_numSamples
  )) {
    try {
      return new CyclicSurface3d(
        Eigen::Matrix<size_t, 2, 1>( u_degree, v_degree ),
        Eigen::Matrix<size_t, 2, 1>( u_numControl, v_numControl ),
        Eigen::Matrix<size_t, 2, 1>( u_numSamples, v_numSamples )
      );
    } catch (...)  {
      std::cout << "SURFACE CONSTRUCTION FAILED!" << std::endl;
      return nullptr;
    }
  }

  VISIBLE(double * CS_get_control(CyclicSurface3d * cs, size_t n)) {
    return cs->control_points[Eigen::Matrix<size_t, 2, 1>(n, 0)].data();
  }
  VISIBLE(void CS_compute_samples(CyclicSurface3d * cs)) {
    try {
      cs->compute_samples();
    } catch (...) {
      std::cout << "SAMPLE COMPUTATION FAILED!" << std::endl;
    }
  }
  VISIBLE(double * CS_get_samples(CyclicSurface3d * cs, size_t n)) {
    return cs->sample_points[Eigen::Matrix<size_t, 2, 1>(n, 0)].data();
  }

};
