#include "nerv.hpp"

#ifndef _DIGITAL_CPP
#define _DIGITAL_CPP

namespace nerv {
namespace digital {

// enum class PinMode { Input, InputPullUp, Output };
enum class State { High = 1, Low = 0 };
enum class InputPinMode { PullUp, PullDown };

template <InputPinMode M> class InputPin {
private:
  rawbyte *pin = nullptr;
  bit8value bit;

public:
  InputPin(const pinum pin) {
    auto ioport = concr::get_port(pin);
    auto bit = concr::get_bitvalue(pin);

    if (ioport == nullptr)
      return;

    *ioport->ddr &= ~bitvalue(bit);

    switch (M) {
    case InputPinMode::PullUp:
      *ioport->port |= bitvalue(bit);
      break;
    case InputPinMode::PullDown:
#ifndef OFF_DEFAULT_INIT
      *ioport->port &= ~bitvalue(bit);
#endif
      break;
    }

    this->pin = ioport->pin;
    this->bit = bit;
  }

  State state() {
    // ...
    return static_cast<State>(*pin & bitvalue(bit));
  }
};

class OutputPin {
private:
  rawbyte *port = nullptr;
  bit8value bit;

public:
  OutputPin(const pinum pin) {
    auto ioport = concr::get_port(pin);
    auto bit = concr::get_bitvalue(pin);

    if (ioport == nullptr)
      return;

    *ioport->ddr |= bitvalue(bit);
#ifndef OFF_DEFAULT_INIT
    *ioport->port &= ~bitvalue(bit);
#endif

    this->port = ioport->port;
    this->bit = bit;
  }

  void write(const State state) {
    if (port == nullptr)
      return;

    if (state == State::High)
      *port |= bitvalue(bit);
    else
      *port &= ~bitvalue(bit);
  }

  void toggle() {
    if (port == nullptr)
      return;

    *port ^= bitvalue(bit);
  }
};

} // namespace digital
} // namespace nerv

#endif
