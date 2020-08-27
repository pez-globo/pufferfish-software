/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *
 */

#pragma once

#include "Pufferfish/HAL/Interfaces/SPIDevice.h"

namespace Pufferfish {
namespace Driver {
namespace SPI {

/**
 * A class represents SPI flash memory in STM32
 */

class SPIFlash : public HAL::SPIDevice {

public:
  SPIFlash(SPIDevice &spi)
       :mDev(spi){
  }

  /** Reads the spi device id
   *
   * @param void
   * @return deviceid get device id
   */
   uint16_t getDeviceID(void);

   /** Enable write for the spi device
     *
     * @param void
     * @return ok on success, error code otherwise
     */
   SPIDeviceStatus writeEnable(void);

   /** Disable write for the spi device
     *
     * @param void
     * @return ok on success, error code otherwise
     */
   SPIDeviceStatus writeDisable(void);

   /** Read data from the spi device
     *
     * @param addr address to read data
     * @param rxBuf pointer to reception data buffer
     * @param size amount of data to be receive
     * @return ok on success, error code otherwise
     */
   SPIDeviceStatus readData(uint32_t addr, uint8_t *rxBuf, uint8_t size);

   /** Chip Erase
     *
     * @param void
     * @return true on success, false otherwise
     */
   bool chipErase(void);

   /** Sector Erase - 4KB
     *
     * @param addr address to erase the sector
     * @return true on success, false otherwise
     */
   bool sectorErase4KB(uint32_t addr);

   /** Block Erase - 32KB
     *
     * @param addr address to erase the block
     * @return true on success, false otherwise
     */
   bool blockErase32KB(uint32_t addr);

   /** Block Erase - 64KB
     *
     * @param addr address to erase the block
     * @return true on success, false otherwise
     */
   bool blockErase64KB(uint32_t addr);

   /** Read byte from the spi device
     *
     * @param addr address to read data
     * @param rxBuf pointer to reception data buffer
     * @return ok on success, error code otherwise
     */
   SPIDeviceStatus readByte(uint32_t addr, uint8_t *rxBuf);

   /** write byte into the spi device
     *
     * @param addr address to write data
     * @param input data to be written
     */
   void writeByte(uint32_t addr, uint8_t input);

private:
   SPIDevice &mDev;
   uint8_t readDataInstruction =0x03;
   uint8_t writeEnableInstruction = 0x06;
   uint8_t writeDisableInstruction = 0x04;
   uint8_t chipEraseInstruction = 0x60;
   uint8_t sectorErase4KBInstruction = 0x20;
   uint8_t blockErase32KBInstruction = 0x52;
   uint8_t blockErase64KBInstruction = 0xD8;
};

}  // namespace SPI
}  // namespace Driver
}  // namespace Pufferfish
