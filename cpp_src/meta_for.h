#pragma once

template<size_t Dimensions, size_t InitalDimensions, class Callable>
void help_meta_for_loop(
  std::array<size_t, Dimensions> const& mins,
  std::array<size_t, Dimensions> const& maxs,
  Callable&& c
) {
  for(
    size_t i = mins[InitalDimensions - Dimensions];
    i != maxs[InitalDimensions - Dimensions]; ++i)
  {
    if constexpr(Dimensions == 1)
    {
      c(i);
    }
    else
    {
      auto bind_an_argument = [i, &c](auto... args)
      {
          c(i, args...);
      };
      help_meta_for_loop<Dimensions-1, InitalDimensions>(mins, maxs, bind_an_argument);
    }
  }
}

template<size_t Dimensions, class Callable>
void meta_for_loop(
  std::array<size_t, Dimensions> const& mins,
  std::array<size_t, Dimensions> const& maxs,
  Callable&& c
) {
  static_assert(Dimensions > 0);
  help_meta_for_loop<Dimensions, Dimensions>(mins, maxs, std::forward(c));
}