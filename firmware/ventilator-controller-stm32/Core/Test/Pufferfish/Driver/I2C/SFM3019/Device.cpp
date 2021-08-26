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
#include "Pufferfish/Util/Endian.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;
using PF::Util::Containers::convert_byte_vector_to_hex_string;
SCENARIO(
    "SFM3019 Device:: SFM3019 device behaves correctly when start_measure function is called") {
  GIVEN("A SFM3019 device constructed with a mock I2C device global I2C device and gas parameter") {
    static constexpr PF::HAL::CRC8Parameters crc_params = {0x31, 0xff, false, false, 0x00};
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::HAL::SoftCRC8 crc8c{crc_params};
    WHEN("GasType O2 is passed as input parameter to device ") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of start measure O2 command byte '0x3603'") {
        auto expected = std::string("\\x36\\x03");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType gas is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::air};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.start_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The start_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of start measure air command byte '0x3608'") {
        auto expected = std::string("\\x36\\x08");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType mixture is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::mixture};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.start_measure();
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of stop measure O2 command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType air is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::air};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.stop_measure();
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The stop_measure returns ok") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The mock I2C's write buffer consists of stop measure air command byte '0x3FF9'") {
        auto expected = std::string("\\x3F\\xF9");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
    WHEN("GasType mixture is passed as input parameter to device") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::mixture};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.stop_measure();
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.set_averaging(0);
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.set_averaging(128);
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::air};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.set_averaging(0);
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::air};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.set_averaging(128);
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::mixture};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.set_averaging(0);
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::mixture};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.set_averaging(128);
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.request_conversion_factors();
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::air};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.request_conversion_factors();
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
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::mixture};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.request_conversion_factors();
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
    WHEN("GasType O2 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      auto status = device_.reset();
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
    WHEN("GasType O2 is passed as input parameter") {
      PF::Driver::I2C::SFM3019::Device device_{
          mock_device, global_device, PF::Driver::I2C::SFM3019::GasType::o2};
      constexpr size_t buffer_size = 254UL;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      size_t count = buffer_size;
      uint32_t re = 0;
      uint32_t& product_number = re;
      device_.read_product_id(product_number);
      mock_device.get_write(input_buffer.buffer(), count);
      auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
      THEN("The mock I2C's write buffer consists of read_product_id command byte '0xE02'") {
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
// SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when read_conversion_factors function
// is called"){
//   GIVEN("A SFM3019 device constructed with a mock I2C device, global I2C device and gas
//   parameter"){
//     PF::HAL::Mock::I2CDevice mock_device;
//     PF::HAL::Mock::I2CDevice global_device;
//     constexpr size_t buffer_size = 254UL;
//     static constexpr PF::HAL::CRC8Parameters crc_params = {0x31, 0xff, false, false, 0x00};
//     PF::HAL::SoftCRC8 crc8c{crc_params};
//     // auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02, 0x03, 0x04, 0x05);
//     // auto status_write = mock_device.write(input_data.data(), input_data.size());
//     // REQUIRE(status_write == PF::I2CDeviceStatus::ok);
//     // auto status_write1 = global_device.write(input_data.data(),input_data.size());
//     // REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
//     WHEN("GasType O2 is passed as input parameter"){
//       PF::Driver::I2C::SFM3019::Device
//       device_{mock_device,global_device,PF::Driver::I2C::SFM3019::GasType::o2}; constexpr size_t
//       buffer_size = 254UL; PF::Driver::I2C::SensirionDevice sensirion_device{mock_device, crc8c};
//       int counter = 0;
//       uint8_t command = 0x01;
//       do
//       {
//         mock_device.write(&command, buffer_size);
//         counter++;
//       } while (counter < 51);
//       std::array<uint8_t, sizeof(uint16_t)> buffer{};
//       auto status_read = sensirion_device.read(buffer);
//       //REQUIRE(status_read == PF::I2CDeviceStatus::ok);

//       PF::Driver::I2C::SFM3019::ConversionFactors conversion;
//       auto status_conversion = device_.read_conversion_factors(conversion);
//       THEN("Command data overwrites previous data") {
//         REQUIRE(status_conversion == PF::I2CDeviceStatus::ok);

//       }
//     }
//   }
// }

// SCENARIO("SFM3019 Device:: SFM3019 device behaves correctly when read_sample function is
// called"){
//   GIVEN("A SFM3019 device constructed with a mock I2C device, global I2C device and gas
//   parameter"){
//     PF::HAL::Mock::I2CDevice mock_device;
//     PF::HAL::Mock::I2CDevice global_device;
//     constexpr size_t buffer_size = 254UL;
//     PF::Util::Containers::ByteVector<buffer_size> input_buffer;
//     size_t count = buffer_size;
//     static constexpr PF::HAL::CRC8Parameters crc_params = {0x31, 0xff, false, false, 0x00};
//     PF::HAL::SoftCRC8 crc8c{crc_params};
//     auto input_data = PF::Util::Containers::make_array<uint8_t>();
//     // mock_device.write(input_data.data(), input_data.size());
//     // global_device.write(input_data.data(), input_data.size());
//     PF::Driver::I2C::SensirionDevice sensirion_device{mock_device, crc8c};
//     std::array<uint8_t, sizeof(uint16_t)> buffer{};
//     auto second_write =
//     sensirion_device.write(static_cast<uint16_t>(PF::Driver::I2C::SFM3019::Command::start_measure_o2));
//     REQUIRE(second_write == PF::I2CDeviceStatus::ok);
//     auto status = sensirion_device.read(buffer);
//     WHEN("GasType O2 is passed as input parameter"){
//       PF::Driver::I2C::SFM3019::Device
//       device_{mock_device,global_device,PF::Driver::I2C::SFM3019::GasType::o2}; constexpr size_t
//       buffer_size = 254UL; PF::Driver::I2C::SFM3019::Sample sample;
//       mock_device.get_write(input_buffer.buffer(), count);
//       auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
//       auto expected = std::string("\\x36\\x03");
//      // REQUIRE(read_buffer == expected); // 254 null bytes instead
// //      sample.raw_flow = 0.12;
//       int16_t scale_factor = 170;
//       int16_t offset = -24576;
//       sample.flow = static_cast<float>(sample.raw_flow - offset) /
//       static_cast<float>(scale_factor);
// //       auto status1 = device_.read_sample(sample,scale_factor,offset);
//        THEN("Command data overwrites previous data") {
// //         REQUIRE(status1 == PF::I2CDeviceStatus::ok);
// // //        REQUIRE(sample.raw_flow == 1);
// //        // REQUIRE(result1 == 0);
// //        REQUIRE(status == PF::I2CDeviceStatus::ok);
//       }
//     }
//   }
// }
