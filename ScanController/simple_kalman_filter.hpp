#pragma once

#include <concepts>

namespace sc {
template <std::floating_point T = float> class simple_kalman_filter {

  T state_;
  T error_;

  T r_;
  T q_;

public:
  simple_kalman_filter(T initial_value, T initial_error, T q, T r);
  auto update(T value) -> T;
};

template <std::floating_point T>
simple_kalman_filter<T>::simple_kalman_filter(T initial_value, T initial_error,
                                              T q, T r)
    : state_(initial_value), error_(initial_error), r_(r), q_(q) {}

template <std::floating_point T>
auto simple_kalman_filter<T>::update(T value) -> T {
  const auto predict_error = error_ + q_;
  const auto amplification = predict_error / (predict_error + r_);
  state_ = state_ + amplification * (value - state_);
  error_ = (1 - amplification) * predict_error;
  return state_;
}
} // namespace sc
