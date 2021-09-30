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

SCENARIO("The update method works correctly") {
  GIVEN(" A state machine is constructed") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;

    WHEN("The update method is called with 4000us as input parameter") {
      uint32_t time = 4000;
      auto action_status1 = state_machine.update(4000);
      THEN("The update method returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The update  returns wait_warmup") {
        auto action_status1 = state_machine.update(time);
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN("The update method is called with 100us as input parameter") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      uint32_t time1 = 100;
      auto status = state_machine.update(time1);
      THEN("The update method returns check range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }

      THEN(
          "After 5000us is passed as an input parameter, update method  returns wait_measurement") {
        uint32_t time = 5000;
        auto status = state_machine.update(time);
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
    WHEN("The update method is called three times with input parameter 250us ") {
      uint32_t time = 4000;
      auto status1 = state_machine.update(time);
      REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      for (size_t i = 0; i < 2; i++) {
        uint32_t time1 = 250;
        state_machine.update(time1);
      }
      THEN("The update method returns wait_measurement action") {
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
    time.set_micros(100);
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xae, 0xdd, 0x5b, 0x00, 0x00, 0x81);
    WHEN(
        "The setup method is called , where mock read buffer contains [97 38 1e 97 38 1e] and mock "
        "time is set to 40000us") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data1.data(), input_data1.size());
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
        THEN("The setup method retuns ok status") {
          REQUIRE(status2 == PF::InitializableState::ok);
        }
      }
      THEN("The get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
    WHEN(
        "The setup method is called , where mock read buffer contains [97 38 1e 97 38 1e] and mock "
        "time is set to 200us") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(200);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      THEN("The get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN(
        "The setup method is called , where mock read buffer contains [15 35 a8 15 35 a8] and mock "
        "time is set to 100us") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(100);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      THEN("The get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
    WHEN(
        "The setup method is called , where mock read buffer contains [15 35 a8 15 35 a8] and mock "
        "time is set to 55000us") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(55000);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      auto status2 = sensor.setup();
      THEN("The setup method retuns ok status") { REQUIRE(status2 == PF::InitializableState::ok); }
      THEN("The get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
    WHEN(
        "The setup method is called , where mock read buffer contains [d5 64 4d d5 64 4d] and mock "
        "time is set to 45000us") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(45000);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      for (size_t i = 0; i < 3; i++) {
        auto status2 = sensor.setup();
        THEN("The setup method retuns ok status") {
          REQUIRE(status2 == PF::InitializableState::ok);
        }
      }
      THEN("The get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }

    WHEN(
        "The setup method is called , where mock read buffer contains [d5 64 4d d5 64 4d] and mock "
        "time is set to 4000us") {
      mock_device.add_read(read_buffer1.data(), read_buffer1.size());
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0xd5, 0x64, 0x4d);
      mock_device.add_read(input_data1.data(), input_data1.size());
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(4000);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      THEN("The get_state method returns wait_warmup action ") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
  }
}

SCENARIO("The output method works correctly") {
  GIVEN(
      "SFMC3019 sensor is created by mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81 97 38 1e 97 38 1e], reseter is set to false and mock time set to "
      "100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 0;
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
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(input_data1.data(), input_data1.size());
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
        "The output method is called with mock time set to 1000 and Action is set to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      REQUIRE(output_status == PF::InitializableState::ok);
      time.set_micros(1000);
      for (size_t i = 0; i < 2; i++) {
        auto output_status1 = sensor.output(flow);
        THEN("The output method returns ok status") {
          REQUIRE(output_status1 == PF::InitializableState::ok);
        }
      }
      THEN("The get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow returns -35.60588F value") { REQUIRE(flow == -35.60588F); }
    }
    WHEN(
        "The output method is called with mock time set to 10 and Action is set to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      REQUIRE(output_status == PF::InitializableState::ok);
      time.set_micros(500);
      for (size_t i = 0; i < 2; i++) {
        auto output_status1 = sensor.output(flow);
        THEN("The output method returns ok status") {
          REQUIRE(output_status1 == PF::InitializableState::ok);
        }
      }
      auto status = sensor.get_state();
      REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      THEN("The output parameter flow returns -35.60588F value") { REQUIRE(flow == -35.60588F); }
    }
  }
  GIVEN(
      "SFMC3019 sensor is created by mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81 15 35 a8 15 35 a8], reseter is set to false and mock time set to "
      "100us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 0;
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
        "The output method is called with mock time set to 1000, and Action is set to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      REQUIRE(output_status == PF::InitializableState::ok);
      time.set_micros(1000);
      for (size_t i = 0; i < 2; i++) {
        auto output_status1 = sensor.output(flow);
        THEN("The output method returns ok status") {
          REQUIRE(output_status1 == PF::InitializableState::ok);
        }
      }
      auto status = sensor.get_state();
      REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      THEN("The output parameter flow returns 154.11765f value") { REQUIRE(flow == 154.11765F); }
    }
    WHEN(
        "The output method is called with mock time set to 10, and Action is set to "
        "wait_measurement") {
      auto output_status = sensor.output(flow);
      REQUIRE(output_status == PF::InitializableState::ok);
      time.set_micros(500);
      for (size_t i = 0; i < 2; i++) {
        auto output_status1 = sensor.output(flow);
        THEN("The output method returns ok status") {
          REQUIRE(output_status1 == PF::InitializableState::ok);
        }
      }
      auto status = sensor.get_state();
      REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      THEN("The output parameter flow returns 154.11765f value") { REQUIRE(flow == 154.11765F); }
    }
  }
  GIVEN(
      "SFMC3019 sensor is created by mock I2C device with read buffer[04 02 60 06 11 a9 00 00 81 "
      "00 aa a6 ae dd 5b 00 00 81 d5 64 4d d5 64 4d], reseter is set to false and mock time set to "
      "500us") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 0;
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
    WHEN("The output method is called , mock time is set to 250 and Action is set to wait_warmup") {
      auto status = sensor.setup();
      REQUIRE(status == PF::InitializableState::setup);
      auto state = sensor.get_state();
      REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      time.set_micros(4000);
      auto status1 = sensor.setup();
      REQUIRE(status1 == PF::InitializableState::setup);
      auto state1 = sensor.get_state();
      REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      THEN("The output method returns failed status") {
        auto output_status = sensor.output(flow);
        REQUIRE(output_status == PF::InitializableState::failed);
      }
    }
  }
}
