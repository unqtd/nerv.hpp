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

template <timernum T, typename Size> class Timer {
private:
  Prescaler prescaler;

public:
  Timer(const Prescaler prescaler) : prescaler(prescaler) {
    concr::timers::clear_timer(T);
  }

  Size value() { return concr::timers::get_timer_value<Size>(T); }
  void set(const Size value) { concr::timers::set_timer_value(T, value); }

  void stop() { concr::timers::init_prescaler(T, Prescaler::NoPrescale); }
  void resume() { concr::timers::init_prescaler(T, prescaler); }

  Prescaler get_prescaler() { return prescaler; }
};

template <timernum T, typename Size> class NormalTimer : public Timer<T, Size> {
public:
  NormalTimer(const Prescaler prescaler) : Timer<T, Size>(prescaler) {
    concr::timers::init_normal_timer(T);
    concr::timers::init_prescaler(T, prescaler);
  }
};

template <timernum T, typename Size> class CTCTimer : public Timer<T, Size> {
public:
  CTCTimer(const Prescaler prescaler) : Timer<T, Size>(prescaler) {
    concr::timers::init_ctc_timer(T);
    concr::timers::init_prescaler(T, prescaler);
  }

  CTCTimer(const Prescaler prescaler, const Size top)
      : CTCTimer<T, Size>(prescaler) {
    set_top(top);
  }

  void set_top(const Size value) { concr::timers::set_ctc_ocr_value(T, value); }
};

template <timernum T, typename Size> class WaitTicksTimer {
private:
  Timer<T, Size> timer;

public:
  WaitTicksTimer(Timer<T, Size> timer) : timer(timer) {}

  void wait(const Size ticks) {
    timer.set(0);
    while (timer.value() <= ticks)
      ;
  }
};

} // namespace timers

} // namespace nerv

#endif
