/// SPIFlash.cpp
/// This file contains methods for SPI flash memory.

// Copyright (c) 2020 Pez-Globo and the Pufferfish project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied.
//
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Pufferfish/Driver/SPI/SPIFlash.h"

namespace Pufferfish {
namespace Driver {
namespace SPI {

SPIDeviceStatus SPIFlash::get_device_id(uint8_t &device_id) {
  uint8_t tx_buf[6] = {0};
  uint8_t rx_buf[6] = {0};
  uint8_t count = 6;

  /* Update the Byte0 of tx_buf with device Id instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::device_id);

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, rx_buf, count);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  device_id = rx_buf[5];
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::get_jedec_id(uint16_t &id) {
  uint8_t tx_buf[4] = {0};
  uint8_t rx_buf[4] = {0};
  uint8_t count = 4;

  /* Update the Byte0 of tx_buf with JEDEC Id instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::jedec_id);

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, rx_buf, count);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  id = rx_buf[2];
  id = (id << 8) & 0xFF00;
  id |= rx_buf[3];
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::enable_write() {
  /* Update the tx_buf with write enable instruction */
  auto tx_buf = static_cast<uint8_t>(SPIInstruction::write_enable);

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  SPIDeviceStatus ret = spi_.write(&tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::disable_write() {
  /* Update the tx_buf with write disable instruction */
  auto tx_buf = static_cast<uint8_t>(SPIInstruction::write_disable);

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  SPIDeviceStatus ret = spi_.write(&tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::write_byte(uint32_t addr, const uint8_t *input,
                                     uint8_t size) {
  uint8_t reg_data = 0;
  uint8_t tx_buf[size + 4] = {0};

  /* Invoke read_block_status to get the status of block */
  SPIDeviceStatus block_status = this->read_block_status(addr);

  if (block_status == SPIDeviceStatus::block_lock) {
    /* if block is locked then invoke unLockIndividualBlock to unlock the block
     */
    SPIDeviceStatus ret = this->unlock_individual_block(addr);
    /* return ret if it is not ok */
    if (ret != SPIDeviceStatus::ok) {
      return ret;
    }
  }

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with write byte instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::write_byte);

  /* Fill the Byte1-Byte3 with address and remaining bytes with input which is
   * to be written */
  for (uint8_t index = 1; index <= (size + 4); index++) {
    if (index < 4) {
      tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
    } else {
      tx_buf[index] = input[index - 4];
    }
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, size + 4);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 3ms */
  HAL::delay(3);

  /* Invoke lockIndividualBlock to lock the block */
  ret = this->lock_individual_block(addr);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::read_byte(uint32_t addr, uint8_t *data,
                                    uint8_t size) {
  uint8_t tx_buf[size + 4] = {0};
  uint8_t rx_buf[size + 4] = {0};

  /* Update the Byte0 of tx_buf with read byte instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::read_byte);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, rx_buf, size + 4);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  for (uint8_t index = 4; index < (size + 4); index++) {
    data[index - 4] = rx_buf[index];
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::lock_individual_block(uint32_t addr) {
  uint8_t tx_buf[5] = {0};
  uint8_t reg_data = 0;
  uint8_t size = 4;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with individual lock block instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::lock_block);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, size);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::unlock_individual_block(uint32_t addr) {
  uint8_t tx_buf[5] = {0};
  uint8_t reg_data = 0;
  uint8_t size = 4;

  /* Input for Write Status Register 3 - WPS is 1 */
  uint8_t reg3_input = 0x04;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Invoke write_status_register3 to make WPS bit to 1 */
  ret = this->write_status_register3(reg3_input);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Update the Byte0 of tx_buf with individual block unlock instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::unlock_block);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, size);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::global_block_unlock() {
  /* Update the tx_buf with global unlock instruction */
  auto tx_buf = static_cast<uint8_t>(SPIInstruction::global_unlock);
  uint8_t reg_data = 0;
  /* Input for Write Status Register 3 - WPS is 1 */
  uint8_t reg_input = 0x04;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Invoke write_status_register3 to make WPS bit to 1 */
  ret = this->write_status_register3(reg_input);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(&tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::global_block_lock() {
  /* Update the tx_buf with global lock instruction */
  auto tx_buf = static_cast<uint8_t>(SPIInstruction::global_lock);
  uint8_t reg_data = 0;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(&tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::read_block_status(uint32_t addr) {
  uint8_t tx_buf[5] = {0};
  uint8_t rx_buf[5] = {0};
  uint8_t size = 5;

  /* Update the Byte0 of tx_buf with read block status instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::read_block_status);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, rx_buf, size);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as block_lock */
  if (((rx_buf[4]) & 0x01) == 1) {
    return SPIDeviceStatus::block_lock;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::erase_chip() {
  uint8_t reg_data = 0;
  /* Update the tx_buf with chip erase instruction */
  auto tx_buf = static_cast<uint8_t>(SPIInstruction::chip_erase);

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Invoke globalBlockUnLock to unlock all the blocks */
  ret = this->global_block_unlock();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(&tx_buf, 1);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 25000ms */
  HAL::delay(25000);

  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::erase_sector_4kb(uint32_t addr) {
  uint8_t tx_buf[4] = {0};
  uint8_t reg_data = 0;

  /* Invoke read_block_status to get the status of block */
  SPIDeviceStatus block_status = this->read_block_status(addr);
  if (block_status == SPIDeviceStatus::block_lock) {
    /* if block is locked then invoke unLockIndividualBlock to unlock the block
     */
    SPIDeviceStatus ret = this->unlock_individual_block(addr);
    /* return ret if it is not ok */
    if (ret != SPIDeviceStatus::ok) {
      return ret;
    }
  }

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with sector erase of 4KB instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::sector_erase_4kb);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 4);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 400ms */
  HAL::delay(400);

  /* Invoke lockIndividualBlock to lock the block */
  ret = this->lock_individual_block(addr);
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::erase_block_32kb(uint32_t addr) {
  uint8_t tx_buf[4] = {0};
  uint8_t reg_data = 0;

  /* Invoke read_block_status to get the status of block */
  SPIDeviceStatus block_status = this->read_block_status(addr);
  if (block_status == SPIDeviceStatus::block_lock) {
    /* if block is locked then invoke unLockIndividualBlock to unlock the block
     */
    SPIDeviceStatus ret = this->unlock_individual_block(addr);
    /* return ret if it is not ok */
    if (ret != SPIDeviceStatus::ok) {
      return ret;
    }
  }

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with block erase 32KB instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::block_erase_32kb);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 4);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 1600ms */
  HAL::delay(1600);

  /* Invoke lockIndividualBlock to lock the block */
  ret = this->lock_individual_block(addr);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::erase_block_64kb(uint32_t addr) {
  uint8_t tx_buf[4] = {0};
  uint8_t reg_data = 0;

  /* Invoke read_block_status to get the status of block */
  SPIDeviceStatus block_status = this->read_block_status(addr);
  if (block_status == SPIDeviceStatus::block_lock) {
    /* if block is locked then invoke unLockIndividualBlock to unlock the block
     */
    SPIDeviceStatus ret = this->unlock_individual_block(addr);
    /* return ret if it is not ok */
    if (ret != SPIDeviceStatus::ok) {
      return ret;
    }
  }

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with block erase 64KB instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::block_erase_64kb);

  /* Fill the Byte1-Byte3 with address */
  for (uint8_t index = 1; index <= 3; index++) {
    tx_buf[index] = (addr >> (8 * (3 - index))) & 0xFF;
  }

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 4);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 2000ms */
  HAL::delay(2000);

  /* Invoke lockIndividualBlock to lock the block */
  ret = this->lock_individual_block(addr);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::write_status_register1(uint8_t input) {
  uint8_t tx_buf[2] = {0};
  uint8_t reg_data = 0;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(reg_data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((reg_data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with write status register 1 instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::write_status_register1);

  /* Update the Byte1 of tx_buf with input which is to be written */
  tx_buf[1] = (reg_data & input);

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 2);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 15ms */
  HAL::delay(15);

  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::read_status_register1(uint8_t &rx_buf) {
  uint8_t tx_buf[2] = {0};
  uint8_t temp[2] = {0};

  /* Update the Byte0 of tx_buf with read status register 1 instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::read_status_register1);

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, temp, 2);

  /* Make the CS pin High after read operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  rx_buf = temp[1];
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::write_status_register2(uint8_t input) {
  uint8_t tx_buf[2] = {0};
  uint8_t data_reg1 = 0;
  uint8_t data_reg2 = 0;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(data_reg1);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((data_reg1 & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Invoke read_status_register2 to get the register data */
  ret = this->read_status_register2(data_reg2);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Update the Byte0 of tx_buf with write status register 2 instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::write_status_register2);

  /* Update the Byte1 of tx_buf with input which is to be written */
  tx_buf[1] = (data_reg2 & input);

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 2);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 15ms */
  HAL::delay(15);
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::read_status_register2(uint8_t &rx_buf) {
  uint8_t tx_buf[2] = {0};
  uint8_t temp[2] = {0};

  /* Update the Byte0 of tx_buf with read status register 2 instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::read_status_register2);

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, temp, 2);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  rx_buf = temp[1];
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::write_status_register3(uint8_t input) {
  uint8_t tx_buf[2] = {0};
  uint8_t data_reg1 = 0;
  uint8_t data_reg3 = 0;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(data_reg1);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((data_reg1 & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Invoke read_status_register3 to get the status register 3 data */
  ret = this->read_status_register3(data_reg3);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Update the Byte0 of tx_buf with write status register 3 instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::write_status_register3);

  /* Update the Byte1 of tx_buf with input which is to be written */
  tx_buf[1] = (data_reg3 | input);

  /* Invoke enableWrite to set the WEL bit to 1 */
  ret = this->enable_write();
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 2);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 15ms */
  HAL::delay(15);
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::read_status_register3(uint8_t &rx_buf) {
  uint8_t tx_buf[2] = {0};
  uint8_t temp[2] = {0};

  /* Update the Byte0 of tx_buf with read status register 3 instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::read_status_register3);

  /* Make the CS pin Low before read operation*/
  spi_.chip_select(false);

  /* Write and Read data to and from the device */
  SPIDeviceStatus ret = spi_.write_read(tx_buf, temp, 2);

  /* Make the CS pin high after read operation*/
  spi_.chip_select(true);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  rx_buf = temp[1];
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::power_down() {
  /* Update the tx_buf with power down instruction */
  auto tx_buf = static_cast<uint8_t>(SPIInstruction::power_down);

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  SPIDeviceStatus ret = spi_.write(&tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 3microsec */
  HAL::delay_micros(3);
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::release_power_down() {
  uint8_t tx_buf[4] = {0};

  /* Update the Byte0 of tx_buf with release power down instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::release_power_down);

  /* Make the CS pin Low before write operation*/
  spi_.chip_select(false);

  /* Write data into the device */
  SPIDeviceStatus ret = spi_.write(tx_buf, 4);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 3microsec */
  HAL::delay_micros(3);
  /* return SPIDeviceStatus */
  return ret;
}

SPIDeviceStatus SPIFlash::reset_device() {
  uint8_t tx_buf[1] = {0};
  uint8_t data = 0;

  /* Invoke read_status_register1 to get the status of device */
  SPIDeviceStatus ret = this->read_status_register1(data);
  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* if LSB bit is 1 then return SPIDeviceStatus as busy */
  if ((data & 0x01) == 1) {
    return SPIDeviceStatus::busy;
  }

  /* Update the Byte0 of tx_buf with  enable reset instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::reset_enable);

  /* Make the CS pin Low before write operation */
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }

  /* provide a delay of 30 microsec */
  HAL::delay_micros(30);

  /* Update the Byte0 of tx_buf with reset device instruction */
  tx_buf[0] = static_cast<uint8_t>(SPIInstruction::reset_device);

  /* Make the CS pin Low after write operation */
  spi_.chip_select(false);

  /* Write data into the device */
  ret = spi_.write(tx_buf, 1);

  /* Make the CS pin High after write operation */
  spi_.chip_select(true);

  /* return ret if it is not ok */
  if (ret != SPIDeviceStatus::ok) {
    return ret;
  }
  /* return SPIDeviceStatus */
  return ret;
}

}  // namespace SPI
}  // namespace Driver
}  // namespace Pufferfish
