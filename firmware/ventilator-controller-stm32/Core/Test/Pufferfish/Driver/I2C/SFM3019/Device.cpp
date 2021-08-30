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

#include "Pufferfish/Driver/I2C/SFM3019/Types.h"
#include "Pufferfish/Driver/I2C/SensirionDevice.h"
//#include "Pufferfish/Driver/I2C/SFM3019/Types.h"
#include <iostream>
#include <string>

#include "Pufferfish/HAL/CRCChecker.h"
#include "Pufferfish/HAL/Mock/I2CDevice.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;
using PF::Util::Containers::convert_byte_vector_to_hex_string;
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when start_measure function is called") {
  GIVEN("A SFM3019 device constructed with a mock I2C device global I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("GasType O2 is passed as input parameter to device ") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of start measure O2 command byte '0x3603'") {
        auto expected = std::string("\\x36\\x03");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType air is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of start measure air command byte '0x3608'") {
        auto expected = std::string("\\x36\\x08");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType mixture is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of start measure mixture command byte '0x3632'") {
        auto expected = std::string("\\x36\\x32");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when stop_measure function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2C device and gas parameter ") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("GasType O2 is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of stop measure O2 command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType air is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of stop measure air command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType mixture is passed as input parameter to device") {
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
    }
  }
}
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when set_averaging function is called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device global I2c device  and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    WHEN("GasType O2 and averaging_window 0 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The mock I2C's write buffer consists of set_averaging command byte '0x366A' followed by "
          "'0x000081'") {
        auto expected = std::string("\\x36\\x6A\\x00\\x00\\x81");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType O2 and averaging_window 128 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB'") {
        auto expected = std::string("\\x36\\x6A\\x00\\x80\\xFB");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }

    WHEN("GasType air and averaging_window 0 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x000081'") {
        auto expected = std::string("\\x36\\x6A\\x00\\x00\\x81");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType air and averaging_window 128 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB'") {
        auto expected = std::string("\\x36\\x6A\\x00\\x80\\xFB");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }

    WHEN("GasType mixture and averaging_window 0 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(0);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x000081'") {
        auto expected = std::string("\\x36\\x6A\\x00\\x00\\x81");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType mixture and averaging_window 128 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::mixture;
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.set_averaging(128);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The set_averaging returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The mock I2C's write buffer consists of set_averaging  command byte '0x366A' followed "
          "by '0x0080FB") {
        auto expected = std::string("\\x36\\x6A\\x00\\x80\\xFB");
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
    WHEN("GasType O2 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
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
          "The mock I2C's write buffer consists of request_conversion_factor O2 command byte "
          "'0x3661 followed by '0x36033A'") {
        auto expected = std::string("\\x36\\x61\\x36\\x03\\x3A");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType air is passed as input parameter") {
      PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
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
          "The mock I2C's write buffer consists of request_conversion_factor air command byte "
          "'0x3661 followed by '0x3608D0'") {
        auto expected = std::string("\\x36\\x61\\x36\\x08\\xD0");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType mixture is passed as input parameter") {
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
        auto expected = std::string("\\x36\\x61\\x36\\x32\\xCE");
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
    WHEN("GasType O2 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device.reset();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The reset function returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
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
      auto input_data = PF::Util::Containers::make_array<uint8_t>(
          0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
      mock_device.add_read(input_data.data(), input_data.size());
      uint32_t product_id = 0x04020611;
      auto status = device.read_product_id(product_id);
      THEN("The read_product_id returns ok status") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
    }
  }
}

SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when read_conversion_factors function is "
    "called") {
  GIVEN(
      "A SFM3019 device constructed with a mock I2C device, global I2C device and gas parameter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    constexpr size_t buffer_size = 254UL;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion_factors;
    conversion_factors.scale_factor = 10;
    conversion_factors.offset = -5;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(
        0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
    mock_device.add_read(input_data.data(), input_data.size());
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    WHEN("scale_factor is 10 and offset is -5 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::ConversionFactors conversion;
      auto status_conversion = device.read_conversion_factors(conversion);
      THEN("Command data overwrites previous data") {
        REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);
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
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
    auto input_data = PF::Util::Containers::make_array<uint8_t>(
        0x78, 0x33, 0x1B, 0x4c, 0x00, 0xbc, 0x00, 0x00, 0x81);
    WHEN("Scale factor is 170 and offset is -24576 are passed as an input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::Sample sample;
      mock_device.add_read(input_data.data(), input_data.size());
      int16_t scale_factor = 170;
      int16_t offset = -24576;
      auto status = device.read_sample(sample, scale_factor, offset);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Flow returns 325.57059f value") { REQUIRE(sample.flow == 325.57059f); }
    }
    WHEN("Scale factor is 10 and offset is 5 are passed as an input parameter") {
      PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
      PF::Driver::I2C::SFM3019::Sample sample;
      mock_device.add_read(input_data.data(), input_data.size());
      int16_t scale_factor = 10;
      int16_t offset = 5;
      auto status = device.read_sample(sample, scale_factor, offset);
      THEN("The read_sample function returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("Flow returns 3076.6001f value") { REQUIRE(sample.flow == 3076.6001f); }
    }
  }
}
