/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * SensirionDevice.cpp
 *
 *  Created on: Dec 1, 2020
 *      Author: Rohan Purohit
 *
 * Unit tests to confirm behavior Sensirion Device
 *
 */
#include "Pufferfish/Driver/I2C/SensirionDevice.h"

#include "Pufferfish/Driver/I2C/SFM3019/Types.h"
#include "Pufferfish/HAL/CRCChecker.h"
#include "Pufferfish/HAL/Mock/MockI2CDevice.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Array.h"
#include "catch2/catch.hpp"

#include <iostream>
namespace PF = Pufferfish;

SCENARIO("SensirionDevice:: Sensirion device behaves correctly when a uint16_t command is given as input", "[sensiriondevice]") {
 GIVEN("A sensirion object constructed with a mock I2C device and 8-bit crc param") {
  using Command = PF::Driver::I2C::SFM3019::Command;
  static constexpr PF::HAL::CRC8Parameters crc_params = {0x31, 0xff, false, false, 0x00};

  PF::HAL::MockI2CDevice mock_device;
  PF::HAL::SoftCRC8 crc8c{crc_params};
  constexpr size_t buffer_size = 254UL;
  PF::Util::ByteVector<buffer_size> input_buffer;
  size_t count = buffer_size;

  PF::Driver::I2C::SensirionDevice sensirion_device{mock_device, crc8c};

  WHEN("Internal buffer of mock device is partially filled with data and command is given to the sensirion device") {
   auto input_data = PF::Util::make_array<uint8_t>(0x01, 0x02, 0x03, 0x04, 0x05);
   mock_device.write(input_data.data(), input_data.size());

   auto second_write = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_o2));
   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);
   THEN("Command data overwrites previous data") {
    auto expected = std::string("\\x36\\x03");
    REQUIRE(read_buffer == expected); // 254 null bytes instead
   }
  }

  WHEN("Internal buffer of mock device is completely filled with data and command is given to the sensirion device") {
   int counter = 0;
   uint8_t command = 0x01;
   do
   {
    mock_device.write(&command, buffer_size);
    counter++;
   } while (counter < 51);
   
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_o2));
   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }
   THEN("The command overwrites internal buffer of mock device") {
    auto expected = std::string("\\x36\\x03");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("2 different commands are given as input to sensirion device") {
   auto first_status = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_air));
   THEN("The write method reports ok status") {
    REQUIRE(first_status == PF::I2CDeviceStatus::ok);
   }
   auto second_status = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_o2));
   THEN("The write method reports ok status") {
    REQUIRE(second_status == PF::I2CDeviceStatus::ok);
   }
   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);   
   THEN("The get_write method reports the buffer from the second command") {
    auto expected = std::string("\\x36\\x03");
    REQUIRE(read_buffer == expected); // 254 null bytes instead
   }
  }

  WHEN("start_measure_air is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_air));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x08");
    REQUIRE(read_buffer == expected);
   }
  }
  
  WHEN("start_measure_02 is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_o2));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   size_t count = buffer_size;
   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x03");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("start_measure_mixture is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::start_measure_mixture));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x32");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("update_concentration is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::update_concentration));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\xE1\\x7D");
    REQUIRE(read_buffer == expected);
   }
  }
  
  WHEN("reset_i2c_addr_ptr is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::reset_i2c_addr_ptr));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\xE0\\x00");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("stop_measure is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::stop_measure));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x3F\\xF9");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("start_measure_o2 is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::set_averaging));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x6A");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("set_averaging is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::set_averaging));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x6A");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("read_conversion is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::read_conversion));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x61");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("reset is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::reset));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x00\\x06");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("sleep is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::sleep));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x36\\x77");
    REQUIRE(read_buffer == expected);
   }
  }

  WHEN("read_product_id is given as input command to sensirion device") {
   auto status = sensirion_device.write(static_cast<uint16_t>(Command::read_product_id));
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\xE1\\x02");
    REQUIRE(read_buffer == expected);
   }
  }
 }
}

SCENARIO("SensirionDevice:: Sensirion device behaves correctly when a uint8_t command is given as input", "[sensiriondevice]") {
 GIVEN("A sensirion object constructed with a mock I2C device and 8-bit crc param") {
  static constexpr PF::HAL::CRC8Parameters crc_params = {0x31, 0xff, false, false, 0x00};

  PF::HAL::MockI2CDevice mock_device;
  PF::HAL::SoftCRC8 crc8c{crc_params};
  constexpr size_t buffer_size = 254UL;
  PF::Util::ByteVector<buffer_size> input_buffer;
  size_t count = buffer_size;

  PF::Driver::I2C::SensirionDevice sensirion_device{mock_device, crc8c};
  WHEN("Internal buffer of mock device is partially filled with data and command is given to the sensirion device") {
   auto input_data = PF::Util::make_array<uint8_t>(0x01, 0x02, 0x03, 0x04, 0x05);
   mock_device.write(input_data.data(), input_data.size());

   auto status = sensirion_device.write(input_data[1]);

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);
   THEN("Command data overwrites previous data") {
    auto expected = std::string("\\x02");
    REQUIRE(read_buffer == expected); // 254 null bytes instead
   }
  }

  WHEN("Internal buffer of mock device is completely filled with data and command is given to the sensirion device") {
   int counter = 0;
   uint8_t command = 0x01;
   do
   {
    mock_device.write(&command, buffer_size);
    counter++;
   } while (counter < 51);
   
   auto status = sensirion_device.write(command);
   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);
   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }
   THEN("The command overwrites internal buffer of mock device") {
    auto expected = std::string("\\x01");
    REQUIRE(read_buffer == expected);
   }
  }  

  WHEN("A null byte is written to the sensirion device") {
   uint8_t command = 0x00;
   auto status = sensirion_device.write(command);

   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x00");
    REQUIRE(read_buffer == expected);
   }   
  }
  
  WHEN("A single non-null byte (0x01) is written to the sensirion device") {
   uint8_t command = 0x01;
   auto status = sensirion_device.write(command);

   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\x01");
    REQUIRE(read_buffer == expected);
   }   
  }

  WHEN("A single non-null byte (0xff) is written to the sensirion device") {
   uint8_t command = 0xff;
   auto status = sensirion_device.write(command);

   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\xFF");
    REQUIRE(read_buffer == expected);
   }   
  }
  
  WHEN("A single non-null byte (0x0a) is written to the sensirion device") {
   uint8_t command = 0xff;
   auto status = sensirion_device.write(command);

   THEN("The write method reports ok status") {
    REQUIRE(status == PF::I2CDeviceStatus::ok);
   }

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string("\\xFF");
    REQUIRE(read_buffer == expected);
   }   
  }
 }
}
SCENARIO("SensirionDevice:: Sensirion device behaves correctly when a uint16_t command and argument is given as input", "[sensiriondevice]") {
 GIVEN("A sensirion object constructed with a mock I2C device and 8-bit crc param") {
  using Command = PF::Driver::I2C::SFM3019::Command;
  static constexpr PF::HAL::CRC8Parameters crc_params = {0x31, 0xff, false, false, 0x00};

  PF::HAL::MockI2CDevice mock_device;
  PF::HAL::SoftCRC8 crc8c{crc_params};
  constexpr size_t buffer_size = 254UL;
  PF::Util::ByteVector<buffer_size> input_buffer;
  size_t count = buffer_size;

  PF::Driver::I2C::SensirionDevice sensirion_device{mock_device, crc8c};

  WHEN("start_measure_air command along with a argument is passed to the sensirion device") {
   uint16_t command = 0x366A;
   auto status =
       sensirion_device.write(static_cast<uint16_t>(Command::start_measure_air), command);

   mock_device.get_write(input_buffer.buffer(), count);
   auto read_buffer = PF::Util::convert_byte_vector_to_hex_string(input_buffer, count);

   THEN("The get_write method reports the buffer written to the mockdevice") {
    auto expected = std::string(R"(\x36\x08\x36\x6A\x09)");
    REQUIRE(read_buffer == expected);
   }  
  }
 }
}