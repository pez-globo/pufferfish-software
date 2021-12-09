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
    size_t count = buffer_size;
    WHEN(
        "The add_read method is called with input parameters, data as [a0 e8 d4 fd 97] and ok "
        "I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::ok;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
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
        "The add_read method is called with input parameters, data [a0 e8 d4 fd 97] bytes and "
        "read_error I2CDeviceStatus  ") {
      auto status = PF::I2CDeviceStatus::read_error;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The read method returns read_error status") {
        REQUIRE(read_status == PF::I2CDeviceStatus::read_error);
      }
    }
    WHEN(
        "The add_read method is called with input parameters, data [a0 e8 d4 fd 97]bytes  and "
        "crc_check_failed I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::crc_check_failed;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The read method returns crc_check_failed error") {
        REQUIRE(read_status == PF::I2CDeviceStatus::crc_check_failed);
      }
    }
    WHEN(
        "The add_read method is called with input parameters, data[a0 e8 d4 fd 97]bytes and "
        "test_failed I2CDeviceStatus") {
      auto status = PF::I2CDeviceStatus::test_failed;
      auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0xa0, 0xe8, 0xd4, 0xfd, 0x97);
      mock_device.add_read(read_buffer.data(), read_buffer.size(), status);
      PF::Util::Containers::ByteVector<buffer_size> input_read;
      auto read_status = mock_device.read(input_read.buffer(), count);
      THEN("The read method returns test_failed error") {
        REQUIRE(read_status == PF::I2CDeviceStatus::test_failed);
      }
    }
  }
}

SCENARIO("Mock I2CDevice works correctly for read method when called multiple times") {
  GIVEN("Freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    // auto read_status4 = PF::I2CDeviceStatus::ok;
    WHEN(
        "The add_read method is called five times with input parametes,[27 b8] [64 f3] [e1 7c] [d5 "
        "55] bytes and [ok][read_error][invalid_arguments][crc_check_failed][no_new_data] "
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

SCENARIO("Mock I2CDevice works correctly for write method") {
  GIVEN("A freshly constructed mock device") {
    PF::HAL::Mock::I2CDevice mock_device;
    constexpr size_t buffer_size = 50UL;
    size_t count = buffer_size;
    WHEN(
        "The write method is called with input parameter, data as[d5 1a 7a bc 24]bytes and Ok "
        "I2CDeviceStatus") {
      auto write_status = PF::I2CDeviceStatus::ok;
      mock_device.add_write_status(write_status);
      auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x1a, 0x7a, 0xbc, 0x24);
      mock_device.write(write_buffer.data(), write_buffer.size());
      PF::Util::Containers::ByteVector<buffer_size> input_buffer;
      auto status_write = mock_device.get_write(input_buffer.buffer(), count);
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
      THEN("The get_write method returns write_error status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::no_new_data);
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
      THEN("The get_write method returns write_error status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::no_new_data);
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
      THEN("The get_write method returns write_error status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::no_new_data);
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
      THEN("After the first write call, get_write returns ok status") {
        REQUIRE(status_write == PF::I2CDeviceStatus::ok);
      }
      THEN("After the first write call, get_write returns [db fe] bytes") {
        REQUIRE(input_buffer[0] == 0xdb);
        REQUIRE(input_buffer[1] == 0xfe);
      }
      THEN("After the second write call, get_write returns no_new_data status") {
        REQUIRE(status_write1 == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("After the third write call, get_write returns no_new_data status") {
        REQUIRE(status_write2 == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("After the fourth write call, get_write returns no_new_data status") {
        REQUIRE(status_write3 == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("After the fifth write call, get_write returns no_new_data status") {
        REQUIRE(status_write4 == PF::I2CDeviceStatus::no_new_data);
      }
    }
  }
}
