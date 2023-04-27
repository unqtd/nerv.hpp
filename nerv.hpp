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

enum class Prescaler { NoPrescale, CLK0, CLK8, CLK1024, ExternalOnFallingEdge };

} // namespace nerv

namespace concr {
nerv::IOPort const *get_port(const nerv::pinum pin);
nerv::bit8value get_bitvalue(const nerv::pinum pin);

namespace timers {

void init_normal_timer(const nerv::timernum tnum);
void init_ctc_timer(const nerv::timernum tnum);

void init_prescaler(const nerv::timernum tnum, const nerv::Prescaler prescaler);
nerv::timernum get_timernum_by_pin(const nerv::pinum pin);

template <typename T>
void set_ctc_ocr_value(const nerv::timernum tnum, const T value);

template <typename T> T get_timer_value(const nerv::timernum tnum);

template <typename T>
void set_timer_value(const nerv::timernum tnum, const T value);

void clear_timer(const nerv::timernum tnum);

} // namespace timers

namespace pwm {

enum class Bits { B8 };

void init_phase_correct_pwm(const nerv::timernum tnum, const Bits bits);
void init_pin_pwm(const nerv::pinum pin);

template <typename T>
void set_pwm_ocr_value(const nerv::pinum pin, const T value);

} // namespace pwm

} // namespace concr

#endif
