#include "Pufferfish/HAL/Mock/I2CDevice.h"

#include <string>

#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"
#include "Pufferfish/HAL/Mock/Time.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Endian.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;
SCENARIO("I2C mock device works correctly for read method") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = 54;
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97]bytes, and ok "
        "I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 54); }
      THEN("The read method returns OK status") { REQUIRE(read_status == PF::I2CDeviceStatus::ok); }
      THEN("The read method returns [a0 e8 d4 fd 97] data bytes") {
        REQUIRE(input_read[0] == 0xa0);
        REQUIRE(input_read[1] == 0xe8);
        REQUIRE(input_read[2] == 0xd4);
        REQUIRE(input_read[3] == 0xfd);
        REQUIRE(input_read[4] == 0x97);
      }
    }
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97] bytes, and "
        "read_error I2CDeviceStatus  ") {
      auto status = PF::I2CDeviceStatus::read_error;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 54); }
      THEN("The read method returns read_error status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::read_error);
      }
    }
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97]bytes, and "
        "crc_check_failed I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::crc_check_failed;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 54); }
      THEN("The read method returns crc_check_failed error") {
        REQUIRE(read_status == PF::I2CDeviceStatus::crc_check_failed);
      }
    }
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97]bytes, and "
        "test_failed I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::test_failed;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 54); }
      THEN("The read method returns test_failed error") {
        REQUIRE(read_status == PF::I2CDeviceStatus::test_failed);
      }
    }
  }
}

SCENARIO("Mock I2CDevice works correctly for read method when called five times") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    WHEN(
        "The add_read method is called five times with input parametes,[27 b8] [64 f3] [e1 7c] [d5 "
        "55] [2e 87] bytes and [ok][read_error][invalid_arguments][crc_check_failed][no_new_data] "
        "I2CDeviceStatus") {
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      PF::Util::Containers::ByteVector<buffer_size> input_read4;
      auto add_status = PF::I2CDeviceStatus::ok;
      auto add_status1 = PF::I2CDeviceStatus::read_error;
      auto add_status2 = PF::I2CDeviceStatus::invalid_arguments;
      auto add_status3 = PF::I2CDeviceStatus::crc_check_failed;
      auto add_status4 = PF::I2CDeviceStatus::no_new_data;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status1);
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xe1, 0x7c);
      mock_device.add_read(input_data.data(), input_data.size(), add_status2);
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x55);
      mock_device.add_read(input_data1.data(), input_data1.size(), add_status3);
      auto input_data2 = PF::Util::Containers::make_array<uint8_t>(0x2e, 0x87);
      mock_device.add_read(input_data2.data(), input_data2.size(), add_status4);
      auto read_status = mock_device.read(input_read.buffer(), count);
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      auto read_status2 = mock_device.read(input_read2.buffer(), count);
      auto read_status3 = mock_device.read(input_read3.buffer(), count);
      auto read_status4 = mock_device.read(input_read4.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      THEN("The second read method call returns read_error status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::read_error);
      }
      THEN("The third read method call returns invalid_argument status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::invalid_arguments);
      }
      THEN("The fourth read method call returns crc_check_failed status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::crc_check_failed);
      }
      THEN("The fifth read method call returns no_new_data status") {
        REQUIRE(read_status4 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "Mock I2CDevice works correctly for read method when called four times and last add_read "
    "status is ok") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    WHEN(
        "The add_read method is called five times with input parametes,[27 b8] [64 f3] [e1 7c] [d5 "
        "55] bytes and [ok][read_error][invalid_arguments][ok]"
        "I2CDeviceStatus") {
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      PF::Util::Containers::ByteVector<buffer_size> input_read4;
      auto add_status = PF::I2CDeviceStatus::ok;
      auto add_status1 = PF::I2CDeviceStatus::read_error;
      auto add_status2 = PF::I2CDeviceStatus::invalid_arguments;
      auto add_status3 = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status1);
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xe1, 0x7c);
      mock_device.add_read(input_data.data(), input_data.size(), add_status2);
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x55);
      mock_device.add_read(input_data1.data(), input_data1.size(), add_status3);
      auto read_status = mock_device.read(input_read.buffer(), count);
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      auto read_status2 = mock_device.read(input_read2.buffer(), count);
      auto read_status3 = mock_device.read(input_read3.buffer(), count);
      auto read_status4 = mock_device.read(input_read4.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      THEN("The second read method call returns read_error status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::read_error);
      }
      THEN("The third read method call returns invalid_argument status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::invalid_arguments);
      }
      THEN("The fourth read method call returns crc_check_failed status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [27 b8]bytes") {
        REQUIRE(input_read3[0] == 0x64);
        REQUIRE(input_read3[1] == 0xf3);
      }

      THEN("The fifth read method call returns no_new_data status") {
        REQUIRE(read_status4 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO("Mock I2CDevice works correctly for read method when called four times") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    WHEN(
        "The add_read method is called five times with input parametes,[27 b8] [64 f3] [e1 7c] "
        "bytes and [ok][read_error][invalid_arguments][crc_check_failed][no_new_data] "
        "I2CDeviceStatus") {
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      auto add_status = PF::I2CDeviceStatus::ok;
      auto add_status1 = PF::I2CDeviceStatus::ok;
      auto add_status2 = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status1);
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xe1, 0x7c);
      mock_device.add_read(input_data.data(), input_data.size(), add_status2);
      auto read_status = mock_device.read(input_read.buffer(), count);
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      auto read_status2 = mock_device.read(input_read2.buffer(), count);
      auto read_status3 = mock_device.read(input_read3.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      THEN("The second read method call returns ok status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [64 f3]bytes") {
        REQUIRE(input_read1[0] == 0x64);
        REQUIRE(input_read1[1] == 0xf3);
      }

      THEN("The third read method call returns ok status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [e1 7c]bytes") {
        REQUIRE(input_read2[0] == 0xe1);
        REQUIRE(input_read2[1] == 0x7c);
      }
      THEN("The fourth read method call returns no_new_data status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO("Mock I2CDevice works correctly for read method when called two times") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    WHEN(
        "The add_read method is called five times with input parametes,[27 b8] [64 f3] [e1 7c] [d5 "
        "55] bytes and [ok][ok][ok][ok][ok] "
        "I2CDeviceStatus") {
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      auto add_status = PF::I2CDeviceStatus::ok;
      auto add_status1 = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status1);
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The second read method call returns ok status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, it returns [64 f3]bytes") {
        REQUIRE(input_read1[0] == 0x64);
        REQUIRE(input_read1[1] == 0xf3);
      }
      auto read_status2 = mock_device.read(input_read2.buffer(), count);
      THEN("The third read method call returns no_new_data status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "I2C mock device works correctly for read method when add_read method has innput parameter 54 "
    "bytes ") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 54UL;
    size_t count = 50;
    WHEN(
        "The add_read method is called with input parameters, 54 bytes [63 89 9a a9 27 54 0e 5e 90 "
        "c2 15 21 45 e1 e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 "
        "eb e0 73 a3 09 9a 4c 13 e3 ce 3f 9b 99 07 be ] and ok "
        "I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
          0x63,
          0x89,
          0x9a,
          0xa9,
          0x27,
          0x54,
          0x0e,
          0x5e,
          0x90,
          0xc2,
          0x15,
          0x21,
          0x45,
          0xe1,
          0xe1,
          0xbb,
          0x99,
          0x55,
          0x73,
          0xa6,
          0xa4,
          0x04,
          0x27,
          0xae,
          0x2d,
          0x1a,
          0xf0,
          0x43,
          0xd6,
          0x76,
          0xd8,
          0xb6,
          0xce,
          0x30,
          0xfc,
          0x89,
          0xe6,
          0xf2,
          0xe9,
          0xeb,
          0xe0,
          0x73,
          0xa3,
          0x09,
          0x9a,
          0x4c,
          0x13,
          0xe3,
          0xce,
          0x3f,
          0x9b,
          0x99,
          0x07,
          0xbe);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("Count returns 50 value") { REQUIRE(count == 50); }
      THEN("The read method returns OK status") { REQUIRE(read_status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The read method returns only 50 data bytes [63 89 9a a9 27 54 0e 5e 90 c2 15 21 45 e1 "
          "e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 eb e0 73 a3 "
          "09 9a 4c 13 e3 ce 3f]") {
        auto read_buffer =
            PF::Util::Containers::convert_byte_vector_to_hex_string(input_read, count);
        const auto* expected =
            R"(\x63\x89\x9A\xA9\x27\x54\x0E\x5E\x90\xC2\x15\x21\x45\xE1\xE1\xBB\x99\x55\x73\xA6\xA4\x04\x27\xAE\x2D\x1A\xF0\x43\xD6\x76\xD8\xB6\xCE\x30\xFC\x89\xE6\xF2\xE9\xEB\xE0\x73\xA3\x09\x9A\x4C\x13\xE3\xCE\x3F)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}
SCENARIO(
    "I2C mock device works correctly for read method when in add_read method buffer size is "
    "greater than count") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 60UL;
    size_t count = 50;
    WHEN(
        "The add_read method is called with input parameters, 60 bytes [63 89 9a a9 27 54 0e 5e 90 "
        "c2 15 21 45 e1 e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 "
        "eb e0 73 a3 09 9a 4c 13 e3 ce 3f 9b 99 07 be be 5b 1d 7a 4a 8c ], 25 data bytes [eb 65 a1 "
        "62 93 13 33 23 ff 97 ba 63 f2 46 46 dd 1a 64 a2 c1 50 04 d3 09 31] and ok "
        "I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
          0x63,
          0x89,
          0x9a,
          0xa9,
          0x27,
          0x54,
          0x0e,
          0x5e,
          0x90,
          0xc2,
          0x15,
          0x21,
          0x45,
          0xe1,
          0xe1,
          0xbb,
          0x99,
          0x55,
          0x73,
          0xa6,
          0xa4,
          0x04,
          0x27,
          0xae,
          0x2d,
          0x1a,
          0xf0,
          0x43,
          0xd6,
          0x76,
          0xd8,
          0xb6,
          0xce,
          0x30,
          0xfc,
          0x89,
          0xe6,
          0xf2,
          0xe9,
          0xeb,
          0xe0,
          0x73,
          0xa3,
          0x09,
          0x9a,
          0x4c,
          0x13,
          0xe3,
          0xce,
          0x3f,
          0x9b,
          0x99,
          0x07,
          0xbe,
          0xbe,
          0x5b,
          0x1d,
          0x7a,
          0x4a,
          0x8c);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      REQUIRE(count == 50);
      THEN("The read method returns OK status") { REQUIRE(read_status == PF::I2CDeviceStatus::ok); }
      THEN(
          "The read method returns only 50 data bytes [63 89 9a a9 27 54 0e 5e 90 c2 15 21 45 e1 "
          "e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 eb e0 73 a3 "
          "09 9a 4c 13 e3 ce 3f]") {
        auto buffer = PF::Util::Containers::convert_byte_vector_to_hex_string(input_read, count);
        const auto* expected =
            R"(\x63\x89\x9A\xA9\x27\x54\x0E\x5E\x90\xC2\x15\x21\x45\xE1\xE1\xBB\x99\x55\x73\xA6\xA4\x04\x27\xAE\x2D\x1A\xF0\x43\xD6\x76\xD8\xB6\xCE\x30\xFC\x89\xE6\xF2\xE9\xEB\xE0\x73\xA3\x09\x9A\x4C\x13\xE3\xCE\x3F)";
        REQUIRE(buffer == expected);  // 254 null bytes instead
      }
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
          0xeb,
          0x65,
          0xa1,
          0x62,
          0x93,
          0x13,
          0x33,
          0x23,
          0xff,
          0x97,
          0xba,
          0x63,
          0xf2,
          0x46,
          0x46,
          0xdd,
          0x1a,
          0x64,
          0xa2,
          0xc1,
          0x50,
          0x04,
          0xd3,
          0x09,
          0x31);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The read method returns OK status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The read method returns expected 25 data bytes [eb 65 a1 62 93 13 33 23 ff 97 ba 63 f2 "
          "46 46 dd 1a 64 a2 c1 50 04 d3 09 31]") {
        auto read_buffer1 =
            PF::Util::Containers::convert_byte_vector_to_hex_string(input_read1, count);
        const auto* expected =
            R"(\xEB\x65\xA1\x62\x93\x13\x33\x23\xFF\x97\xBA\x63\xF2\x46\x46\xDD\x1A\x64\xA2\xC1\x50\x04\xD3\x09\x31\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00)";
        REQUIRE(read_buffer1 == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("Mock I2CDevice works correctly for write method") {
  GIVEN("A freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    WHEN(
        "The write method is called with input parameter, data bytes[d5 1a 7a bc 24] and Ok "
        "I2CDeviceStatus") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The get_write method returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write method returns [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
    }
    WHEN(
        "The write method is called with input parameter data [d5 1a 7a bc 24]bytes and  "
        "write_error I2CDeviceStatus") {
      auto write_status = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The get_write method returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write method returns [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
    }
    WHEN(
        "The write method is called with input parameter data [d5 1a 7a bc 24]bytes and "
        "not_supported I2CDeviceStatus") {
      auto write_status = PF::I2CDeviceStatus::not_supported;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The get_write method returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write method returns [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
    }

    WHEN(
        "The write method is called with input parameters, data [d5 1a 7a bc 24]bytes and "
        "no_new_data I2CDeviceStatus") {
      auto write_status = PF::I2CDeviceStatus::no_new_data;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("The get_write method returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write method returns [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
    }
  }
}

SCENARIO("Mock device works correctly when write method is called multiple times") {
  GIVEN("Freshly constructed mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;

    WHEN(
        "The write method is called five times with buffer bytes [db fe] [79 4b] [cd 93] [5a 56] "
        "[22 19]and I2CDevice status as [Ok] [write_error] [test_failed] [no_new_data] "
        "[invalid_argument] ") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      mock_device.write(write_buffer.data(), write_buffer.size());
      auto write_status1 = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status1);
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x79, 0x4b);
      mock_device.write(write_buffer1.data(), write_buffer1.size());
      auto write_status2 = PF::I2CDeviceStatus::test_failed;
      mock_device.add_write_status(write_status2);
      auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0xcd, 0x93);
      mock_device.write(write_buffer2.data(), write_buffer2.size());
      auto write_status3 = PF::I2CDeviceStatus::no_new_data;
      mock_device.add_write_status(write_status3);
      auto write_buffer3 = PF::Util::Containers::make_array<uint8_t>(0x5a, 0x56);
      mock_device.write(write_buffer3.data(), write_buffer3.size());
      auto write_status4 = PF::I2CDeviceStatus::invalid_arguments;
      mock_device.add_write_status(write_status4);
      auto write_buffer4 = PF::Util::Containers::make_array<uint8_t>(0x22, 0x19);
      mock_device.write(write_buffer4.data(), write_buffer4.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer3;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer4;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
      auto status_write3 = mock_device.get_write(input_buffer3.buffer(), count);
      auto status_write4 = mock_device.get_write(input_buffer4.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("After the first write call, get_write returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      THEN("After the second write call, get_write returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer1[0] == 0x79);
        REQUIRE(input_buffer1[1] == 0x4b);
      }
      THEN("After the third write call, get_write returns ok status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer2[0] == 0xcd);
        REQUIRE(input_buffer2[1] == 0x93);
      }
      THEN("After the fourth write call, get_write returns ok status") {
        REQUIRE(status_write3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer3[0] == 0x5a);
        REQUIRE(input_buffer3[1] == 0x56);
      }
      THEN("After the fifth write call, get_write returns ok status") {
        REQUIRE(status_write4 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer4[0] == 0x22);
        REQUIRE(input_buffer4[1] == 0x19);
      }
    }
  }
}

SCENARIO("Mock device works correctly when write method is called five times") {
  GIVEN("Freshly constructed mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;

    WHEN(
        "The write method is called five times with buffer bytes [db fe] [79 4b] [cd 93] [5a 56] "
        "[22 19]and I2CDevice status as [Ok] [write_error] [test_failed] [no_new_data] "
        "[invalid_argument] ") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      mock_device.write(write_buffer.data(), write_buffer.size());
      auto write_status1 = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status1);
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x79, 0x4b);
      mock_device.write(write_buffer1.data(), write_buffer1.size());
      auto write_status2 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status2);
      auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0xcd, 0x93);
      mock_device.write(write_buffer2.data(), write_buffer2.size());
      auto write_status3 = PF::I2CDeviceStatus::no_new_data;
      mock_device.add_write_status(write_status3);
      auto write_buffer3 = PF::Util::Containers::make_array<uint8_t>(0x5a, 0x56);
      mock_device.write(write_buffer3.data(), write_buffer3.size());
      auto write_status4 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status4);
      auto write_buffer4 = PF::Util::Containers::make_array<uint8_t>(0x22, 0x19);
      mock_device.write(write_buffer4.data(), write_buffer4.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer3;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer4;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
      auto status_write3 = mock_device.get_write(input_buffer3.buffer(), count);
      auto status_write4 = mock_device.get_write(input_buffer4.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("After the first write call, get_write returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      THEN("After the second write call, get_write returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer1[0] == 0x79);
        REQUIRE(input_buffer1[1] == 0x4b);
      }
      THEN("After the third write call, get_write returns ok status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer2[0] == 0xcd);
        REQUIRE(input_buffer2[1] == 0x93);
      }
      THEN("After the fourth write call, get_write returns ok status") {
        REQUIRE(status_write3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer3[0] == 0x5a);
        REQUIRE(input_buffer3[1] == 0x56);
      }
      THEN("After the fifth write call, get_write returns ok status") {
        REQUIRE(status_write4 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer4[0] == 0x22);
        REQUIRE(input_buffer4[1] == 0x19);
      }
    }
  }
}
SCENARIO("Mock I2CDevice works correctly for write method when unput bytes are 54") {
  GIVEN("A freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 70;
    size_t count = 0;
    WHEN(
        "The write method is called with input parameter, 54 bytes [63 89 9a a9 27 54 0e 5e 90 c2 "
        "15 21 45 e1 e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 eb "
        "e0 73 a3 09 9a 4c 13 e3 ce 3f 9b 99 07 be]bytes and Ok "
        "I2CDeviceStatus") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(
          0x63,
          0x89,
          0x9a,
          0xa9,
          0x27,
          0x54,
          0x0e,
          0x5e,
          0x90,
          0xc2,
          0x15,
          0x21,
          0x45,
          0xe1,
          0xe1,
          0xbb,
          0x99,
          0x55,
          0x73,
          0xa6,
          0xa4,
          0x04,
          0x27,
          0xae,
          0x2d,
          0x1a,
          0xf0,
          0x43,
          0xd6,
          0x76,
          0xd8,
          0xb6,
          0xce,
          0x30,
          0xfc,
          0x89,
          0xe6,
          0xf2,
          0xe9,
          0xeb,
          0xe0,
          0x73,
          0xa3,
          0x09,
          0x9a,
          0x4c,
          0x13,
          0xe3,
          0xce,
          0x3f,
          0x9b,
          0x99,
          0x07,
          0xbe);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The get_write method returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
        REQUIRE(count == 50);
      }
      THEN(
          "The get_write method only returns 50 bytes [63 89 9a a9 27 54 0e 5e 90 c2 15 21 45 e1 "
          "e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 eb e0 73 a3 "
          "09 9a 4c 13 e3 ce 3f] data bytes") {
        auto read_buffer =
            PF::Util::Containers::convert_byte_vector_to_hex_string(input_buffer, count);
        const auto* expected =
            R"(\x63\x89\x9A\xA9\x27\x54\x0E\x5E\x90\xC2\x15\x21\x45\xE1\xE1\xBB\x99\x55\x73\xA6\xA4\x04\x27\xAE\x2D\x1A\xF0\x43\xD6\x76\xD8\xB6\xCE\x30\xFC\x89\xE6\xF2\xE9\xEB\xE0\x73\xA3\x09\x9A\x4C\x13\xE3\xCE\x3F)";
        REQUIRE(read_buffer == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO(
    "I2C mock device works correctly for get_write  method when write method has innput parameter "
    "54 bytes 25bytes, and OK I2C DeviceStatus") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 100UL;
    size_t count = 0;
    WHEN(
        "The write method is called with input parameters, data as [63 89 9a a9 27 54 0e 5e 90 c2 "
        "15 21 45 e1 e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 eb "
        "e0 73 a3 09 9a 4c 13 e3 ce 3f 9b 99 07 be ] and ok "
        "I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(
          0x63,
          0x89,
          0x9a,
          0xa9,
          0x27,
          0x54,
          0x0e,
          0x5e,
          0x90,
          0xc2,
          0x15,
          0x21,
          0x45,
          0xe1,
          0xe1,
          0xbb,
          0x99,
          0x55,
          0x73,
          0xa6,
          0xa4,
          0x04,
          0x27,
          0xae,
          0x2d,
          0x1a,
          0xf0,
          0x43,
          0xd6,
          0x76,
          0xd8,
          0xb6,
          0xce,
          0x30,
          0xfc,
          0x89,
          0xe6,
          0xf2,
          0xe9,
          0xeb,
          0xe0,
          0x73,
          0xa3,
          0x09,
          0x9a,
          0x4c,
          0x13,
          0xe3,
          0xce,
          0x3f,
          0x9b,
          0x99,
          0x07,
          0xbe);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_write;
      auto write_status = mock_device.get_write(input_write.buffer(), count);
      REQUIRE(count == 50);
      THEN("The get_write method returns OK status") {
        REQUIRE(write_status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The get_write method returns only 50 data bytes [63 89 9a a9 27 54 0e 5e 90 c2 15 21 45 "
          "e1 e1 bb 99 55 73 a6 a4 04 27 ae 2d 1a f0 43 d6 76 d8 b6 ce 30 fc 89 e6 f2 e9 eb e0 73 "
          "a3 09 9a 4c 13 e3 ce 3f]") {
        auto buffer = PF::Util::Containers::convert_byte_vector_to_hex_string(input_write, count);
        const auto* expected =
            R"(\x63\x89\x9A\xA9\x27\x54\x0E\x5E\x90\xC2\x15\x21\x45\xE1\xE1\xBB\x99\x55\x73\xA6\xA4\x04\x27\xAE\x2D\x1A\xF0\x43\xD6\x76\xD8\xB6\xCE\x30\xFC\x89\xE6\xF2\xE9\xEB\xE0\x73\xA3\x09\x9A\x4C\x13\xE3\xCE\x3F)";
        REQUIRE(buffer == expected);  // 254 null bytes instead
      }
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
          0xeb,
          0x65,
          0xa1,
          0x62,
          0x93,
          0x13,
          0x33,
          0x23,
          0xff,
          0x97,
          0xba,
          0x63,
          0xf2,
          0x46,
          0x46,
          0xdd,
          0x1a,
          0x64,
          0xa2,
          0xc1,
          0x50,
          0x04,
          0xd3,
          0x09,
          0x31);
      mock_device.write(read_buffer1.data(), read_buffer1.size());
      PF::Util::Containers::ByteVector<buffer_size> input_write1;
      auto write_status1 = mock_device.get_write(input_write1.buffer(), count);
      THEN("The get_write method returns OK status") {
        REQUIRE(write_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "The get_write method returns expected 25 data bytes [eb 65 a1 62 93 13 33 23 ff 97 ba "
          "63 f2 46 46 dd 1a 64 a2 c1 50 04 d3 09 31]") {
        auto read_buffer1 =
            PF::Util::Containers::convert_byte_vector_to_hex_string(input_write1, count);
        const auto* expected =
            R"(\xEB\x65\xA1\x62\x93\x13\x33\x23\xFF\x97\xBA\x63\xF2\x46\x46\xDD\x1A\x64\xA2\xC1\x50\x04\xD3\x09\x31\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00)";
        REQUIRE(read_buffer1 == expected);  // 254 null bytes instead
      }
    }
  }
}

SCENARIO("Mock device works correctly when write method is called three times") {
  GIVEN("Freshly constructed mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = 0;

    WHEN(
        "The write method is called five times with buffer bytes [db fe] [79 4b] [cd 93] [5a 56] "
        "[22 19]and I2CDevice status as [Ok] [write_error] [test_failed] [no_new_data] "
        "[invalid_argument] ") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      mock_device.write(write_buffer.data(), write_buffer.size());
      auto write_status1 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status1);
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x32, 0x9b, 0xf7, 0x4b);
      mock_device.write(write_buffer1.data(), write_buffer1.size());
      auto write_status2 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status2);
      auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0xcd, 0x93);
      mock_device.write(write_buffer2.data(), write_buffer2.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer3;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
      auto status_write3 = mock_device.get_write(input_buffer3.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("After the first write call, get_write returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      THEN("After the second write call, get_write returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [32 9b f7 4b] bytes") {
        REQUIRE(input_buffer1[0] == 0x32);
        REQUIRE(input_buffer1[1] == 0x9b);
        REQUIRE(input_buffer1[2] == 0xf7);
        REQUIRE(input_buffer1[3] == 0x4b);
      }
      THEN("After the third write call, get_write returns ok status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [cd 93] bytes") {
        REQUIRE(input_buffer2[0] == 0xcd);
        REQUIRE(input_buffer2[1] == 0x93);
      }
      THEN("After the fourth write call, get_write returns no_new_data status") {
        REQUIRE(status_write3 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO("Mock device works correctly when write method is called two times") {
  GIVEN("Freshly constructed mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = 0;

    WHEN(
        "The write method is called two times with buffer bytes [db fe] [32 9b f7 4b] "
        "and I2CDevice status as [Ok] [OK] ") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      mock_device.write(write_buffer.data(), write_buffer.size());
      auto write_status1 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status1);
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The count value returns 50") { REQUIRE(count == 50); }
      THEN("After the first write call, get_write returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x32, 0x9b, 0xf7, 0x4b);
      mock_device.write(write_buffer1.data(), write_buffer1.size());
      auto write_status2 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status2);
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      THEN("After the second write call, get_write returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [32 9b f7 4b] bytes") {
        REQUIRE(input_buffer1[0] == 0x32);
        REQUIRE(input_buffer1[1] == 0x9b);
        REQUIRE(input_buffer1[2] == 0xf7);
        REQUIRE(input_buffer1[3] == 0x4b);
      }
      PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
      auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
      THEN("After the fourth write call, get_write returns no_new_data status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}