#ifndef __AVR_ATtiny2313A__
#define __AVR_ATtiny2313A__
#include <cstdint>
#endif

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
  const uint8_t cs0 = timer == 0 ? CS00 : CS10;
  const uint8_t cs1 = timer == 0 ? CS01 : CS11;
  const uint8_t cs2 = timer == 0 ? CS02 : CS12;

  volatile uint8_t *tccrb = timer == 0 ? &TCCR0B : &TCCR1B;

  switch (prescaler) {
  case nerv::Prescaler::CLK0:
    *tccrb &= ~(bitvalue(cs1) | bitvalue(cs2));
    *tccrb |= bitvalue(cs0);
    break;
  case nerv::Prescaler::CLK8:
    *tccrb &= ~(bitvalue(cs0) | bitvalue(cs2));
    *tccrb |= bitvalue(cs1);
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
  static nerv::bit8value const PINS[] = {PD0, PD1, PA1, PA0, PD2, PD3,
                                         PD4, PD5, PD6, PB0, PB1, PB2,
                                         PB3, PB4, PB5, PB7, PA2};
  return PINS[pin];
}

template <nerv::timernum T, nerv::TimerMode M>
Timer<T, M>::Timer(nerv::Prescaler prescaler) : prescaler(prescaler) {
  if (T == 0) {
    if (M == nerv::TimerMode::Normal) {
#ifndef AGGROPT_TIMER_IN_NORMAL_MODE
      TCCR0A &= ~(bitvalue(WGM00) | bitvalue(WGM01));
      TCCR0B &= ~bitvalue(WGM02);
#endif
    }
  } else if (T == 1) {
    if (M == nerv::TimerMode::Normal) {
#ifndef AGGROPT_TIMER_IN_NORMAL_MODE
      TCCR1A &= ~(bitvalue(WGM11) | bitvalue(WGM10));
      TCCR1B &= ~(bitvalue(WGM12) | bitvalue(WGM13));
#endif
    }
  }

  attiny2313a::init_timer_prescaler(T, prescaler);
}

template <nerv::timernum T, nerv::TimerMode M> uint16_t Timer<T, M>::value() {
  if (T == 0) {
    return TCNT0;
  } else { // T1
    return TCNT1;
  }
}

template <nerv::timernum T, nerv::TimerMode M>
void Timer<T, M>::set(const uint16_t value) {
  if (T == 0) {
    TCNT0 = value;
  } else { // T1
    TCNT1 = value;
  }
}

} // namespace concr

#endif
