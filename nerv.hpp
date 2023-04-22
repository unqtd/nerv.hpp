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

enum class Prescaler { CLK0, CLK8 };
enum class TimerMode { Normal };

} // namespace nerv

namespace concr {
nerv::IOPort const *get_port(const nerv::pinum pin);
nerv::bit8value get_bitvalue(const nerv::pinum pin);

template <nerv::timernum T, nerv::TimerMode M> class Timer {
private:
  nerv::Prescaler prescaler;

public:
  Timer(nerv::Prescaler prescaler);

  uint16_t value();
  void set(const uint16_t value);

  int16_t get_frequency_divider() {
    switch (prescaler) {
    case nerv::Prescaler::CLK0:
      return 1;
    case nerv::Prescaler::CLK8:
      return 8;
    default:
      return 0;
    }
  }
};
} // namespace concr

#endif
