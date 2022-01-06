#include "Pufferfish/HAL/Mock/I2CDevice.h"

#include <string>

#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"
#include "Pufferfish/HAL/Mock/Time.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Endian.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;
SCENARIO("I2C mock device works correctly for add_read and read method") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 5UL;
    size_t count = buffer_size;
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97]bytes, and ok "
        "I2CDeviceStatus and count [5], and then read method is called twice with input "
        "parameter always with a count [5] ") {
      auto status = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The read method returns OK status") { REQUIRE(read_status == PF::I2CDeviceStatus::ok); }
      THEN("After the read method call, output buffer contains [a0 e8 d4 fd 97] bytes") {
        REQUIRE(input_read[0] == 0xa0);
        REQUIRE(input_read[1] == 0xe8);
        REQUIRE(input_read[2] == 0xd4);
        REQUIRE(input_read[3] == 0xfd);
        REQUIRE(input_read[4] == 0x97);
      }
      THEN("The second read method call returns no_new_data status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97] bytes, and "
        "read_error I2CDeviceStatus, and count [5], then read method is called twice"
        "with input parameter always with a count [5]") {
      auto status = PF::I2CDeviceStatus::read_error;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The first read method returns read_error status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::read_error);
      }
      THEN("After the first read method call, output buffer contains [a0 e8 d4 fd 97] bytes") {
        REQUIRE(input_read[0] == 0xa0);
        REQUIRE(input_read[1] == 0xe8);
        REQUIRE(input_read[2] == 0xd4);
        REQUIRE(input_read[3] == 0xfd);
        REQUIRE(input_read[4] == 0x97);
      }
      THEN("The second read method call returns no_new_data status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The add_read method is called with input parameters [a0 e8 d4 fd 97]bytes, and "
        "crc_check_failed I2CDeviceStatus and count [5] and then read method is "
        "called with input parameter count [5]") {
      auto status = PF::I2CDeviceStatus::crc_check_failed;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The read method returns crc_check_failed status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::crc_check_failed);
      }
      THEN("After the read method call, output buffer contains [a0 e8 d4 fd 97] bytes") {
        REQUIRE(input_read[0] == 0xa0);
        REQUIRE(input_read[1] == 0xe8);
        REQUIRE(input_read[2] == 0xd4);
        REQUIRE(input_read[3] == 0xfd);
        REQUIRE(input_read[4] == 0x97);
      }
      THEN("The second read method call returns no_new_data status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The add_read method is called with input parameters buffer[a0 e8 d4 fd 97]bytes, and "
        "[test_failed] I2CDeviceStatus, and count [5], and then read method is "
        "called twice with input parameter always with a count [5]") {
      auto status = PF::I2CDeviceStatus::test_failed;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The first read method returns test_failed status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::test_failed);
      }
      THEN("After first the read method call , output buffer contains[a0 e8 d4 fd 97] bytes") {
        REQUIRE(input_read[0] == 0xa0);
        REQUIRE(input_read[1] == 0xe8);
        REQUIRE(input_read[2] == 0xd4);
        REQUIRE(input_read[3] == 0xfd);
        REQUIRE(input_read[4] == 0x97);
      }
      THEN("The second read method call returns no_new_data status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "Mock I2CDevice works correctly when add_read method is called multiple times and then read "
    "method is called multiple times") {
  GIVEN("Freshly constructed mock device") {
    WHEN(
        "The add_read method is called five times with input parametes,buffer [27 b8] [64 f3] [e1 "
        "7c] [d5 "
        "55] [2e 87] bytes and [ok][read_error][invalid_arguments][crc_check_failed][no_new_data] "
        "I2CDeviceStatus and count [2], then read method is called six times with "
        "input parameter count [2]") {
      PF::HAL::Mock::I2CDevice mock_device;
      constexpr size_t buffer_size = 2UL;
      size_t count = buffer_size;
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      PF::Util::Containers::ByteVector<buffer_size> input_read4;
      PF::Util::Containers::ByteVector<buffer_size> input_read5;
      auto add_status = PF::I2CDeviceStatus::ok;
      auto add_status1 = PF::I2CDeviceStatus::read_error;
      auto add_status2 = PF::I2CDeviceStatus::invalid_arguments;
      auto add_status3 = PF::I2CDeviceStatus::crc_check_failed;
      auto add_status4 = PF::I2CDeviceStatus::test_failed;
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
      auto read_status5 = mock_device.read(input_read5.buffer(), count);
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, output buffer contains [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      THEN("The second read method call returns read_error status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::read_error);
      }
      THEN("After the second read method call, output buffer contains [64 f3]bytes") {
        REQUIRE(input_read1[0] == 0x64);
        REQUIRE(input_read1[1] == 0xf3);
      }
      THEN("The third read method call returns invalid_argument status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::invalid_arguments);
      }
      THEN("After the third read method call, output buffer contains [e1 7c]bytes") {
        REQUIRE(input_read2[0] == 0xe1);
        REQUIRE(input_read2[1] == 0x7c);
      }

      THEN("The fourth read method call returns crc_check_failed status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::crc_check_failed);
      }
      THEN("After the fourth read method call, output buffer contains [d5 55]bytes") {
        REQUIRE(input_read3[0] == 0xd5);
        REQUIRE(input_read3[1] == 0x55);
      }

      THEN("The fifth read method call returns test_failed status") {
        REQUIRE(read_status4 == PF::I2CDeviceStatus::test_failed);
      }
      THEN("After the fifth read method call, output buffer contains [2e 87]bytes") {
        REQUIRE(input_read4[0] == 0x2e);
        REQUIRE(input_read4[1] == 0x87);
      }
      THEN("The sixth read method call returns no_new_data status") {
        REQUIRE(read_status5 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The add_read method is called four times with input parametes,[27 b8] [64 f3] [e1 7c] [d5 "
        "55] bytes and [ok][read_error][invalid_arguments][ok]"
        "I2CDeviceStatus and count [2], then read method is called five times "
        "with input parameter count [2]") {
      PF::HAL::Mock::I2CDevice mock_device;
      constexpr size_t buffer_size = 20UL;
      size_t count = 2;
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
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, output buffer contains [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      THEN("The second read method call returns read_error status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::read_error);
      }
      THEN("After the second read method call, output buffer contains [64 f3]bytes") {
        REQUIRE(input_read1[0] == 0x64);
        REQUIRE(input_read1[1] == 0xf3);
      }
      THEN("The third read method call returns invalid_argument status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::invalid_arguments);
      }
      THEN("After the third read method call, output buffer contains [e1 7c]bytes") {
        REQUIRE(input_read2[0] == 0xe1);
        REQUIRE(input_read2[1] == 0x7c);
      }
      THEN("The fourth read method call returns ok status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the fourth read method call, output buffer contains [d5 55]bytes") {
        REQUIRE(input_read3[0] == 0xd5);
        REQUIRE(input_read3[1] == 0x55);
      }
      THEN("The fifth read method call returns no_new_data status") {
        REQUIRE(read_status4 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    // count is greater than read_buf size, invalid argument
    // WHEN(
    //     "The add_read method is called three times with input parametes,[27 b8] [64 f3] [e1 7c] "
    //     "bytes and [ok][read_error][ok]"
    //     "I2CDeviceStatus and count [10], then read method is called three times "
    //     "with input parameter count [10]") {
    //   PF::HAL::Mock::I2CDevice mock_device;
    //   constexpr size_t buffer_size = 20UL;
    //   size_t count = 10;
    //   PF::Util::Containers::ByteVector<buffer_size> input_read;
    //   PF::Util::Containers::ByteVector<buffer_size> input_read1;
    //   PF::Util::Containers::ByteVector<buffer_size> input_read2;
    //   PF::Util::Containers::ByteVector<buffer_size> input_read3;
    //   auto add_status = PF::I2CDeviceStatus::ok;
    //   auto add_status1 = PF::I2CDeviceStatus::read_error;
    //   auto add_status2 = PF::I2CDeviceStatus::invalid_arguments;
    //   auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
    //   mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
    //   auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3);
    //   mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status1);
    //   auto input_data = PF::Util::Containers::make_array<uint8_t>(0xe1, 0x7c);
    //   mock_device.add_read(input_data.data(), input_data.size(), add_status2);
    //   auto read_status = mock_device.read(input_read.buffer(), count);
    //   auto read_status1 = mock_device.read(input_read1.buffer(), count);
    //   auto read_status2 = mock_device.read(input_read2.buffer(), count);
    //   THEN("The first read method call returns Ok status") {
    //     REQUIRE(read_status == PF::I2CDeviceStatus::ok);
    //   }
    //   THEN("After the first read method call, output buffer contains [27 b8]bytes") {
    //     REQUIRE(input_read[0] == 0x27);
    //     REQUIRE(input_read[1] == 0xb8);
    //   }
    //   THEN("The second read method call returns read_error status") {
    //     REQUIRE(read_status1 == PF::I2CDeviceStatus::read_error);
    //   }
    //   THEN("After the second read method call, output buffer contains [64 f3]bytes") {
    //     REQUIRE(input_read1[0] == 0x64);
    //     REQUIRE(input_read1[1] == 0xf3);
    //   }
    //   THEN("The third read method call returns invalid_argument status") {
    //     REQUIRE(read_status2 == PF::I2CDeviceStatus::invalid_arguments);
    //   }
    //   THEN("After the third read method call, output buffer contains [e1 7c]bytes") {
    //     REQUIRE(input_read2[0] == 0xe1);
    //     REQUIRE(input_read2[1] == 0x7c);
    //   }
    // }
    WHEN(
        "The add_read method is called three times with input parametes,[27 b8] [64 f3 e0 fd 6e "
        "68] [e1 7c 70 5f ee] "
        "bytes and [ok][ok][ok] I2CDeviceStatus and count [10], then read method "
        "is called four times with input parameter count [2 6 5 5] respactively") {
      PF::HAL::Mock::I2CDevice mock_device;
      constexpr size_t buffer_size = 20UL;
      size_t count = 10;
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      auto add_status = PF::I2CDeviceStatus::ok;
      auto add_status1 = PF::I2CDeviceStatus::ok;
      auto add_status2 = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
      auto read_buffer1 =
          PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3, 0xe0, 0xfd, 0x6e, 0x68);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status1);
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xe1, 0x7c, 0x70, 0x5f, 0xee);
      mock_device.add_read(input_data.data(), input_data.size(), add_status2);
      count = 2;
      auto read_status = mock_device.read(input_read.buffer(), count);
      count = 6;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      count = 5;
      auto read_status2 = mock_device.read(input_read2.buffer(), count);
      auto read_status3 = mock_device.read(input_read3.buffer(), count);
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, output buffer contains [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      THEN("The second read method call returns ok status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the second read method call, output buffer contains [64 f3]bytes") {
        REQUIRE(input_read1[0] == 0x64);
        REQUIRE(input_read1[1] == 0xf3);
      }

      THEN("The third read method call returns ok status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the third read method call, output buffer contains [e1 7c]bytes") {
        REQUIRE(input_read2[0] == 0xe1);
        REQUIRE(input_read2[1] == 0x7c);
      }
      THEN("The fourth read method call returns no_new_data status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "Mock I2CDevice works correctly when add_read method and read method is called in alternating "
    "order") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 20UL;
    WHEN(
        "The add_read method is called with input parameters,[27 b8]bytes and [ok] I2CDeviceStatus "
        "and count [50], and"
        "then read method is called with input parameter count [10]; and then add_read and read "
        "methods are called in alternating order for 6 more "
        "times where add_read method's input parameters are [64 f3 b3] "
        "[2f 1c 80 78] [39 25 ed] [fe 39] [2f 1c][9c 7f ]and [ok][ok][ok][ok][ok][ok]"
        "I2CDeviceStatus, and read method is called 7 times with input parameter count [2 3 4 3 2 "
        "2 6] respectively  ") {
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      PF::Util::Containers::ByteVector<buffer_size> input_read3;
      PF::Util::Containers::ByteVector<buffer_size> input_read4;
      PF::Util::Containers::ByteVector<buffer_size> input_read5;
      PF::Util::Containers::ByteVector<buffer_size> input_read6;
      PF::Util::Containers::ByteVector<buffer_size> input_read7;
      auto add_status = PF::I2CDeviceStatus::ok;
      size_t count = 2;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x27, 0xb8);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), add_status);
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The first read method call returns Ok status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first read method call, output buffer contains [27 b8]bytes") {
        REQUIRE(input_read[0] == 0x27);
        REQUIRE(input_read[1] == 0xb8);
      }
      count = 3;
      auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x64, 0xf3, 0xb3);
      mock_device.add_read(read_buffer1.data(), read_buffer1.size(), add_status);
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      THEN("The second read method call returns ok status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the second read method call, output buffer contains [64 f3 b3]bytes") {
        REQUIRE(input_read1[0] == 0x64);
        REQUIRE(input_read1[1] == 0xf3);
        REQUIRE(input_read1[2] == 0xb3);
      }
      count = 4;
      auto read_buffer2 = PF::Util::Containers::make_array<uint8_t>(0x2f, 0x1c, 0x80, 0x78);
      mock_device.add_read(read_buffer2.data(), read_buffer2.size(), add_status);
      auto read_status2 = mock_device.read(input_read2.buffer(), count);
      THEN("The third read method call returns Ok status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the third read method call, output buffer contains [2f 1c 80 78]bytes") {
        REQUIRE(input_read2[0] == 0x2f);
        REQUIRE(input_read2[1] == 0x1c);
        REQUIRE(input_read2[2] == 0x80);
        REQUIRE(input_read2[3] == 0x78);
      }
      count = 3;
      auto read_buffer3 = PF::Util::Containers::make_array<uint8_t>(0x39, 0x25, 0xed);
      mock_device.add_read(read_buffer3.data(), read_buffer3.size(), add_status);
      auto read_status3 = mock_device.read(input_read3.buffer(), count);
      THEN("The fourth read method call returns Ok status") {
        REQUIRE(read_status3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the fourth read method call, output buffer contains [39 25 ed]bytes") {
        REQUIRE(input_read3[0] == 0x39);
        REQUIRE(input_read3[1] == 0x25);
        REQUIRE(input_read3[2] == 0xed);
      }
      count = 2;
      auto read_buffer4 = PF::Util::Containers::make_array<uint8_t>(0xfe, 0x39);
      mock_device.add_read(read_buffer4.data(), read_buffer4.size(), add_status);
      auto read_status4 = mock_device.read(input_read4.buffer(), count);
      THEN("The fifth read method call returns Ok status") {
        REQUIRE(read_status4 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the fifth read method call, output buffer contains [fe 39]bytes") {
        REQUIRE(input_read4[0] == 0xfe);
        REQUIRE(input_read4[1] == 0x39);
      }
      count = 2;
      auto read_buffer5 = PF::Util::Containers::make_array<uint8_t>(0x2f, 0x1c);
      mock_device.add_read(read_buffer5.data(), read_buffer5.size(), add_status);
      auto read_status5 = mock_device.read(input_read5.buffer(), count);
      THEN("The sixth read method call returns Ok status") {
        REQUIRE(read_status5 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the sixth read method call, output buffer contains [2f 1c]bytes") {
        REQUIRE(input_read5[0] == 0x2f);
        REQUIRE(input_read5[1] == 0x1c);
      }
      count = 6;
      auto read_buffer6 =
          PF::Util::Containers::make_array<uint8_t>(0x9c, 0x7f, 0xaf, 0x63, 0xc7, 0x53);
      mock_device.add_read(read_buffer6.data(), read_buffer6.size(), add_status);
      auto read_status6 = mock_device.read(input_read6.buffer(), count);
      THEN("The seventh read method call returns Ok status") {
        REQUIRE(read_status6 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the seventh read method call, output buffer contains [9c 7f af 63 c7 53]bytes") {
        REQUIRE(input_read6[0] == 0x9c);
        REQUIRE(input_read6[1] == 0x7f);
        REQUIRE(input_read6[2] == 0xaf);
        REQUIRE(input_read6[3] == 0x63);
        REQUIRE(input_read6[4] == 0xc7);
        REQUIRE(input_read6[5] == 0x53);
      }
      auto read_status7 = mock_device.read(input_read7.buffer(), count);
      THEN("The eighth read method call returns no_new_data status") {
        REQUIRE(read_status7 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "I2C mock device works correctly for  add_read method when input parameter has differnt buffer "
    "size, then read method is called repeatedly") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    WHEN(
        "The add_read method is called two times , for first method call input parameters are 50 "
        "bytes "
        "[63 ...... ce 3f] and OK I2CDeviceStatus and count  [50], for second "
        "method call, input parameters are 25 data bytes [eb ... 09 31] and [ok] I2CDeviceStatus "
        "and count [25] ,then read method is called thrice with "
        "input parameter count [50 25 25] respectively") {
      auto status = PF::I2CDeviceStatus::ok;
      size_t count = 50;
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
          0x3f);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The first read method call returns OK status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "After the first read method, the output buffer contains expected exactly 50 data bytes "
          "[63 .... ce 3f]") {
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
      count = 25;
      PF::Util::Containers::ByteVector<buffer_size> input_read1;
      auto read_status1 = mock_device.read(input_read1.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_read2;
      auto read_status2 = mock_device.read(input_read2.buffer(), count);

      THEN("The second read method call returns OK status") {
        REQUIRE(read_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "After the second read method call,  the output buffer contains expected 25 data bytes "
          "[eb .... 09 31]") {
        auto read_buffer1 =
            PF::Util::Containers::convert_byte_vector_to_hex_string(input_read1, count);
        const auto* expected =
            R"(\xEB\x65\xA1\x62\x93\x13\x33\x23\xFF\x97\xBA\x63\xF2\x46\x46\xDD\x1A\x64\xA2\xC1\x50\x04\xD3\x09\x31)";
        REQUIRE(read_buffer1 == expected);  // 254 null bytes instead
      }
      THEN("The third read method call returns no_new_data status") {
        REQUIRE(read_status2 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO("Mock I2CDevice works correctly for write method and get_write method") {
  GIVEN("A freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = 5;
    WHEN(
        "The write method is called with input parameter [d5 1a 7a bc 24]bytes and count [5], "
        "add_write_status "
        "method is called with input parameter Ok "
        "I2CDeviceStatus and then get_write method is called twice") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      THEN("The write method returns ok status") { REQUIRE(status == PF::I2CDeviceStatus::ok); }
      THEN("The first get_write method's output parameter count has value 5") {
        REQUIRE(count == 5);
      }
      THEN("The get_write method call returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the get_write method call, output buffer contains [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
      THEN("The second get_write method call returns no_new_data status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The write method is called with input parameter [d5 1a 7a bc 24]bytes and count [5]"
        "add_write_status method with input parameter  "
        "write_error I2CDeviceStatus is called, and then get_write method is called twice") {
      auto write_status = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      THEN("The write method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::write_error);
      }
      THEN("The output parameter of get_write method count has value 5") { REQUIRE(count == 5); }
      THEN("The first get_write method call returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the get_write method call, output buffer contains [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
      THEN("The second get_write method call returns no_new_data status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The write method is called with input parameter [d5 1a 7a bc 24]bytes and count [5], and "
        "add_write_status method with input parameter"
        "not_supported I2CDeviceStatus is called and then get_write method is called twice") {
      auto write_status = PF::I2CDeviceStatus::not_supported;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      THEN("The write method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::not_supported);
      }
      THEN("The get_write output parameter count has value 5") { REQUIRE(count == 5); }
      THEN("The first get_write method call returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the get_write method call, output buffer contains [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
      THEN("The second get_write method call returns no_new_data status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::no_new_data);
      }
    }

    WHEN(
        "The write method is called with input parameters [d5 1a 7a bc 24]bytes and count [5], and "
        "add_write_status method is called with input parameter"
        "no_new_data I2CDeviceStatus, then get_write method is called twice") {
      auto write_status = PF::I2CDeviceStatus::no_new_data;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      THEN("The write method returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The get_write method output parameter count has value 5") { REQUIRE(count == 5); }
      THEN("The first get_write method call returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the get_write method call, output buffer contains [d5 1a 7a bc 24] bytes") {
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x1a);
        REQUIRE(input_buffer[2] == 0x7a);
        REQUIRE(input_buffer[3] == 0xbc);
        REQUIRE(input_buffer[4] == 0x24);
      }
      THEN("The second get_write method call returns no_new_data status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "Mock device works correctly when write method is called multiple times and then get_write "
    "method is called multiple times") {
  GIVEN("Freshly constructed mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = 5;

    WHEN(
        "The write method is called five times with input parameter buffer bytes [db fe] [79 4b] "
        "[cd 93] [5a 56] "
        "[22 19]and count [2], and add_write_staus method is called five times with input [Ok] "
        "I2CDeviceStatus "
        "[write_error] [test_failed] [no_new_data] "
        "[invalid_argument] and then get_write method is called six times") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      auto write_status1 = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status1);
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x79, 0x4b);
      auto status1 = mock_device.write(write_buffer1.data(), write_buffer1.size());
      auto write_status2 = PF::I2CDeviceStatus::test_failed;
      mock_device.add_write_status(write_status2);
      auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0xcd, 0x93);
      auto status2 = mock_device.write(write_buffer2.data(), write_buffer2.size());
      auto write_status3 = PF::I2CDeviceStatus::no_new_data;
      mock_device.add_write_status(write_status3);
      auto write_buffer3 = PF::Util::Containers::make_array<uint8_t>(0x5a, 0x56);
      auto status3 = mock_device.write(write_buffer3.data(), write_buffer3.size());
      auto write_status4 = PF::I2CDeviceStatus::invalid_arguments;
      mock_device.add_write_status(write_status4);
      auto write_buffer4 = PF::Util::Containers::make_array<uint8_t>(0x22, 0x19);
      auto status4 = mock_device.write(write_buffer4.data(), write_buffer4.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer3;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer4;
      PF::Util::Containers::ByteVector<buffer_size> input_buffer5;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
      auto status_write3 = mock_device.get_write(input_buffer3.buffer(), count);
      auto status_write4 = mock_device.get_write(input_buffer4.buffer(), count);
      auto status_write5 = mock_device.get_write(input_buffer5.buffer(), count);
      THEN("The first five write method return ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
        REQUIRE(status1 == PF::I2CDeviceStatus::write_error);
        REQUIRE(status2 == PF::I2CDeviceStatus::test_failed);
        REQUIRE(status3 == PF::I2CDeviceStatus::no_new_data);
        REQUIRE(status4 == PF::I2CDeviceStatus::invalid_arguments);
      }
      THEN("The get_write output parameter count has value 2") { REQUIRE(count == 2); }
      THEN("After the first write method call, get_write method returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first get_write method call, output buffer contains [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      THEN("After the second write method call, get_write method returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the second get_write method call, output buffer contains [79 4b] bytes") {
        REQUIRE(input_buffer1[0] == 0x79);
        REQUIRE(input_buffer1[1] == 0x4b);
      }
      THEN("After the third write method call, get_write method returns ok status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the third get_write method call, output buffer contains [cd 93] bytes") {
        REQUIRE(input_buffer2[0] == 0xcd);
        REQUIRE(input_buffer2[1] == 0x93);
      }
      THEN("After the fourth write method call, get_write method returns ok status") {
        REQUIRE(status_write3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the fourth get_write method call, output buffer contains [5a 56] bytes") {
        REQUIRE(input_buffer3[0] == 0x5a);
        REQUIRE(input_buffer3[1] == 0x56);
      }
      THEN("After the fifth write method call, get_write method returns ok status") {
        REQUIRE(status_write4 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the fifth get_write method call, output buffer contains  [22 19] bytes") {
        REQUIRE(input_buffer4[0] == 0x22);
        REQUIRE(input_buffer4[1] == 0x19);
      }
      THEN("The sixth get_write method call returns no_new_data status") {
        REQUIRE(status_write5 == PF::I2CDeviceStatus::no_new_data);
      }
    }
    WHEN(
        "The write method is called four times with input parameter buffer bytes [db fe] [79 4b] "
        "[cd 93] [22 19] and count [2] "
        "and add_write_status method is called four times with input I2CDeviceStatus [Ok] "
        "[write_error] [ok] [ok], and then get_write method is called "
        "five times ") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      auto write_status1 = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status1);
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x79, 0x4b);
      auto status1 = mock_device.write(write_buffer1.data(), write_buffer1.size());
      auto write_status2 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status2);
      auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0xcd, 0x93);
      auto status2 = mock_device.write(write_buffer2.data(), write_buffer2.size());
      auto write_status4 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status4);
      auto write_buffer4 = PF::Util::Containers::make_array<uint8_t>(0x22, 0x19);
      auto status3 = mock_device.write(write_buffer4.data(), write_buffer4.size());
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
      THEN("The four write method calls return ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
        REQUIRE(status1 == PF::I2CDeviceStatus::write_error);
        REQUIRE(status2 == PF::I2CDeviceStatus::ok);
        REQUIRE(status3 == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write method output parameter count has value 2") { REQUIRE(count == 2); }
      THEN("After the first write call, get_write method call returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first get_write method call, output buffer contains [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      THEN("After the second write method call, get_write method call returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the second get_write method call, output buffer contains [79 4b] bytes") {
        REQUIRE(input_buffer1[0] == 0x79);
        REQUIRE(input_buffer1[1] == 0x4b);
      }
      THEN("After the third write method call, get_write method call returns ok status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the third get_write method call, output buffer contains [cd 93] bytes") {
        REQUIRE(input_buffer2[0] == 0xcd);
        REQUIRE(input_buffer2[1] == 0x93);
      }
      THEN("After the fourth write method call, get_write method call returns ok status") {
        REQUIRE(status_write3 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the fourth get_write method call, output buffer contains [22 19] bytes") {
        REQUIRE(input_buffer3[0] == 0x22);
        REQUIRE(input_buffer3[1] == 0x19);
      }
      THEN("The fifth get_write method call returns no_new_data status") {
        REQUIRE(status_write4 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}

SCENARIO(
    "Mock device works correctly when write method is  get_write method is called in alternating "
    "order multiple times") {
  GIVEN("Freshly constructed mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    WHEN(
        "The write method is called with input paramter [db fe] and count [2], and "
        "add_write_status method is "
        "called with input [ok] I2CDeviceStatus and then "
        "get_write method is called; write, add_write_status and get_write method is called in "
        "alternating order twice, where write method's input parameters are [32 9b f7 4b] [52 f1 "
        "9d 1c] and count [4]"
        "and add_write_status's input parameters are [write_error] [OK] I2CDeviceStatus") {
      size_t count = 2;
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xdb, 0xfe);
      auto status = mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
      THEN("The firts write method call returns ok status") {
        REQUIRE(status == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write output parameter count has value 2") { REQUIRE(count == 2); }
      THEN("After the first write method call, get_write call returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write method call, output buffer contains [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      count = 4;
      auto write_status1 = PF::I2CDeviceStatus::write_error;
      mock_device.add_write_status(write_status1);
      auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x32, 0x9b, 0xf7, 0x4b);
      auto status1 = mock_device.write(write_buffer1.data(), write_buffer1.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
      auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
      THEN("The second write method call returns ok status") {
        REQUIRE(status1 == PF::I2CDeviceStatus::write_error);
      }

      THEN("After the second write method call, get_write method call returns ok status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the second write method call, output buffer contains [32 9b f7 4b] bytes") {
        REQUIRE(input_buffer1[0] == 0x32);
        REQUIRE(input_buffer1[1] == 0x9b);
        REQUIRE(input_buffer1[2] == 0xf7);
        REQUIRE(input_buffer1[3] == 0x4b);
      }
      THEN("The get_write output parameter count has value 4") { REQUIRE(count == 4); }
      auto write_status2 = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status2);
      auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0x52, 0xf1, 0x9d, 0x1c);
      auto status2 = mock_device.write(write_buffer2.data(), write_buffer2.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
      auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
      THEN("The third write method call returns ok status") {
        REQUIRE(status2 == PF::I2CDeviceStatus::ok);
      }

      THEN("After the third write call, get_write method call returns ok status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
      }
      THEN("After the third write method call, output buffer contains [52 f1 9d 1c] bytes") {
        REQUIRE(input_buffer2[0] == 0x52);
        REQUIRE(input_buffer2[1] == 0xf1);
        REQUIRE(input_buffer2[2] == 0x9d);
        REQUIRE(input_buffer2[3] == 0x1c);
      }
      THEN("The get_write output parameter count has value 4") { REQUIRE(count == 4); }
    }
  }
}

SCENARIO(
    "I2C mock device works correctly for get_write method and write method, when write method has "
    "input parameter "
    " of differnt buffer sizes ") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    WHEN(
        "The write method is called twice, first method call with input parameters, 50 data bytes "
        "and count [50] "
        "[63 ... 09 31] and add_write_status is called with input parameter [ok] "
        "I2CDeviceStatus, second method call has input parameters buffer bytes [eb .. 09 31] and "
        "count [50] "
        "then add_write_status is called with input parameter [OK] I2CDeviceStatus, then get_write "
        "method is called three times") {
      auto status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(status);
      size_t count = 50;
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
          0x3f);
      auto status1 = mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_write;
      auto write_status = mock_device.get_write(input_write.buffer(), count);
      THEN("The first write call returns ok status") {
        REQUIRE(status1 == PF::I2CDeviceStatus::ok);
      }
      THEN("The get_write output parameter count has value 50") { REQUIRE(count == 50); }
      THEN("The first get_write method returns OK status") {
        REQUIRE(write_status == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "After the first get_write method call, output buffer contains expected 50 data bytes "
          "[63 ..ce 3f]") {
        auto buffer = PF::Util::Containers::convert_byte_vector_to_hex_string(input_write, count);
        const auto* expected =
            R"(\x63\x89\x9A\xA9\x27\x54\x0E\x5E\x90\xC2\x15\x21\x45\xE1\xE1\xBB\x99\x55\x73\xA6\xA4\x04\x27\xAE\x2D\x1A\xF0\x43\xD6\x76\xD8\xB6\xCE\x30\xFC\x89\xE6\xF2\xE9\xEB\xE0\x73\xA3\x09\x9A\x4C\x13\xE3\xCE\x3F)";
        REQUIRE(buffer == expected);  // 254 null bytes instead
      }
      count = 25;
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
      THEN("The second write method returns ok status") {
        // REQUIRE(status2 == PF::I2CDeviceStatus::ok);
      }
      THEN("The second get_write method returns OK status") {
        REQUIRE(write_status1 == PF::I2CDeviceStatus::ok);
      }
      THEN(
          "After the second get_write method call, output buffer contains expected 25 data "
          "bytes [eb ... 09 31]") {
        auto read_buffer1 =
            PF::Util::Containers::convert_byte_vector_to_hex_string(input_write1, count);
        const auto* expected =
            R"(\xEB\x65\xA1\x62\x93\x13\x33\x23\xFF\x97\xBA\x63\xF2\x46\x46\xDD\x1A\x64\xA2\xC1\x50\x04\xD3\x09\x31)";
        REQUIRE(read_buffer1 == expected);  // 254 null bytes instead
      }
      THEN("The get_write output parameter count has value 25") { REQUIRE(count == 25); }
      PF::Util::Containers::ByteVector<buffer_size> input_write2;
      auto write_status2 = mock_device.get_write(input_write2.buffer(), count);
      THEN("The third get_write method call returns no_new_data status") {
        REQUIRE(write_status2 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}
