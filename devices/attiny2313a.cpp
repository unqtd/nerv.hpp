#ifndef __AVR_ATtiny2313A__
#define __AVR_ATtiny2313A__
#include <cstdint>
#endif

#include "../digital.hpp"
#include "../nerv.hpp"

#ifndef _T2313A_CPP
#define _T2313A_CPP

namespace attiny2313a {
enum class PortName { A, B, D };

nerv::IOPort const *get_port(const PortName portname) {
  static nerv::IOPort const portb = {&DDRB, &PORTB, &PINB};
  static nerv::IOPort const porta = {&DDRA, &PORTA, &PINA};
  static nerv::IOPort const portd = {&DDRD, &PORTD, &PIND};

  switch (portname) {
  case PortName::A:
    return &porta;
  case PortName::B:
    return &portb;
  case PortName::D:
    return &portd;
  default:
    return nullptr;
  }
}

void init_timer_prescaler(nerv::timernum timer, nerv::Prescaler prescaler) {
  const nerv::bit8value cs0 = timer == 0 ? CS00 : CS10;
  const nerv::bit8value cs1 = timer == 0 ? CS01 : CS11;
  const nerv::bit8value cs2 = timer == 0 ? CS02 : CS12;

  nerv::rawbyte *tccrb = timer == 0 ? &TCCR0B : &TCCR1B;

  switch (prescaler) {
  case nerv::Prescaler::NoPrescale:
    break;
  case nerv::Prescaler::CLK0:
    *tccrb |= bitvalue(cs0);
    break;
  case nerv::Prescaler::CLK8:
    *tccrb |= bitvalue(cs1);
    break;
  case nerv::Prescaler::CLK1024:
    *tccrb |= bitvalue(cs2) | bitvalue(cs0);
    break;
  case nerv::Prescaler::ExternalOnFallingEdge:
    *tccrb |= bitvalue(cs1) | bitvalue(cs2);
    break;
  }
}

} // namespace attiny2313a

namespace concr {
using namespace attiny2313a;

nerv::IOPort const *get_port(const nerv::pinum pin) {
  if (pin <= 1 || (pin > 3 && pin <= 8))
    return get_port(PortName::D);
  else if (pin <= 3 || pin == 17)
    return get_port(PortName::A);
  else if (pin <= 16)
    return get_port(PortName::B);
  else
    return nullptr;
}

nerv::bit8value get_bitvalue(const nerv::pinum pin) {
  static nerv::bit8value const pins_map[] = {PD0, PD1, PA1, PA0, PD2, PD3,
                                             PD4, PD5, PD6, PB0, PB1, PB2,
                                             PB3, PB4, PB5, PB7, PA2};
  return pins_map[pin];
}

namespace timers {

void init_normal_timer(const nerv::timernum tnum) {
  switch (tnum) {
  case 0:
    break;
  case 1:
    break;
  }
}

void init_ctc_timer(const nerv::timernum tnum) {
  switch (tnum) {
  case 0:
    TCCR0A |= bitvalue(WGM01);
    break;
  case 1:
    TCCR1B |= bitvalue(WGM12);
    break;
  }
}

template <typename T>
void set_ctc_ocr_value(const nerv::timernum tnum, const T value) {
  switch (tnum) {
  case 0:
    OCR0A = value;
    break;
  case 1:
    if (sizeof(T) == sizeof(uint8_t))
      OCR1AL = value;
    else
      OCR1A = value;
    break;
  }
}

void init_prescaler(const nerv::timernum tnum,
                    const nerv::Prescaler prescaler) {
  attiny2313a::init_timer_prescaler(tnum, prescaler);
}

template <typename T> T get_timer_value(const nerv::timernum tnum) {
  switch (tnum) {
  case 0:
    return TCNT0;
  case 1:
    if (sizeof(T) == sizeof(uint8_t))
      return TCNT1L;
    else
      return TCNT1;
  default:
    return -1;
  }
}

nerv::timernum get_timernum_by_pin(const nerv::pinum pin) {
  switch (pin) {
  case 11: // OCR0A
    return 0;
  case 12: // OCR1A
  case 13: // OCR1B
    return 1;
  default:
    return TIMERNUM_UNDEF;
  }
}

template <typename T>
void set_timer_value(const nerv::timernum tnum, const T value) {
  switch (tnum) {
  case 0:
    TCNT0 = value;
    break;
  case 1:
    if (sizeof(T) == sizeof(uint8_t))
      TCNT1L = value;
    else
      TCNT1 = value;
    break;
  }
}

void clear_timer(const nerv::timernum tnum) {
  switch (tnum) {
  case 0:
    TCCR0A = 0;
    TCCR0B = 0;
    break;
  case 1:
    TCCR1A = 0;
    TCCR1B = 0;
    break;
  }
}

} // namespace timers

namespace pwm {

nerv::bit8value get_mask_phase_correct_pwm(const nerv::timernum tnum,
                                           const Bits bits) {
  if (tnum == 0) {
    return bitvalue(WGM00);
  } else if (tnum == 1) {
    switch (bits) {
    case Bits::B8:
      return bitvalue(WGM10);
    case Bits::B9:
      return bitvalue(WGM11);
    case Bits::B10:
    case Bits::BMAX:
      return bitvalue(WGM11) | bitvalue(WGM10);
    }
  }

  return 0;
}

nerv::bit8value get_mask_pin_pwm(const nerv::pinum pin) {
  switch (pin) {
  case 7: // OCA0B
    return bitvalue(COM0B1);
  case 11: // OCA0A
    return bitvalue(COM0A1);
  case 12: // OCR1A
    return bitvalue(COM1A1);
  case 13: // OCR1B
    return bitvalue(COM1B1);
  default:
    return 0;
  }
}

void init_phase_correct_pwm_on_pin(const nerv::timernum tnum, const Bits bits,
                                   const nerv::pinum pin) {
  const nerv::bit8value mask_tccr =
      get_mask_pin_pwm(pin) | get_mask_phase_correct_pwm(tnum, bits);

  switch (tnum) {
  case 0:
    TCCR0A = mask_tccr;
    break;
  case 1:
    TCCR1A = mask_tccr;
    break;
  }
}

template <typename T>
void set_pwm_ocr_value(const nerv::pinum pin, const T value) {
  switch (pin) {
  case 11: // OCR0A
    OCR0A = value;
    break;
  case 12: // OCR1A
    if (sizeof(T) == sizeof(uint8_t))
      OCR1AL = value;
    else
      OCR1A = value;
    break;
  case 13: // OCR1B
    if (sizeof(T) == sizeof(uint8_t))
      OCR1BL = value;
    else
      OCR1B = value;
    break;
  }
}

} // namespace pwm

} // namespace concr

#endif
