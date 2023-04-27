#include "digital.hpp"
#include "nerv.hpp"

#ifndef _PWM_NERV_HPP
#define _PWM_NERV_HPP

namespace nerv {
namespace pwm {

enum class Mode { Fast, PhaseCorrect };

template <Mode M, typename Size> class PWM {
private:
  const nerv::timernum tnum;
  const nerv::pinum pin;

  nerv::Prescaler prescaler;

public:
  PWM(const nerv::pinum pin, const nerv::Prescaler prescaler)
      : tnum(concr::timers::get_timernum_by_pin(pin)), pin(pin),
        prescaler(prescaler) {
    concr::timers::clear_timer(tnum);

    switch (M) {
    case Mode::Fast:
    case Mode::PhaseCorrect:
      concr::pwm::init_phase_correct_pwm(tnum, concr::pwm::Bits::B8);
      break;
    }

    digital::OutputPin::init(pin);

    concr::pwm::init_pin_pwm(pin);
    concr::timers::init_prescaler(tnum, prescaler);
  }

  void write(const Size value) { concr::pwm::set_pwm_ocr_value(pin, value); }

  void stop() {
    concr::timers::init_prescaler(tnum, nerv::Prescaler::NoPrescale);
  }

  void resume() { concr::timers::init_prescaler(tnum, prescaler); }
};

} // namespace pwm
} // namespace nerv

#endif
