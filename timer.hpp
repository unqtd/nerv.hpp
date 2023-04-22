#include "nerv.hpp"

#ifndef _TIMER_HPP
#define _TIMER_HPP

namespace nerv {

template <nerv::timernum T, nerv::TimerMode M> class WaitTimer {
private:
  concr::Timer<T, M> timer;

public:
  WaitTimer(concr::Timer<T, M> timer) : timer(timer) {}

  void wait(const uint16_t ticks) {
    timer.set(0);
    while (timer.value() <= ticks)
      ;
  }
};

} // namespace nerv

#endif
