#include "nerv.hpp"

#ifndef _PWM_NERV_HPP
#define _PWM_NERV_HPP

namespace nerv {
namespace pwm {

enum class Mode { Fast, PhaseCorrect };

template <Mode M>
class PWM {
};

} // namespace pwm
} // namespace nerv

#endif
