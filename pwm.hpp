#include "digital.hpp"
#include "nerv.hpp"

#ifndef _PWM_NERV_HPP
#define _PWM_NERV_HPP

namespace nerv {
namespace pwm {

enum class Mode { Fast, PhaseCorrect };

template <Mode M, typename Size = uint8_t> class PWM {
private:
  const nerv::timernum tnum;
  const nerv::pinum pin;

  nerv::Prescaler prescaler;

public:
  PWM(const nerv::pinum pin, const nerv::Prescaler prescaler,
      const concr::pwm::Bits bits)
      : tnum(concr::timers::get_timernum_by_pin(pin)), pin(pin),
        prescaler(prescaler) {
    concr::timers::clear_timer(tnum);

    switch (M) {
    case Mode::Fast:
      // TODO!
    case Mode::PhaseCorrect:
      concr::pwm::init_phase_correct_pwm(tnum, bits, pin, prescaler);
      break;
    }

    digital::OutputPin::init(pin);
  }

  PWM(const nerv::pinum pin, const nerv::Prescaler prescaler)
      : PWM<M, Size>(pin, prescaler,
                     (sizeof(Size) == sizeof(uint8_t)
                          ? concr::pwm::Bits::B8
                          : concr::pwm::Bits::BMAX)) {}

  void write(const Size value) { concr::pwm::set_pwm_ocr_value(pin, value); }

  void stop() {
    concr::timers::init_prescaler(tnum, nerv::Prescaler::NoPrescale);
  }

  void resume() { concr::timers::init_prescaler(tnum, prescaler); }
};

} // namespace pwm
} // namespace nerv

#endif
