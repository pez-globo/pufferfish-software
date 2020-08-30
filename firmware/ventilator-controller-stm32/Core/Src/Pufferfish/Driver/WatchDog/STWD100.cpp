/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 */

#include "Pufferfish/Driver/WatchDog/STWD100.h"

namespace Pufferfish {
namespace Driver {
namespace WatchDog {

void STWD100::enable(void){

  mEnablenPin.write(false);
}

void STWD100::disable(void){

  mEnablenPin.write(true);
}

void STWD100::toggleInput(void){

  mInputPin.write(true);
  ///TBD:Need to decide delay time
  HAL::delayMicros(2);
  mInputPin.write(false);
}

}  // namespace WatchDog
}  // namespace Driver
}  // namespace Pufferfish
