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

#include <iostream>
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
      "A SFM3019 device constructed with a mock I2C device, global I2C device and O2 as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The start_measure method called as default mock I2C device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of start measure O2 command byte '0x3603'") {
        auto expected = std::string("\\x36\\x03");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'x03603'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x36\\x03");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2C device and air as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The start_measure method called as default mock I2C device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of start measure air command byte '0x3608'") {
        auto expected = std::string("\\x36\\x08");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'x03608'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x36\\x08");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2C device and mixture as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The start_measure method called as default mock I2C device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of start measure mixture command byte '0x3632'") {
        auto expected = std::string("\\x36\\x32");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'0x3632'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x36\\x32");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when stop_measure function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2C device and O2 as gas "
      "parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The stop_measure method called as default mock I2C device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of stop measure O2 command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'0x3FF9'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2C device and air as gas "
      "parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The stop_measure method called as default mock I2C device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock I2C's write buffer consists of stop measure air command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'0x03FF9'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2C device and mixture as gas "
      "parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The stop_measure method called on default mock I2C device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of stop measure mixture command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'0x3FF9'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when set_averaging function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2c device  and O2 as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The averaging_window 0 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging command byte '0x366A' followed by "
          "'0x000081'") {
        auto expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN(
          "The global_device I2C's write buffer consists '0x366A' followed by "
          "'0x000081'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("The averaging_window 128 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB'") {
        auto expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN(
          "The global_device I2C's write buffer consists'0x366A' followed "
          "by '0x0080FB'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2c device  and air as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The averaging_window 0 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x000081'") {
        auto expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN(
          "The global_device I2C's write buffer consists '0x366A' followed "
          "by '0x000081'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("The averaging_window 128 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB'") {
        auto expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN(
          "The global_device I2C's write buffer consists '0x366A' followed "
          "by '0x0080FB'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2c device  and mixture as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("The averaging_window 0 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x000081'") {
        auto expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN(
          "The global_device I2C's write buffer consists '0x366A' followed "
          "by '0x000081'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x6A\x00\x00\x81)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("The averaging_window 128 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB") {
        auto expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists '0x366A' followed by '0x0080FB'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x6A\x00\x80\xFB)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when request_conversion_factors function is "
    "called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device , global I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN(
        "The request_conversion_factors method is called with default mock I2C device and on gas "
        "type O2 as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.request_conversion_factors();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The request_conversion_factors method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of request_conversion_factor O2 command byte "
          "'0x3661 followed by '0x36033A'") {
        auto expected = R"(\x36\x61\x36\x03\x3A)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'0x3661 followed by '0x36033A'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x61\x36\x03\x3A)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The request_conversion_factors method is called with default mock I2C device and on gas "
        "type air as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.request_conversion_factors();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The request_conversion_factors method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of request_conversion_factor air command byte "
          "'0x3661 followed by '0x3608D0'") {
        auto expected = R"(\x36\x61\x36\x08\xD0)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'0x3661 followed by '0x3608D0'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x61\x36\x08\xD0)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The request_conversion_factors method is called with default mock I2C device and on gas "
        "type mixture as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.request_conversion_factors();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The request_conversion_factors returns ok") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The mock I2C's write buffer consists of request_conversion_factor mixture command byte "
          "'0x3661 followed by '0x3632CE'") {
        auto expected = R"(\x36\x61\x36\x32\xCE)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'x03603'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = R"(\x36\x61\x36\x32\xCE)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_conversion_factors function is "
    "called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2C device and air as gas "
      "parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion_factors;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(
        0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
    mock_device.add_read(input_data.data(), input_data.size());
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    WHEN(
        "The read_conversion_factors method with the mock I2C device's read buffer set to "
        "0x78331B4C00bc000081"
        "corresponding to scale factor 10, offset -5 and a default-initialized ConversionFactors "
        "object as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::ConversionFactors conversion;
      conversion_factors.scale_factor = 10;
      conversion_factors.offset = -5;
      auto status_conversion = device.read_conversion_factors(conversion);
      THEN("The read_conversion_factors return ok status") {
        REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method for global_device returns no new data") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_conversion_factors method with the mock I2C device's read buffer set to "
        "0x78331B4C00bc000081"
        "corresponding to scale factor 10, offset -5 and a default-initialized ConversionFactors "
        "object as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::ConversionFactors conversion;
      conversion_factors.scale_factor = 170;
      conversion_factors.offset = -24576;
      auto status_conversion = device.read_conversion_factors(conversion);
      THEN("Command data overwrites previous data") {
        REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);
      }
      THEN("The mock_device I2C's write buffer consists of expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_product_id function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    WHEN("Product id 0x04020611 is passed as an input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto input_data = PF::Util::Containers::make_array<uint8_t>(
          0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
      mock_device.add_read(input_data.data(), input_data.size());
      uint32_t product_id = 0x04020611;
      auto status = device.read_product_id(product_id);
      THEN("The read_product_id method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The read method returns no new data status for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'x03603'") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when read_sample function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    PF::Driver::I2C::SFM3019::Sample sample;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(
        0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
    WHEN(
        "The read_sample method with the mock I2C device's read buffer set to 0x78331B4c00bc000081"
        " a scale factor of 170 and an offset of  -24576  as input parameters, and a Sample object "
        "initialized to default as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      mock_device.add_read(input_data.data(), input_data.size());
      conversion.scale_factor = 170;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("sample.flow returns 325.57059f ") { REQUIRE(sample.flow == 325.57059F); }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample method with the mock I2C device's read buffer set to 0x78331B4c00bc000081"
        " a scale factor of 170 and an offset of -14576  as input parameters, and a Sample object "
        "initialized to default as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      mock_device.add_read(input_data.data(), input_data.size());
      conversion.scale_factor = 170;
      conversion.offset = -14576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("sample.flow returns 266.74707f") { REQUIRE(sample.flow == 266.74707F); }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample method with the mock I2C device's read buffer set to 0x78331B4c00bc000081"
        " a scale factor of 170 and an offset of -100  as input parameters, and a Sample object "
        "initialized to default as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      mock_device.add_read(input_data.data(), input_data.size());
      conversion.scale_factor = 170;
      conversion.offset = -100;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("sample.flow returns 181.59412f") { REQUIRE(sample.flow == 181.59412F); }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample method with the mock I2C device's read buffer set to 0x78331B4c00bc000081"
        " a scale factor of 100 and an offset of -14576  as input parameters, and a Sample object "
        "initialized to default as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      mock_device.add_read(input_data.data(), input_data.size());
      conversion.scale_factor = 100;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("sample.flow returns 553.46997f") { REQUIRE(sample.flow == 553.46997F); }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample method with the mock I2C device's read buffer set to 0x78331B4c00bc000081"
        " a scale factor of 50 and an offset of -14576  as input parameters, and a Sample object "
        "initialized to default as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      mock_device.add_read(input_data.data(), input_data.size());
      conversion.scale_factor = 50;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("sample.flow returns 1106.93994f") { REQUIRE(sample.flow == 1106.93994F); }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN(
        "The read_sample method with the mock I2C device's read buffer set to 0x78331B4c00bc000081"
        " a scale factor of 200 and an offset of -14576  as input parameters, and a Sample object "
        "initialized to default as the output parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      mock_device.add_read(input_data.data(), input_data.size());
      conversion.scale_factor = 200;
      conversion.offset = -24576;
      auto status = device.read_sample(conversion, sample);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("sample.flow returns 276.73499f ") { REQUIRE(sample.flow == 276.73499F); }
      THEN("The mock_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        mock_device.write(input_data.data(), input_data.size());
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
      THEN("The read method returns no new data status for global_device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists expected data") {
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x01\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when reset function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    WHEN(
        "The reset method is called with default I2C mock device and gas type O2 as input "
        "parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.reset();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The reset function returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The read method returns no new data status for mock device") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The read method returns no new data status for global device") {
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer consists'x06'") {
        global_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\x06");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
