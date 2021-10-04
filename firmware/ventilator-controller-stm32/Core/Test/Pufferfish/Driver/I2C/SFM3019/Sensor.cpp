/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Sensor.cpp
 *
 *  Created on:
 *      Author: Raavi Lagoo
 *
 * Unit tests to confirm behavior of SFM3019  Device
 *
 */
#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"

#include <string>

#include "Pufferfish/HAL/Mock/I2CDevice.h"
#include "Pufferfish/HAL/Mock/Time.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Endian.h"
#include "catch2/catch.hpp"
namespace PF = Pufferfish;
using PF::Util::Containers::convert_byte_vector_to_hex_string;

SCENARIO("The update method works correctly") {
  GIVEN(" A state machine is constructed") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN("The update method is called once with 4000 as input parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }

    WHEN("The update method is called twice with 4000us as input parameter") {
      uint32_t time = 4000;
      auto action_status1 = state_machine.update(4000);
      THEN("The first update method returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The second update method returns wait_warmup") {
        auto action_status1 = state_machine.update(time);
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN("The update method is called thrice with [4000, 100, 5000] as input parameter") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 100;
      auto status = state_machine.update(time1);
      THEN("The second update method returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }

      THEN("The third update method returns wait_measurement action") {
        uint32_t time = 5000;
        auto status = state_machine.update(time);
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
    WHEN("The update method is called three times with [4000, 250, 500] as input parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 250;
      auto status = state_machine.update(time1);
      THEN("The second update method returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("The third update method returns wait_measurement action") {
        uint32_t time1 = 250;
        auto action_status1 = state_machine.update(time1);
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
  }
}

SCENARIO("The setup mehod works correctly") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81], reseter is set to false and mock time set to 100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    size_t count = buffer_size;

    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x00, 0x00, 0x81);
    WHEN(
        "The setup method is called thrice , where mock read buffer  is appended with bytes [97 38 "
        "1e] and junk bytes [01 02]and mock "
        "time is set to [100, 40000, 50000] before each corresponding setup call") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      THEN("After setup method is called first time, get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(40000);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the setup method is called second time, get_state returns check_range action ") {
        auto state4 = sensor.get_state();
        REQUIRE(state4 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      time.set_micros(50000);
      auto status2 = sensor.setup();
      THEN("The third setup method retuns ok state") {
        REQUIRE(status2 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer remains unchanged") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The setup method is called twice , where mock read buffer  is appended with bytes [97 38 "
        "1e] and junk bytes [01 02] and mock "
        "time is set to [100, 200] before each corresponding setup call") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(200);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes 0x97381e") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x97);
        REQUIRE(input_buffer[1] == 0x38);
        REQUIRE(input_buffer[2] == 0x1e);
      }
    }
    WHEN(
        "The setup method is called twice , where mock read buffer  is appended with bytes [15 35 "
        "a8] and junk bytes [01 02] and mock "
        "time is set to [100, 2000] before each corresponding setup call") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      auto state = sensor.get_state();
      THEN("The get_state returns wait_warmup action") {
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(100);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes 0x1535a8") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x15);
        REQUIRE(input_buffer[1] == 0x35);
        REQUIRE(input_buffer[2] == 0xa8);
      }
    }
    WHEN(
        "The setup method is called thrice , where mock read buffer  is appended with bytes [15 35 "
        "a8] and junk bytes [01 02] and mock "
        "time is set to [100, 55000, 40000] before each corresponding setup call") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(55000);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns check_range action") {
        auto state2 = sensor.get_state();
        REQUIRE(state2 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      auto status3 = sensor.setup();
      THEN("The third setup method retuns ok status") {
        REQUIRE(status3 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes 0x0102") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The setup method is called thrice , where mock read buffer  is appended with bytes [d5 64 "
        "4d] and junk bytes [01 02]and mock "
        "time is set to [100, 45000, 40000] before each corresponding setup call") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup methos returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      auto state = sensor.get_state();
      THEN("The get_state method returns wait_warmup action") {
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(45000);
      auto status1 = sensor.setup();
      THEN("The second setup methos returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns check_range action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      auto status2 = sensor.setup();
      THEN("The third setup method retuns ok state") {
        REQUIRE(status2 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes 0x0102") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The setup method is called twice , where mock read buffer  is appended with bytes [d5 64 "
        "4d] and junk bytes [01 02] and mock "
        "time is set to [100, 5000] before each corresponding setup call") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(4000);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes 0xd5644d") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0xd5);
        REQUIRE(input_buffer[1] == 0x64);
        REQUIRE(input_buffer[2] == 0x4d);
      }
    }
  }
}

SCENARIO("The output method works correctly") {
  GIVEN(
      "SFMC3019 sensor is created by mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81 97 38 1e 97 38 1e] and junk bytes [01 02], reseter is set to "
      "false and mock time set to "
      "100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 0;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    size_t count = buffer_size;
    time.set_micros(100);
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(input_data1.data(), input_data1.size());
    auto junk_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(junk_data.data(), junk_data.size());
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
    auto status = sensor.setup();
    REQUIRE(status == PF::InitializableState::setup);
    auto state = sensor.get_state();
    REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
    time.set_micros(40000);
    auto status1 = sensor.setup();
    REQUIRE(status1 == PF::InitializableState::setup);
    for (size_t i = 0; i < 3; i++) {
      auto status2 = sensor.setup();
      REQUIRE(status2 == PF::InitializableState::ok);
    }
    auto state1 = sensor.get_state();
    REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
    WHEN(
        "The output method is called thrice with mock time set to [40000,1000] and Action is set "
        "to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      THEN("The first output method returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action ") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(1000);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns measure action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      auto output_status2 = sensor.output(flow);
      THEN("The third output method returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_measurement method") {
        auto state2 = sensor.get_state();
        REQUIRE(state2 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns -35.60588F value") { REQUIRE(flow == -35.60588F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns  junk bytes 0x0102") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The output method is called thrice with mock time set to [40000, 10] and Action is set to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      THEN("The first output method returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(500);
      auto output_status1 = sensor.output(flow);
      THEN("The  second output method returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns measure action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      auto output_status2 = sensor.output(flow);
      THEN("The third output method returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }

      THEN("The output parameter flow returns -35.60588F value") { REQUIRE(flow == -35.60588F); }

      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes 0x0102") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
  }
  GIVEN(
      "SFMC3019 sensor is created by mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81 15 35 a8 15 35 a8] and junk bytes [01 02], reseter is set to "
      "false and mock time set to "
      "100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 0;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    size_t count = buffer_size;

    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    time.set_micros(100);
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data1.data(), input_data1.size());
    auto junk_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(junk_data.data(), junk_data.size());
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
    auto status = sensor.setup();
    REQUIRE(status == PF::InitializableState::setup);
    auto state = sensor.get_state();
    REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
    time.set_micros(40000);
    auto status1 = sensor.setup();
    REQUIRE(status1 == PF::InitializableState::setup);
    for (size_t i = 0; i < 3; i++) {
      auto status2 = sensor.setup();
      REQUIRE(status2 == PF::InitializableState::ok);
    }
    auto state1 = sensor.get_state();
    REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
    WHEN(
        "The output method is called thrice with mock time set to [40000, 1000], and Action is set "
        "to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      THEN("The first output method returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(1000);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      auto output_status2 = sensor.output(flow);
      THEN("The third output method returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns 154.11765f value") { REQUIRE(flow == 154.11765F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns  junk bytes 0x0102") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The output method is called thrice with mock time set to [40000, 10], and Action is set "
        "to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      THEN("The first output method returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(500);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      auto output_status2 = sensor.output(flow);
      THEN("The third output method returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::setup);
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns 154.11765f value") { REQUIRE(flow == 154.11765F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns  junk bytes 0x0102") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
  }
  GIVEN(
      "SFMC3019 sensor is created by mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81 d5 64 4d d5 64 4d], reseter is set to false and mock time set to "
      "500us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 0;
    constexpr size_t buffer_size = 254UL;
    PF::Util::Containers::ByteVector<buffer_size> input_buffer;
    size_t count = buffer_size;

    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    time.set_micros(500);
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
    mock_device.add_read(input_data1.data(), input_data1.size());
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
    WHEN(
        "The output method is called once, mock time is set to [4000]and Action is set to "
        "wait_warmup") {
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(4000);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      auto state1 = sensor.get_state();
      REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      THEN("The output method returns failed state") {
        auto output_status = sensor.output(flow);
        REQUIRE(output_status == PF::InitializableState::failed);
      }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns 0x00") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x00);
      }
    }
  }
}
