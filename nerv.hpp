#include <avr/io.h>
#include <stdint.h>

#ifndef _NERV_HPP
#define _NERV_HPP

#define bitvalue(X) _BV(X)
#define loop while (true)

namespace nerv {

using bit8value = uint8_t;
using rawbyte = volatile uint8_t;

using pinum = uint8_t;
using timernum = uint8_t;

struct IOPort {
  rawbyte *ddr;
  rawbyte *port;
  rawbyte *pin;
};

enum class Prescaler { CLK0, CLK8, CLK1024, ExternalOnFallingEdge };
// enum class TimerMode { Normal, PhaseCorrectPWM };

} // namespace nerv

namespace concr {
nerv::IOPort const *get_port(const nerv::pinum pin);
nerv::bit8value get_bitvalue(const nerv::pinum pin);

namespace timers {

template <nerv::timernum T> class NormalTimer {
private:
  nerv::Prescaler prescaler;

public:
  NormalTimer(const nerv::Prescaler prescaler);

  template <typename Size> Size value();

  template <typename Size> void set(const Size value);

  nerv::Prescaler get_prescaler() { return prescaler; }
};

} // namespace timers

namespace pwm {

enum class NBits { B8 };

template <nerv::timernum T> class PhaseCorrect {
private:
  const nerv::pinum pin;

public:
  PhaseCorrect(const nerv::pinum pin, const NBits nbits,
               const nerv::Prescaler prescaler);

  template <typename Size> void write(const Size value);
};

} // namespace pwm

} // namespace concr

#endif
