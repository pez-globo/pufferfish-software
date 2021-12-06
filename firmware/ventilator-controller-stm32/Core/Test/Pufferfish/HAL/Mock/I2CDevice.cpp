#include "Pufferfish/HAL/Mock/I2CDevice.h"

#include <string>

#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"
#include "Pufferfish/HAL/Mock/Time.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Endian.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;

SCENARIO("Mock device works correctly for write method") {
  GIVEN("Freshly initalized mock-device") {
    PF::HAL::Mock::I2CDevice mock_device;
    auto write_status = PF::I2CDeviceStatus::ok;
    mock_device.add_write_status(write_status);
    auto write_buffer = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.write(write_buffer.data(), write_buffer.size());
    constexpr size_t buffer_size = 50UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer2;
    size_t count = buffer_size;
    auto status_write = mock_device.get_write(input_buffer.buffer(), count);
    REQUIRE(status_write == PF::I2CDeviceStatus::ok);
    auto write_status1 = PF::I2CDeviceStatus::write_error;
    mock_device.add_write_status(write_status1);
    auto write_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.write(write_buffer1.data(), write_buffer1.size());
    auto status_write1 = mock_device.get_write(input_buffer1.buffer(), count);
    REQUIRE(status_write1 == PF::I2CDeviceStatus::no_new_data);
    auto write_status2 = PF::I2CDeviceStatus::ok;
    mock_device.add_write_status(write_status2);
    auto write_buffer2 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.write(write_buffer2.data(), write_buffer2.size());
    auto status_write2 = mock_device.get_write(input_buffer2.buffer(), count);
    REQUIRE(status_write2 == PF::I2CDeviceStatus::ok);
  }
}