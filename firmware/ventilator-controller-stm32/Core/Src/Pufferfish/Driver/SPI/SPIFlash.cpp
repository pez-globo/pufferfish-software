/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 */

#include "Pufferfish/Driver/SPI/SPIFlash.h"

namespace Pufferfish {
namespace Driver {
namespace SPI {

uint16_t SPIFlash::getDeviceID(void){
  uint8_t txbuf[5] = { 0x90, 0x00, 0x00, 0x00, 0x00 };
  uint8_t rxbuf[5] = {0};
  uint8_t count = 5;
  uint16_t deviceid = 0;

  mDev.writeread(txbuf, rxbuf, count);

  deviceid = rxbuf[3];
  deviceid = (deviceid << 8) & 0xFF00;
  deviceid |= rxbuf[4];

  return deviceid;
}

SPIDeviceStatus SPIFlash::writeEnable(void){
  SPIDeviceStatus stat = mDev.write(&writeEnableInstruction, 1);

  if (stat == SPIDeviceStatus::ok) {
    return SPIDeviceStatus::ok;
  } else {
    return SPIDeviceStatus::writeError;
  }

}

SPIDeviceStatus SPIFlash::writeDisable(void){
  SPIDeviceStatus stat = mDev.write(&writeDisableInstruction, 1);

  if (stat == SPIDeviceStatus::ok) {
      return SPIDeviceStatus::ok;
    } else {
      return SPIDeviceStatus::writeError;
    }

}

SPIDeviceStatus SPIFlash::readData(uint32_t addr, uint8_t *rxBuf, uint8_t size){
  uint8_t txbuf[size+4] = {0};

  txbuf[0]= readDataInstruction;
  for (uint8_t index = 1; index<=3 ; index++)
  {
    txbuf[index] = (addr >> (8 * (3-index))) & 0xFF;
  }

  SPIDeviceStatus stat = mDev.writeread(txbuf, rxBuf, size+4);
  if (stat == SPIDeviceStatus::ok) {
        return SPIDeviceStatus::ok;
      } else {
        return SPIDeviceStatus::readError;
      }
}

bool SPIFlash::chipErase(void){
  SPIDeviceStatus stat = mDev.write(&chipEraseInstruction, 1);
  if (stat == SPIDeviceStatus::ok) {
    return true;
  } else {
    return false;
  }
}

bool SPIFlash::sectorErase4KB(uint32_t addr){
  uint8_t txbuf[4] = {0};
  txbuf[0]= sectorErase4KBInstruction;

  for (uint8_t index = 1; index<=3 ; index++)
  {
    txbuf[index] = (addr >> (8 * (3-index))) & 0xFF;
  }

  SPIDeviceStatus stat = mDev.write(txbuf, 4);
  if (stat == SPIDeviceStatus::ok) {
    return true;
  } else {
    return false;
  }
}

bool SPIFlash::blockErase32KB(uint32_t addr){
  uint8_t txbuf[4] = {0};
  txbuf[0]= blockErase32KBInstruction;
  for (uint8_t index = 1; index<=3 ; index++){
      txbuf[index] = (addr >> (8 * (3-index))) & 0xFF;
    }

  SPIDeviceStatus stat = mDev.write(txbuf, 4);
  if (stat == SPIDeviceStatus::ok) {
    return true;
  } else {
    return false;
  }

}

bool SPIFlash::blockErase64KB(uint32_t addr){
  uint8_t txbuf[4] = {0};
  txbuf[0]= blockErase64KBInstruction;
  for (uint8_t index = 1; index<=3 ; index++){
      txbuf[index] = (addr >> (8 * (3-index))) & 0xFF;
    }

  SPIDeviceStatus stat = mDev.write(txbuf, 4);
  if (stat == SPIDeviceStatus::ok) {
    return true;
  } else {
    return false;
  }

}

SPIDeviceStatus SPIFlash::readByte(uint32_t addr, uint8_t *rxBuf){
  uint8_t txbuf[4] = {0};
  for (uint8_t index = 0; index<=3 ; index++)
    {
      txbuf[index] = (addr >> (8 * (3-index))) & 0xFF;
    }

    SPIDeviceStatus stat = mDev.writeread(txbuf, rxBuf, 1);
    if (stat == SPIDeviceStatus::ok) {
          return SPIDeviceStatus::ok;
        } else {
          return SPIDeviceStatus::readError;
        }

}

// FIXME: we can do by using union
void SPIFlash::writeByte(uint32_t addr, uint8_t input){
   uint8_t txbuf[5] = {0};
   static uint8_t  defaultsize = 1;
    this->writeEnable();
    for (uint8_t index = 1; index<=3 ; index++){
          txbuf[index] = (addr >> (8 * (3-index))) & 0xFF;
        }

    mDev.write(txbuf, 4);
    mDev.write(&input, defaultsize);
    this->writeDisable();
}

}  // namespace SPI
}  // namespace Driver
}  // namespace Pufferfish


