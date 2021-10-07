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

SCENARIO("The StateMachine::update method works correctly") {
  GIVEN(" A state machine is constructed") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN("The update method is called once with 4000 as input parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The update method returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the update method output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN("The update method is called twice with 4000us as input parameter for both cases") {
      uint32_t time = 4000;
      auto action_status1 = state_machine.update(4000);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      auto action_status2 = state_machine.update(time);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the second update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN("The update method is called thrice with [4000, 100, 5000] as input parameter") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 100;
      auto status = state_machine.update(time1);
      THEN("The second update method call, returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      uint32_t time2 = 5000;
      auto status2 = state_machine.update(time2);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
    WHEN("The update method is called four times with [4000, 250, 500, 100] as input parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 250;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      uint32_t time2 = 250;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      uint32_t time3 = 100;
      auto action_status2 = state_machine.update(time3);
      THEN("The fourth update method call returns measure action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("After the fourth update method output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
    }
    WHEN(
        "The update method is called four times with [4000, 3999, 3500, 3000] as input "
        "parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 3999;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      uint32_t time2 = 3500;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      uint32_t time3 = 3000;
      auto action_status2 = state_machine.update(time3);
      THEN("The fourth update method call returns measure action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("After the fourth update method, output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
    }
    WHEN("The update method is called three times with [4000, 5000, 6000] as input parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 5000;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the second update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time2 = 6000;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the third update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN("The update method is called three times with [4000, 5000, 500] as input parameter ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 5000;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the second update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time2 = 250;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the third update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
    }
    WHEN(
        "The update method is called five times with [500, 250, 5000, 6000, 200] as input "
        "parameter ") {
      uint32_t time = 500;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 250;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      uint32_t time2 = 5000;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      uint32_t time3 = 6000;
      auto action_status2 = state_machine.update(time3);
      THEN("The fourth update method call returns measure action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("After the fourth update method, output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      uint32_t time4 = 200;
      auto action_status3 = state_machine.update(time4);
      THEN("The fourth update method call returns wait_measurement action") {
        REQUIRE(action_status3 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the fourth update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
  }
}

SCENARIO("The Sensor::setup mehod works correctly") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 ae dd 5b 00 00 81](as read_conversion_factor response), reseter is set to "
      "false and mock time set to 100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());

    WHEN(
        "The setup method is called thrice , where mock read buffer is appended with bytes [97 38 "
        "1e](as read_sample response) and junk bytes [01 02]and mock "
        "time is set to [100, 40000, 50000] before each corresponding setup call") {
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }

      THEN("After first setup method call, get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(40000);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After second setup method call, get_state returns check_range action ") {
        auto state4 = sensor.get_state();
        REQUIRE(state4 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      time.set_micros(50000);
      auto status2 = sensor.setup();
      THEN("The third setup method call retuns ok state") {
        REQUIRE(status2 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes [01 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;

        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
    }
    WHEN(
        "The setup method is called twice , where mock read buffer is appended with bytes [97 38 "
        "1e](as read_sample response) and junk bytes [01 02] and mock "
        "time is set to [100, 200] before each corresponding setup call") {
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
      THEN("After the first setup method call, get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(200);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the second setup method call,  get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes [97 38 1e]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x97);
        REQUIRE(input[1] == 0x38);
        REQUIRE(input[2] == 0x1e);
      }
    }
    WHEN(
        "The setup method is called twice , where mock read buffer  is appended with bytes [15 35 "
        "a8](as read_sample response) and junk bytes [01 02] and mock "
        "time is set to [100, 100] before each corresponding setup call") {
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      auto state = sensor.get_state();
      THEN("After the first setup method call, get_state returns wait_warmup action") {
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(100);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the second setup method call, get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes [15 35 a8]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x15);
        REQUIRE(input[1] == 0x35);
        REQUIRE(input[2] == 0xa8);
      }
    }
  }
}
SCENARIO("The Sensor::setup mehod works correctly for differnt mock time") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 ae dd 5b 00 00 81](as read_conversion_factor response), reseter is set to "
      "false and mock time set to 100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());

    WHEN(
        "The setup method is called thrice , where mock read buffer  is appended with bytes [15 35 "
        "a8](as read_sample response) and junk bytes [01 02] and mock "
        "time is set to [100, 55000, 40000] before each corresponding setup call") {
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      THEN("After the first setup method call, get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(55000);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the second setup method call, get_state method returns check_range action") {
        auto state2 = sensor.get_state();
        REQUIRE(state2 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      auto status3 = sensor.setup();
      THEN("The third setup method call retuns ok status") {
        REQUIRE(status3 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes [01 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
    }
    WHEN(
        "The setup method is called thrice , where mock read buffer  is appended with bytes [d5 64 "
        "4d](as read_sample response) and junk bytes [01 02]and mock "
        "time is set to [100, 45000, 40000] before each corresponding setup call") {
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      auto state = sensor.get_state();
      THEN("After the first setup method call, get_state method returns wait_warmup action") {
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(45000);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the second setup method call, get_state method returns check_range action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      auto status2 = sensor.setup();
      THEN("The third setup method retuns ok state") {
        REQUIRE(status2 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes [01 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
    }
    WHEN(
        "The setup method is called twice , where mock read buffer  is appended with bytes [d5 64 "
        "4d](as read_sample response) and junk bytes [01 02] and mock "
        "time is set to [100, 5000] before each corresponding setup call") {
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
      THEN("After the first setup method call, get_state method returns wait_warmup action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(5000);
      auto status1 = sensor.setup();
      THEN("The second setup method returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the second setup method call, get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes [d5 64 4d]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0xd5);
        REQUIRE(input[1] == 0x64);
        REQUIRE(input[2] == 0x4d);
      }
    }
    WHEN(
        "The setup method is called thrice , where mock read buffer  is appended with bytes [d5 64 "
        "4d](as read_sample response) and junk bytes [01 02]and mock "
        "time is set to [45000, 40000, 39000] before each corresponding setup call") {
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(45000);
      auto status = sensor.setup();
      THEN("The first setup method call returns setup state") {
        REQUIRE(status == PF::InitializableState::setup);
      }
      auto state = sensor.get_state();
      THEN("After the first setup method call, get_state method returns wait_warmup action") {
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      time.set_micros(40000);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After the second setup method call, get_state method returns check_range action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      time.set_micros(39000);
      auto status2 = sensor.setup();
      THEN("The third setup method retuns ok state") {
        REQUIRE(status2 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns bytes [01 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
    }
  }
}

SCENARIO("The Sensor::output method works correctly") {
  GIVEN(
      "SFMC3019 sensor is fully initialised by calling setup method until it returns ok and action "
      "returns wait_measurement with the mock read buffer consisting of [97 38 1e 97 38 1e](as "
      "read_sample response) and junk bytes [01 02] afterwords and with the mock time set to "
      "100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 20.5;
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
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x01, 0x48, 0xf1);
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
        "The output method is called thrice with mock time set to [40000,1000, 2000] and output "
        "parameter flow is set to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN(
          "After the first output method call, get_state method returns wait_measurement action ") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }
      time.set_micros(1000);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("After the second output method call, get_state method returns measure action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      time.set_micros(2000);
      auto output_status2 = sensor.output(flow);
      THEN("The third output method call returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::ok);
      }
      THEN("After the third output method call, get_state method returns wait_measurement method") {
        auto state2 = sensor.get_state();
        REQUIRE(state2 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns -35.60588F value") { REQUIRE(flow == -35.60588F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes [01 02]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The output method is called thrice with mock time set to [40000, 500, 100] and Action is "
        "set to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("After the first output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(500);
      auto output_status1 = sensor.output(flow);
      THEN("The  second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }

      THEN("After the second output method call, get_state method returns measure action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }

      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }

      time.set_micros(100);
      auto output_status2 = sensor.output(flow);
      THEN("The third output method call returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::ok);
      }
      THEN("After the third output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }

      THEN("The output parameter flow returns -35.60588F value") { REQUIRE(flow == -35.60588F); }

      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes [01 02]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
  }
}
SCENARIO(
    "The Sensor::output method works correctly for different mcok read buffer and for mock time "
    "100") {
  GIVEN(
      "SFMC3019 sensor is fully initialised by calling setup method until it returns ok and action "
      "returns wait_measurement with the mock read buffer consisting of [15 35 a8 15 35 a8](as "
      "read_sample response) and junk bytes [01 02] afterwords and with the mock time set to "
      "100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 20.5;
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
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x01, 0x48, 0xf1);
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
        "The output method is called thrice with mock time set to [40000, 1000, 100], and output "
        "parameter is set to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("After the first output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(1000);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("After the second output method call, get_state method returns measure action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }
      time.set_micros(100);
      auto output_status2 = sensor.output(flow);
      THEN("The third output method call returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::ok);
      }
      THEN("After the third output method call, get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns 154.11765f value") { REQUIRE(flow == 154.11765F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns  junk bytes [01 02]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The output method is called thrice with mock time set to [40000, 500, 250] and output "
        "parameter flow is set to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("After the first output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      time.set_micros(500);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("After the second output method call, get_state method returns measure action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }
      time.set_micros(250);
      auto output_status2 = sensor.output(flow);
      THEN("The third output method call returns setup state") {
        REQUIRE(output_status2 == PF::InitializableState::ok);
      }
      THEN("After the third output method call, get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns 154.11765f value") { REQUIRE(flow == 154.11765F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns  junk bytes [01 02]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
    }
    WHEN(
        "The output method is called twice with mock time set to [4000] for both calls and output "
        "parameter flow is set to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("After the first output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      auto output_status1 = sensor.output(flow);
      THEN("The second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN(
          "After the second output method call, get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns [15 35 a8]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x15);
        REQUIRE(input_buffer[1] == 0x35);
        REQUIRE(input_buffer[2] == 0xa8);
      }
    }
  }
}
SCENARIO(
    "The Sensor::output method works correctly for different mcok read buffer and 500 mock time") {
  GIVEN(
      "SFMC3019 sensor is partially initialised by calling setup method where it returns setup and "
      "action returns wait_warmup with the mock read buffer consisting of [d5 64 4d](as "
      "read_sample response) afterwords and with the mock time set to 500us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 20.5;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
    mock_device.add_read(input_data.data(), input_data.size());
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    time.set_micros(500);

    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
    auto status = sensor.setup();
    REQUIRE(status == PF::InitializableState::setup);
    auto state = sensor.get_state();
    REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
    WHEN(
        "The output method is called once, mock time is set to [250]and output parameter flow is "
        "set to 20.5") {
      time.set_micros(250);
      auto output_status = sensor.output(flow);
      THEN("The output method returns failed state") {
        REQUIRE(output_status == PF::InitializableState::failed);
      }
      THEN("After  output method call, get_state method returns wait_warmup action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns [d5 64 4d]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input_buffer1.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer1[0] == 0xd5);
        REQUIRE(input_buffer1[1] == 0x64);
        REQUIRE(input_buffer1[2] == 0x4d);
      }
    }
  }
}
SCENARIO("The Sensor::output method works correctly for different mcok read buffer") {
  GIVEN(
      "SFMC3019 sensor is partially initialised by calling setup method where it returns setup and "
      "action returns check_range with the mock read buffer consisting of [d5 64 4d](as "
      "read_sample response) afterwords and with the mock time set to 500us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 20.5;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
    mock_device.add_read(input_data.data(), input_data.size());
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    time.set_micros(500);

    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
    auto status = sensor.setup();
    REQUIRE(status == PF::InitializableState::setup);
    sensor.get_state();
    time.set_micros(55000);
    sensor.setup();
    auto state1 = sensor.get_state();
    REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
    WHEN(
        "The output method is called once, mock time is set to [250]and output parameter flow is "
        "set to 20.5") {
      time.set_micros(250);
      auto output_status = sensor.output(flow);
      THEN("The output method call returns failed state") {
        REQUIRE(output_status == PF::InitializableState::failed);
      }
      THEN("After output method call, get_state method returns check_range action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("The output parameter flow returns 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        auto read_buffer = convert_byte_vector_to_hex_string(input_buffer, count);
        auto expected = std::string("\\xE1\\x02");
        REQUIRE(read_buffer == expected);
      }
      THEN("The mock_device I2C's read buffer returns [d5 64 4d]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input_buffer1.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer1[0] == 0xd5);
        REQUIRE(input_buffer1[1] == 0x64);
        REQUIRE(input_buffer1[2] == 0x4d);
      }
    }
  }
}
