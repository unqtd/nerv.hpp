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
  case nerv::Prescaler::CLK0:
    *tccrb &= ~(bitvalue(cs1) | bitvalue(cs2));
    *tccrb |= bitvalue(cs0);
    break;
  case nerv::Prescaler::CLK8:
    *tccrb &= ~(bitvalue(cs0) | bitvalue(cs2));
    *tccrb |= bitvalue(cs1);
    break;
  case nerv::Prescaler::CLK1024:
    *tccrb &= ~bitvalue(cs1);
    *tccrb |= bitvalue(cs2) | bitvalue(cs0);
    break;
  case nerv::Prescaler::ExternalOnFallingEdge:
    *tccrb &= ~bitvalue(cs0);
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

template <nerv::timernum T>
NormalTimer<T>::NormalTimer(const nerv::Prescaler prescaler)
    : prescaler(prescaler) {
#ifndef AGGROPT_TIMER_IN_NORMAL_MODE
  if (T == 0) {
    TCCR0A &= ~(bitvalue(WGM00) | bitvalue(WGM01));
    TCCR0B &= ~bitvalue(WGM02);
  } else if (T == 1) {
    TCCR1A &= ~(bitvalue(WGM11) | bitvalue(WGM10));
    TCCR1B &= ~(bitvalue(WGM12) | bitvalue(WGM13));
  }
#endif

  attiny2313a::init_timer_prescaler(T, prescaler);
}

template <nerv::timernum T>
template <typename Size>
Size NormalTimer<T>::value() {
  if (T == 0) {
    return TCNT0;
  } else { // T1
    if (sizeof(Size) == sizeof(uint8_t))
      return TCNT1L;
    else
      return TCNT1;
  }
}

template <nerv::timernum T>
template <typename Size>
void NormalTimer<T>::set(const Size value) {
  if (T == 0) {
    TCNT0 = value;
  } else { // T1
    if (sizeof(Size) == sizeof(uint8_t))
      TCNT1L = value;
    else
      TCNT1 = value;
  }
}

} // namespace timers

namespace pwm {

template <nerv::timernum T>
PhaseCorrect<T>::PhaseCorrect(const nerv::pinum pin, const NBits nbits,
                              const nerv::Prescaler prescaler)
    : pin(pin) {
  nerv::digital::OutputPin::init(pin);
  attiny2313a::init_timer_prescaler(T, prescaler);

  if (T == 0) {
#warning "Not impl PhaseCorrect T0"
  } else if (T == 1) {
    TCCR1A &= ~(bitvalue(WGM11) | bitvalue(WGM10));
    TCCR1B &= ~(bitvalue(WGM12) | bitvalue(WGM13));

    switch (pin) {
    case 12: // OCR1A
      TCCR1A |= bitvalue(COM1A1);
      TCCR1A &= ~bitvalue(COM1A0);
      break;
    case 13: // OCR1B
      TCCR1A |= bitvalue(COM1B1);
      TCCR1A &= ~bitvalue(COM1B0);
      break;
    }

    switch (nbits) {
    case NBits::B8:
      TCCR1A |= bitvalue(WGM10);
      break;
    }
  }
}

template <nerv::timernum T>
template <typename Size>
void PhaseCorrect<T>::write(const Size value) {
  if (T == 1) {
    switch (pin) {
    case 12: // OCR1A
      OCR1A = value;
      break;
    case 13: // OCR1B
      OCR1B = value;
      break;
    }
  } else if (T == 0) {
    switch (pin) {
    case 11: // OCR0A
      OCR0A = value;
      break;
    }
  }
}

} // namespace pwm

} // namespace concr

#endif
