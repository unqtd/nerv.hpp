#include "nerv.hpp"

#ifndef _TIMER_HPP
#define _TIMER_HPP

namespace nerv {
namespace timers {

inline int16_t get_frequency_divider(Prescaler prescaler) {
  switch (prescaler) {
  case nerv::Prescaler::CLK0:
    return 1;
  case nerv::Prescaler::CLK8:
    return 8;
  case nerv::Prescaler::CLK1024:
    return 1024;
  default:
    return 0;
  }
}

template <typename T, typename Size> class WaitTimer {
private:
  T timer;

public:
  WaitTimer(T timer) : timer(timer) {}

  void wait(const Size ticks) {
    timer.template set<Size>(0);
    while (timer.template value<Size>() <= ticks)
      ;
  }
};

} // namespace timers

} // namespace nerv

#endif
