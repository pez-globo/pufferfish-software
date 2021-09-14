/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * SensirionDevice.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of SFM3019  Device
 *
 */
#include "Pufferfish/Driver/I2C/SFM3019/Device.h"

#include <string>

#include "Pufferfish/Driver/I2C/SFM3019/Types.h"
#include "Pufferfish/Driver/I2C/SensirionDevice.h"
#include "Pufferfish/HAL/CRCChecker.h"
#include "Pufferfish/HAL/Mock/I2CDevice.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;
using PF::Util::Containers::convert_byte_vector_to_hex_string;
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when start_measure function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and O2 as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());

    WHEN("The start_measure function is called ") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      THEN("The start_measure function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of start measure O2 command byte '0x3603'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x36\\x03");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock  I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device ,  global I2C device of write buffer "
      "'0x01' and air as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    WHEN("The start_measure function is called ") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      THEN("The start_measure function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of start measure air command byte '0x3608'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x36\\x08");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty ") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty ") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and mixture as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The start_measure function is called ") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      THEN("The start_measure function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of start measure mixture command byte '0x3632'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x36\\x32");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when stop_measure function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and O2 as gas "
      "parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The stop_measure function is called ") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      auto status = device.stop_measure();
      THEN("The stop_measure function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of stop measure O2 command byte '0x3FF9'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2C device of write buffer "
      "'0x01',and air as gas "
      "parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The stop_measure function is called ") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      THEN("The stop_measure function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of stop measure air command byte '0x3FF9'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and mixture as gas "
      "parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The stop_measure function is called ") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      THEN("The stop_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of stop measure mixture command byte '0x3FF9'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when set_averaging function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2c device  of write buffer "
      "'0x01'  and O2 as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());

    WHEN("The set_averaging function is called wirh averaging_window 0 as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      THEN("The set_averaging function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging command byte '0x366A' followed by "
          "'0x000081'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("The set_averaging function is called wirh averaging_window 128 as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      THEN("The set_averaging function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2c device of write buffer "
      "'0x01'  and air as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The set_averaging function is called wirh averaging_window 0 as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      THEN("The set_averaging function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x000081'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("The set_averaging function is called wirh averaging_window 128 as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      THEN("The set_averaging function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2c device of write buffer "
      "'0x01'  and mixture as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The set_averaging function is called wirh averaging_window 0 as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      THEN("The set_averaging function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x000081'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("The set_averaging function is called wirh averaging_window 128 as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB") {
        const auto* expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged ") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when request_conversion_factors function is "
    "called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and O2 as gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The request_conversion_factors function is called") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.request_conversion_factors();
      THEN("The request_conversion_factors function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of request_conversion_factor O2 command byte "
          "'0x3661 followed by '0x36033A'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x61\x36\x03\x3A)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and air as gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The request_conversion_factors function is called ") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.request_conversion_factors();
      THEN("The request_conversion_factors function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of request_conversion_factor air command byte "
          "'0x3661 followed by '0x3608D0'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x61\x36\x08\xD0)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and mixture as gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());
    WHEN("The request_conversion_factors function is called") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.request_conversion_factors();
      THEN("The request_conversion_factors returns ok") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of request_conversion_factor mixture command byte "
          "'0x3661 followed by '0x3632CE'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected = R"(\x36\x61\x36\x32\xCE)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_conversion_factors function is "
    "called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device having read buffer 0x[78 33 1B 4c 00 bc "
      "00 00 81], global I2C device both having write buffer '0x01' and air as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0xae, 0xdc, 0x6a, 0x00, 0x00, 0x81, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion_factors;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data.data(), input_data.size());
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto write_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(write_data.data(), write_data.size());
    mock_device.write(write_data.data(), write_data.size());

    WHEN(
        "The read_conversion_factors function is called with a default-initialized "
        "ConversionFactors "
        "output parameter and with the read buffer of mock I2C device consisting 0x[ae dc 6a 00 00 "
        "81 00 00 81]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::ConversionFactors conversion;
      auto status_conversion = device.read_conversion_factors(conversion);
      THEN("The read_conversion_factors return ok status") {
        REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);
      }
      THEN("The output scale factor is 0") { REQUIRE(conversion_factors.scale_factor == 0); }
      THEN("The output offset is 0") { REQUIRE(conversion_factors.offset == 0); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }

    WHEN(
        "The read_conversion_factors function is called with a default-initialized "
        "ConversionFactors "
        "output parameter and with the read buffer of mock I2C device consisting 0x[ae dc 6a 00 00 "
        "81 00 00 81], corresponding to scale factor 10 and offset of -5") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::ConversionFactors conversion;
      conversion_factors.scale_factor = 10;
      conversion_factors.offset = -5;
      auto status_conversion = device.read_conversion_factors(conversion);
      THEN("The read_conversion_factors return ok status") {
        REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);
      }
      THEN("The output scale factor is 10") { REQUIRE(conversion_factors.scale_factor == 10); }
      THEN("The output offset is -5") { REQUIRE(conversion_factors.offset == -5); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_conversion_factors function is called with a default-initialized "
        "ConversionFactors "
        "output parameter and with the read buffer of mock I2C device consisting 0x[ae dc 6a 00 00 "
        "81 00 00 81], corresponding to scale factor 170 and offset of -24576") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::ConversionFactors conversion;
      conversion_factors.scale_factor = 170;
      conversion_factors.offset = -24576;
      auto status_conversion = device.read_conversion_factors(conversion);
      THEN("The read_conversion_factors returns ok status") {
        REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);
      }
      THEN("The output scale factor is 170") { REQUIRE(conversion_factors.scale_factor == 170); }
      THEN("The output offset is -24576") { REQUIRE(conversion_factors.offset == -24576); }

      THEN("The mock_device I2C's write buffer remains unchanged") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when request_product_id called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device of write buffer '0x01',, global I2C "
      "device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(input_data.data(), input_data.size());

    WHEN("The request_product_id function is called") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      auto status = device.request_product_id();
      THEN("The request_product_id function returns ok") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock_device I2C's write buffer contains command byte '0xE102'") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_global = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected_gl = std::string("\\x01");
        REQUIRE(read_global == expected_gl);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_product_id function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device of write buffer "
      "'0x01' and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto write_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    global_device.write(write_data.data(), write_data.size());
    mock_device.write(write_data.data(), write_data.size());

    WHEN("Product id is passed as an output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      size_t count = buffer_size;
      auto input_data = PF::Util::Containers::make_array<uint8_t>(
          0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
      mock_device.add_read(input_data.data(), input_data.size());
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(read_buffer.data(), read_buffer.size());
      uint32_t product_id = 0;
      auto status = device.read_product_id(product_id);
      THEN("The read_product_id function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The product id is 0x78334c00") { REQUIRE(product_id == 0x78334c00); }
      THEN("The mock_device I2C's write buffer remains unchanged ") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when read_sample function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device with read buffer 0x[ae dc 6a], global "
      "I2C device, having write buffer '0x01' and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    PF::Driver::I2C::SFM3019::Sample sample{};
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0xae, 0xdc, 0x6a);
    mock_device.add_read(input_data.data(), input_data.size());
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    WHEN(
        "The read_sample function is called with  scale factor of 170 and an offset of  -24576  as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[ae, dc, 6a]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 170;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns -20772") { REQUIRE(sample.raw_flow == -20772); }
      THEN("Output parameter flow returns 22.37647F ") { REQUIRE(sample.flow == 22.37647F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample function is called with  scale factor of 170 and an offset of -14576 as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[ae, dc, 6a]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 170;
      conversion.offset = -14576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns -20772") { REQUIRE(sample.raw_flow == -20772); }
      THEN("Output parameter flow returns -36.44706F") { REQUIRE(sample.flow == -36.44706F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample function is called with  scale factor of 170 and an offset of -100 as "
        "input "
        "parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[ae, dc, 6a]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 170;
      conversion.offset = -100;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns -20772") { REQUIRE(sample.raw_flow == -20772); }
      THEN("Output parameter flow returns -121.6F") { REQUIRE(sample.flow == -121.6F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample function is called with  scale factor of 100 and an offset of -24576  as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[ae, dc, 6a]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 100;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns -20772") { REQUIRE(sample.raw_flow == -20772); }
      THEN("Output parameter flow returns 38.04F") { REQUIRE(sample.flow == 38.04F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample function is called with  scale factor of 50 and an offset of  -24576  as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[ae, dc, 6a]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 50;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns -20772") { REQUIRE(sample.raw_flow == -20772); }
      THEN("Output parameter flow returns 76.08F") { REQUIRE(sample.flow == 76.08F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample function is called with  scale factor of 200 and an offset of  -24576  as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[ae, dc, 6a]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 200;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns -20772") { REQUIRE(sample.raw_flow == -20772); }
      THEN("Output parameter flow returns 19.02F ") { REQUIRE(sample.flow == 19.02F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_sample function is called with "
    "'x1535a8' buffer") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device with read buffer 0x[15 35 a8], global "
      "I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    PF::Driver::I2C::SFM3019::Sample sample{};
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data.data(), input_data.size());
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    WHEN(
        "The read_sample function is called with  scale factor of 170 and an offset of  -24576  as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[15, 35, a8]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 170;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns 5429") { REQUIRE(sample.raw_flow == 5429); }
      THEN("Output parameter flow returns 176.5F ") { REQUIRE(sample.flow == 176.5F); }
      THEN("The mock_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_sample function is called with "
    "different buffer") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device with read buffer 0x[74 6f ac], global "
      "I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    PF::Util::Containers::ByteVector<buffer_size> global_buffer;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    PF::Driver::I2C::SFM3019::Sample sample{};
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x74, 0x6f, 0xac);
    mock_device.add_read(input_data.data(), input_data.size());
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    WHEN(
        "The read_sample function is called with  scale factor of 170 and an offset of  -24576  as "
        "input parameters, and a Sample object "
        "initialized to default as the output parameter where mock I2C's read buffer is set to "
        "0x[74, 6f, ac]") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      conversion.scale_factor = 170;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Output parameter raw_flow returns 29807") { REQUIRE(sample.raw_flow == 29807); }
      THEN("Output parameter flow returns 319.89999F ") { REQUIRE(sample.flow == 319.89999F); }
      THEN("The mock_device I2C's write buffer consists '0x01' initially given") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when reset function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device of write buffer '0x01',, global I2C "
      "device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
    mock_device.write(input_data.data(), input_data.size());

    WHEN("The reset function is called") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      PF::Util::Containers::ByteVector<buffer_size> global_buffer;
      auto status = device.reset();
      THEN("The reset function returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock_device I2C's write buffer remains unchaned") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer is empty") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'x06'") {
        global_device.get_write(global_buffer.buffer(), count);
        auto read_global = convert_byte_vector_to_hex_string(global_buffer, count);
        auto expected_gl = std::string("\\x06");
        REQUIRE(read_global == expected_gl);  // 254 null bytes instead
      }
    }
  }
}
