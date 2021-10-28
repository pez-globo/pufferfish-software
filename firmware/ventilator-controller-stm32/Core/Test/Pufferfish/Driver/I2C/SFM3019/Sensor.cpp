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

SCENARIO(
    "The StateMachine::update method works correctly when update is called with normal interval of "
    "time for wait_warmup - check range cycle ") {
  GIVEN(" A freshly constructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN("The update method is called twice with 4000us as input parameter ") {
      // Action returns wait_warmup action
      uint32_t time = 0;
      auto status1 = state_machine.update(time);
      THEN("The first update method returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 30000;
      auto status = state_machine.update(time1);
      THEN("The second update method returns check_range action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
    }
  }
}

SCENARIO(
    "The StateMachine::update method works correctly when update is called with normal interval of "
    "time for wait_measurement-measure cycle ") {
  GIVEN(" A freshly constructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN("The update method is called four times with [4000, 250, 500, 100]us as input parameter") {
      // Action cycles between measure and wait_measurement
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
      uint32_t time4 = 3000;
      auto action_status3 = state_machine.update(time4);
      THEN("The fifth update method call returns measure action") {
        REQUIRE(action_status3 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the fifth update method, output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
  }
}
SCENARIO(
    "The StateMachine::update method works correctly when update is called multiple times for edge "
    "cases for wait_warmup - check_range cycle") {
  GIVEN("The freshly constructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN("The update method is called two times with [0, 29999, 30000]us as input parameter ") {
      // for edge cases, action remains wait_warmup for 29999 time,
      // action returns check_range for 30000 input time
      uint32_t time = 0;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 29999;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the second update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time2 = 30000;
      auto status2 = state_machine.update(time2);
      THEN("The third update method call returns check_range action") {
        REQUIRE(status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the third update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
    }
  }
}
SCENARIO(
    "The StateMachine::update method works correctly when update is called multiple times for edge "
    "cases for wait_measurement-measure cycle") {
  GIVEN("The freshly constructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN(
        "The update method is called four times with [0, 30001, 30002, 30100]us as input "
        "parameter ") {
      // enough time is not elaspsed, action remains wait_measurement instead of measure
      uint32_t time2 = 0;
      auto action_status1 = state_machine.update(time2);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time3 = 30001;
      auto action_status2 = state_machine.update(time3);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }

      uint32_t time4 = 30002;
      auto action_status3 = state_machine.update(time4);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status3 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      uint32_t time5 = 30100;
      auto action_status4 = state_machine.update(time5);
      THEN("The fourth update method call returns wait_measurement action") {
        REQUIRE(action_status4 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the fourth update method output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
    WHEN(
        "The update method is called five times with [0, 30001, 30002, 31000, 32000]us as input "
        "parameter") {
      // enough time has elapsed, action returns wait_measurement
      uint32_t time2 = 0;
      auto action_status1 = state_machine.update(time2);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time3 = 30001;
      auto action_status2 = state_machine.update(time3);
      THEN("The second update method call returns check_range action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns check_range action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }

      uint32_t time4 = 30002;
      auto action_status3 = state_machine.update(time4);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status3 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      uint32_t time5 = 31000;
      auto action_status4 = state_machine.update(time5);
      THEN("The fourth update method call returns measure action") {
        REQUIRE(action_status4 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("After the fourth update method output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      uint32_t time6 = 32000;
      auto action_status5 = state_machine.update(time6);
      THEN("The fifth update method call returns measure action") {
        REQUIRE(action_status5 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the fifth update method output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
  }
}

SCENARIO(
    "The update method works correctly  when enough time is not elapsed between consecutive "
    "calls for wait_warmup - check_range action") {
  GIVEN("The freshly constructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    // Enough time is not elapsed , action remains wait_warmup
    WHEN("The update method is called three times with [4000, 5000, 6000]us as input parameter ") {
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
  }
}
SCENARIO(
    "The update method works correctly  when enough time is not elapsed between consecutive "
    "calls for wait_measurement-measure action") {
  GIVEN("The freshly constructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    // Not enough time has elapsed, action remains wait_measurement instead of measure
    WHEN(
        "The update method is called four times with [500, 250, 300, 300]us as input "
        "parameter and for fourth call input parameter remains same as for third call ") {
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
      uint32_t time2 = 300;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_measurement action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_measurement action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      auto action_status2 = state_machine.update(time2);
      THEN("The fourth update method call returns measure action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the fourth update method, output method returns measure action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
  }
}
SCENARIO(
    "The StateMachine::update method works correctly for relatively small interval of time as "
    "input for wait_warmup - check range cycle") {
  GIVEN(" A freshly contructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN(
        "The update method is called three times with [100, 299, 300]us as input "
        "parameter ") {
      // action remains  wait_warmup
      uint32_t time = 100;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 299;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the second update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time2 = 300;
      auto action_status1 = state_machine.update(time2);
      THEN("The third update method call returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the third update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
    }
  }
}
SCENARIO(
    "The StateMachine::update method works correctly for relatively small interval of time as "
    "input for wait_measurement - measure cycle") {
  GIVEN(" A freshly contructed state machine") {
    PF::Driver::I2C::SFM3019::StateMachine state_machine;
    WHEN(
        "The update method is called three times with [100, 299, 299, 30001]us as input "
        "parameter ") {
      // action remains  wait_warmup instead of measure
      uint32_t time = 0;
      auto status1 = state_machine.update(time);
      THEN("The first update method call returns wait_warmup action") {
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("After the first update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      uint32_t time1 = 30000;
      auto status = state_machine.update(time1);
      THEN("The second update method call returns wait_warmup action") {
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("After the second update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      //      uint32_t time2 = 300;
      auto action_status1 = state_machine.update(time1);
      THEN("The third update method call returns wait_warmup action") {
        REQUIRE(action_status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the third update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      uint32_t time2 = 30001;
      auto action_status2 = state_machine.update(time2);
      THEN("The fourth update method call returns wait_warmup action") {
        REQUIRE(action_status2 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("After the fourth update method, output method returns wait_warmup action") {
        auto op_state = state_machine.output();
        REQUIRE(op_state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
    }
  }
}

SCENARIO(
    "The Sensor::setup method works correctly for normal cases when action cycles form "
    "wait_warmup-check_range") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consisting of[04 02 60 06 "
      "11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response) and [97 38 "
      "1e](as read_sample response) followed by junk bytes [01 02], reseter is set to "
      "false and mock time is set to 100us. global device's mock write buffer is set to [01] "
      "byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data1.data(), input_data1.size());
    // normal case, Action goes from wait_warmup-check_range

    WHEN(
        "The setup method is called twice , mock time is set to [100, 31000]us before each "
        "corresponding setup call") {
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

      time.set_micros(31000);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After second setup method call, get_state returns check_range action ") {
        auto state4 = sensor.get_state();
        REQUIRE(state4 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains junk bytes [97 38 1e]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x97);
        REQUIRE(input[1] == 0x38);
        REQUIRE(input[2] == 0x1e);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO(
    "The Sensor::setup method works correctly for normal cases when action cycles form "
    "check_range-wait_measurement") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consisting of[04 02 60 06 "
      "11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response) and [97 38 "
      "1e](as read_sample response) followed by junk bytes [01 02], reseter is set to "
      "false and mock time is set to 100us. global device's mock write buffer is set to [01] "
      "byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data1.data(), input_data1.size());
    // normal case, action goes from check_range to wait_measurement
    WHEN(
        "The setup method is called thrice , mock time is set to [100, 31000, 31000]us before each "
        "corresponding setup call") {
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

      time.set_micros(31000);
      auto status1 = sensor.setup();
      THEN("The second setup method call returns setup state") {
        REQUIRE(status1 == PF::InitializableState::setup);
      }
      THEN("After second setup method call, get_state returns check_range action ") {
        auto state4 = sensor.get_state();
        REQUIRE(state4 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }

      auto status2 = sensor.setup();
      THEN("The third setup method call retuns ok state") {
        REQUIRE(status2 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }

      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains junk bytes [01 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO(
    "The Sensor::setup method works correctly when enough time is not elapsed and for cycle "
    "wait_warmup-check_range action") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response) and [15 35 "
      "a8](as read_sample response) followed by junk bytes [01 02] , reseter is set to "
      "false and mock time set to 100us , global device's mock write buffer is set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data1.data(), input_data1.size());

    // Enough time is not elapsed between two setup calls, timeout condition is not satisfied
    //  Action remains wait_warmup instead of check_range
    WHEN(
        "The setup method is called twice, mock time is set to [100, 5000]us before each "
        "corresponding setup call") {
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
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains bytes [15 35 a8]") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input;
        size_t count = buffer_size;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x15);
        REQUIRE(input[1] == 0x35);
        REQUIRE(input[2] == 0xa8);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO("The Sensor::setup method works correctly for different mock read buffer ") {
  // Data in mock read buffer contains wrong product id
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consisting of[97 38 1e]  "
      "(as a read_product_id response), "
      " reseter is set to false and mock time is set to 100us, global device's mock write buffer "
      "is "
      "set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    WHEN("The setup method is called once ,time is set to 100us") {
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns failed state") {
        REQUIRE(status == PF::InitializableState::failed);
      }

      THEN("After first setup method call, get_state method returns initialize action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::initialize);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains no_new_data") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
  // wrong mock buffer for read_conversion_factors
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consisting of[04 02 60 06 "
      "11 a9 00 00 81] (as a read_product_id response) followed by [97 38 1e](as "
      "read_conversion_factors response) "
      " reseter is set to false and mock time is set to 100us, global device's mock write buffer "
      "is "
      "set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
    mock_device.add_read(read_buffer1.data(), read_buffer.size());

    WHEN("The setup method is called once ,time is set to 100us") {
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns failed state") {
        REQUIRE(status == PF::InitializableState::failed);
      }
      THEN("After first setup method call, get_state method returns initialize action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::initialize);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        mock_device.get_write(input.buffer(), count);
        REQUIRE(input[0] == 0xE1);
        REQUIRE(input[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains no_new_data") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO(
    "The Sensor::setup method works correctly when flow is greater than the pre-defined limit") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response) [01 00 "
      "b1](as read_sample response) followed by junk bytes [01 02] , reseter is set to "
      "false and mock time is set to 100us, global device's mock write buffer is set to [01] "
      "byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data1.data(), input_data1.size());
    auto input_data2 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data2.data(), input_data2.size());
    auto input_data3 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data3.data(), input_data3.size());
    auto input_data4 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data4.data(), input_data4.size());
    auto input_data5 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data5.data(), input_data5.size());
    auto input_data6 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data6.data(), input_data6.size());
    auto input_data7 = PF::Util::Containers::make_array<uint8_t>(0x31, 0x38, 0xbb);
    mock_device.add_read(input_data7.data(), input_data7.size());

    // in check_range method, read_sample condition flow is 218.682 greater than flow max
    WHEN(
        "The setup method is called thrice , mock time is set to [100, 55000, 65000]us before each "
        "corresponding setup call") {
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
      // setup should returns failed state, flow is greater than range
      time.set_micros(65000);
      auto status2 = sensor.setup();
      THEN("The third setup method call returns setup state") {
        REQUIRE(status2 == PF::InitializableState::setup);
      }
      THEN("After the third setup method call, get_state method returns check_range action") {
        auto state2 = sensor.get_state();
        REQUIRE(state2 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains no_new_data") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO("The Sensor::setup method works correctly when resetter is set to true") {
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a read_product_id response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response), [15 35 "
      "a8](as read_sample response) followed by junk bytes [01 02], reseter is set to "
      "true and mock time is set to 100us, global device's mock write buffer is set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data1.data(), input_data1.size());

    WHEN(
        "The setup method is called thrice , where mock "
        "time is set to [100, 55000, 40000]us before each corresponding setup call") {
      PF::Driver::I2C::SFM3019::StateMachine state_machine;
      PF::Driver::I2C::SFM3019::Sensor sensor{device, true, time};
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
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains junk bytes [01 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer contains [06] byte") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x06);
      }
    }
  }
}

SCENARIO(
    "The Sensor::setup method works correctly when mock read buffer is different than expected, "
    "setup method retries for 8 times") {
  // wrong data for product id, after 8 tries it fails
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consisting of[00 aa a6 a0 "
      "00 7e 01 48 f1]  (as a invalid read_product_id response) "
      " reseter is set to false and mock time set to 100us, global device's mock write buffer is "
      "set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    WHEN("The setup method is called once ,time is set to 100us") {
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The setup method call returns failed state") {
        REQUIRE(status == PF::InitializableState::failed);
      }

      THEN("After setup method call, get_state method returns initialize action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::initialize);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains no_new_data") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }

  //   // First 36 bytes fails read_product_id conditions(first 4 retry_count fails), it passes on
  //   next
  //   // count
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[00 aa a6 a0 00 "
      "7e 00 00 81](4 invalid response) [04 02 60 06 11 a9 00 00 81] (as a valid read_product_id "
      "response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response) [15 35 "
      "a8](as read_sample response) followd by junk bytes [01 02], reseter is set to "
      "false and mock time set to 100us, global device's mock write buffer is set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto read_buffer2 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer2.data(), read_buffer2.size());
    auto read_buffer3 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer3.data(), read_buffer3.size());
    auto read_buffer4 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer4.data(), read_buffer4.size());
    auto read_buffer5 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer5.data(), read_buffer5.size());

    WHEN(
        "The setup method is called thrice , where  mock time is set to [100, 55000, 40000]us "
        "before "
        "each corresponding setup call") {
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
      mock_device.add_read(input_data.data(), input_data.size());
      auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
      mock_device.add_read(input_data1.data(), input_data1.size());

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
      time.set_micros(40000);
      auto status3 = sensor.setup();
      THEN("The third setup method call retuns ok status") {
        REQUIRE(status3 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains junk bytes [01 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
  // wrong read buffer for read_product_id conditions(first 7 retry_count fails), it passes on last
  // count
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[00 aa a6 a0 00 "
      "7e 00 00 81](7 invalid response)  [04 02 60 06 11 "
      "a9 00 00 81] (as a valid read_product_id response) "
      "and[00 aa a6 a0 00 7e 00 00 81](as read_conversion_factor response) [15 35 "
      "a8](as read_sample response) followd by junk bytes [01 02], reseter is set to "
      "false and mock time set to 100us, global device's mock write buffer is set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto read_buffer2 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer2.data(), read_buffer2.size());
    auto read_buffer3 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer3.data(), read_buffer3.size());
    auto read_buffer4 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer4.data(), read_buffer4.size());
    auto read_buffer5 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer5.data(), read_buffer5.size());
    auto read_buffer6 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer6.data(), read_buffer6.size());
    auto read_buffer7 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer7.data(), read_buffer7.size());
    auto read_buffer8 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer8.data(), read_buffer8.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data1.data(), input_data1.size());

    WHEN(
        "The setup method is called thrice , where  mock time is set to [100, 55000, 40000]us "
        "before "
        "each corresponding setup call") {
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
      time.set_micros(40000);
      auto status3 = sensor.setup();
      THEN("The third setup method call retuns ok status") {
        REQUIRE(status3 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains junk bytes [01 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
  // read buffer for read_conversion_factor is wrong, after 8 tries it fails
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a valid read_product_id response)"
      "and[04 02 60 06 11 a9 00 00 81](as invalid read_conversion_factor response), reseter is set "
      "to "
      "false and mock time is set to 100us, global device's mock write buffer is set to [01] "
      "byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());

    WHEN(
        "The setup method is called once , where mock time is set to 100us before corresponding "
        "setup call") {
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns failed state") {
        REQUIRE(status == PF::InitializableState::failed);
      }
      THEN("After the first setup method call, get_state method returns initialize action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::initialize);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains no_new_data") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
  // wrong read buffer for read_conversion_factors conditions(first 7 retry_count fails), it passes
  // on last
  //  count
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consists of[04 02 60 06 11 "
      "a9 00 00 81] (as a valid read_product_id response) "
      "and[04 02 60 06 11 a9 00 00 81](as 7 invalid read_conversion_factor response) and [00 aa a6 "
      "a0 00 "
      "7e 00 00 81](as valid read_conversion_factor response) [15 35 a8](as read_sample response) "
      "followd by junk bytes [01 02], reseter is set to "
      "false and mock time set to 100us, global device's mock write buffer is set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto read_buffer2 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer2.data(), read_buffer2.size());
    auto read_buffer3 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer3.data(), read_buffer3.size());
    auto read_buffer4 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer4.data(), read_buffer4.size());
    auto read_buffer5 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer5.data(), read_buffer5.size());
    auto read_buffer6 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer6.data(), read_buffer6.size());
    auto read_buffer7 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer7.data(), read_buffer7.size());
    auto read_buffer8 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer8.data(), read_buffer8.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0x15, 0x35, 0xa8);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
    mock_device.add_read(input_data1.data(), input_data1.size());

    WHEN(
        "The setup method is called thrice , where  mock time is set to [100, 55000, 40000]us "
        "before "
        "each corresponding setup call") {
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
      time.set_micros(40000);
      auto status3 = sensor.setup();
      THEN("The third setup method call retuns ok status") {
        REQUIRE(status3 == PF::InitializableState::ok);
      }
      THEN("After the third setup method call, get_state method returns wait_measurement action") {
        auto state1 = sensor.get_state();
        REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains junk bytes [01 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x01);
        REQUIRE(input[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
  GIVEN(
      "SFMC3019 sensor is created with mock I2C device with read buffer consisting of[00 aa a6 a0 "
      "00 7e 01 48 f1]  ( 4 invalid read_product_id response) and [04 02 60 06 11 a9 00 00 81](as "
      "valid read_product_id response) [04 02 60 06 11 a9 00 00 81](5 invalid "
      "read_conversion_factor response) "
      "[00 aa a6 a0 00 7e 01 48 f1] (as valid read_conversion_factor response )reseter is set to "
      "false and mock time set to 100us, global device's mock write buffer is "
      "set to [01] byte") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::HAL::Mock::Time time;
    constexpr size_t buffer_size = 254UL;
    size_t count = buffer_size;
    PF::Driver::I2C::SFM3019::ConversionFactors conversion;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto read_buffer2 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer2.data(), read_buffer2.size());
    auto read_buffer3 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer3.data(), read_buffer3.size());
    auto read_buffer4 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer4.data(), read_buffer4.size());
    auto read_buffer5 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer5.data(), read_buffer5.size());
    auto read_buffer6 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer6.data(), read_buffer6.size());
    auto read_buffer8 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer8.data(), read_buffer8.size());
    auto read_buffer9 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer9.data(), read_buffer9.size());
    auto read_buffer10 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer10.data(), read_buffer10.size());
    auto read_buffer7 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
    mock_device.add_read(read_buffer7.data(), read_buffer7.size());

    WHEN("The setup method is called once ,time is set to 100us before setup call") {
      PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
      time.set_micros(100);
      auto status = sensor.setup();
      THEN("The first setup method call returns failed state") {
        REQUIRE(status == PF::InitializableState::failed);
      }

      THEN("After first setup method call, get_state method returns initialize action") {
        auto state = sensor.get_state();
        REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::initialize);
      }
      THEN("The mock_device I2C's write buffer contains command byte [E1 02]") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer contains no_new_data") {
        PF::Util::Containers::ByteVector<buffer_size> input;
        auto read_status = mock_device.read(input.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input[0] == 0x00);
        REQUIRE(input[1] == 0xaa);
        REQUIRE(input[2] == 0xa6);
        REQUIRE(input[3] == 0xa0);
        REQUIRE(input[4] == 0x00);
        REQUIRE(input[5] == 0x7e);
        REQUIRE(input[6] == 0x01);
        REQUIRE(input[7] == 0x48);
        REQUIRE(input[8] == 0xf1);
      }
      THEN("The global_device I2C's read buffer is empty") {
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO(
    "The Sensor::output method works correctly for action cycles from wait_measurement - measure - "
    "wait_measurement") {
  GIVEN(
      "SFMC3019 sensor is fully initialised by calling setup method until it returns ok and action "
      "returns wait_measurement with the mock read buffer consisting of [97 38 1e 97 38 1e](as "
      "read_sample response) followed by junk bytes [01 02],  and with the mock time set "
      "to "
      "100us, global device's mock write buffer is set to [01] byte") {
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
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
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
    // Normal case, output returns ok state
    // updates flow according to read buffer
    WHEN(
        "The output method is called thrice with mock time set to [40000,1000, 2000]us and output "
        "parameter flow is initialized to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN(
          "After the first output method call, get_state method returns wait_measurement action ") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      time.set_micros(1000);
      for (size_t i = 0; i < 2; i++) {
        auto output_status1 = sensor.output(flow);
        THEN("The second output method call returns ok state") {
          REQUIRE(output_status1 == PF::InitializableState::ok);
        }
      }
      THEN(
          "After the second output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow is set to -13.22353F value") { REQUIRE(flow == -13.22353F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer returns junk bytes [01 02]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x01);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The global_device I2C's read buffer is empty") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO(
    "The Sensor::output method works correctly for when action cycles between  wait_measurement - "
    "measure") {
  GIVEN(
      "SFMC3019 sensor is fully initialised by calling setup method until it returns ok and action "
      "returns wait_measurement with the mock read buffer consisting of [15 35 a8 15 35 a8](as "
      "read_sample response) followed by junk bytes [01 02], the mock time is set "
      "to "
      "100us, global device's mock write buffer is set to [01] byte") {
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
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
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
    // action remains measure
    // flow does not get updated
    WHEN(
        "The output method is called twice with mock time set to [40000, 1000]us, and output "
        "parameter flow is initialized to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("After the first output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      time.set_micros(1000);
      auto output_status1 = sensor.output(flow);
      THEN("The second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN("After the second output method call, get_state method returns measure action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::measure);
      }
      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer returns  junk bytes [01 02]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x15);
        REQUIRE(input_buffer[1] == 0x35);
        REQUIRE(input_buffer[2] == 0xa8);
      }
      THEN("The global_device I2C's read buffer is empty") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
    // Action remains wait_measurement
    // Flow does not get updated
    WHEN(
        "The output method is called twice with mock time set to 4000us for both calls and output "
        "parameter flow is initialized to 20.5") {
      auto output_status = sensor.output(flow);
      THEN("The first output method call returns ok state") {
        REQUIRE(output_status == PF::InitializableState::ok);
      }
      THEN("After the first output method call, get_state method returns wait_measurement action") {
        auto status = sensor.get_state();
        REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      auto output_status1 = sensor.output(flow);
      THEN("The second output method call returns ok state") {
        REQUIRE(output_status1 == PF::InitializableState::ok);
      }
      THEN(
          "After the second output method call, get_state method returns wait_measurement action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
      }
      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
      }
      THEN("The mock_device I2C's read buffer returns [15 35 a8]") {
        auto read_status = mock_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::ok);
        REQUIRE(input_buffer[0] == 0x15);
        REQUIRE(input_buffer[1] == 0x35);
        REQUIRE(input_buffer[2] == 0xa8);
      }
      THEN("The global_device I2C's read buffer is empty") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}

SCENARIO("The Sensor::output method works correctly, when sensor is partially initialized") {
  GIVEN(
      "SFMC3019 sensor is partially initialised by calling setup method where it returns setup "
      "state and "
      "action returns wait_warmup, with the mock read buffer consisting of [d5 64 4d](as "
      "read_sample response) and with the mock time set to 500us, global device's mock write "
      "buffer is set to [01] byte") {
    // Action is wait_warmup, output method fails
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 20.5;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
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
        "The output method is called once, mock time is set to 250us and output parameter flow is "
        "initialized to 20.5") {
      time.set_micros(250);
      auto output_status = sensor.output(flow);
      THEN("The output method returns failed state") {
        REQUIRE(output_status == PF::InitializableState::failed);
      }
      THEN("After  output method call, get_state method returns wait_warmup action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
      }
      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
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
      THEN("The global_device I2C's read buffer is empty") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
  GIVEN(
      "SFMC3019 sensor is partially initialised by calling setup method where it returns setup and "
      "action returns check_range with the mock read buffer consisting of [d5 64 4d](as "
      "read_sample response) and with the mock time set to 500us, global device's mock write "
      "buffer is set to [01] byte") {
    // Action is check_range, output method fails
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    float flow = 20.5;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());

    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
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
    WHEN(
        "The output method is called once, mock time is set to 250us and output parameter flow is "
        "initialized to 20.5") {
      time.set_micros(250);
      auto output_status = sensor.output(flow);
      THEN("The output method returns failed state") {
        REQUIRE(output_status == PF::InitializableState::failed);
      }
      THEN("After  output method call, get_state method returns check_range action") {
        auto status1 = sensor.get_state();
        REQUIRE(status1 == PF::Driver::I2C::SFM3019::StateMachine::Action::check_range);
      }

      THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
      THEN("The mock_device I2C's write buffer returns command byte 0xE102") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        size_t count = buffer_size;
        mock_device.get_write(input_buffer.buffer(), count);
        REQUIRE(input_buffer[0] == 0xE1);
        REQUIRE(input_buffer[1] == 0x02);
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
      THEN("The global_device I2C's read buffer is empty") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer;
        auto read_status = global_device.read(input_buffer.buffer(), count);
        REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
      }
      THEN("The global_device I2C's write buffer remains unchanged") {
        constexpr size_t buffer_size = 254UL;
        PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
        size_t count = buffer_size;
        PF::Util::Containers::ByteVector<buffer_size> global_buffer;
        auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
        global_device.write(input_data.data(), input_data.size());
        global_device.get_write(global_buffer.buffer(), count);
        REQUIRE(global_buffer[0] == 0x01);
      }
    }
  }
}
// read_sample data is incorrect, output should return failed state
// SCENARIO(
//     "The Sensor::output method works correctly when read buffer is not as expected") {
//   GIVEN(
//       "SFMC3019 sensor is fully initialised by calling setup method until it returns ok and
//       action " "returns wait_measurement with the mock read buffer consisting of [97 38 1e 97 38
//       1e](as " "read_sample response) followed by junk bytes [01 02],  and with the mock time set
//       " "to " "100us, global device's mock write buffer is set to [01] byte") {
//     PF::HAL::Mock::I2CDevice mock_device;
//     PF::HAL::Mock::I2CDevice global_device;
//     float flow = 20.5;
//     PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
//     PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
//     PF::HAL::Mock::Time time;
//     constexpr size_t buffer_size = 254UL;
//     PF::Util::Containers::ByteVector<buffer_size> input_buffer;
//     size_t count = buffer_size;
//     time.set_micros(100);
//     auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
//         0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
//     mock_device.add_read(read_buffer.data(), read_buffer.size());
//     auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
//         0x00, 0xaa, 0xa6, 0xa0, 0x00, 0x7e, 0x01, 0x48, 0xf1);
//     mock_device.add_read(read_buffer1.data(), read_buffer1.size());
//     auto input_data = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
//     mock_device.add_read(input_data.data(), input_data.size());
//     // auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0x97, 0x38, 0x1e);
//     // mock_device.add_read(input_data1.data(), input_data1.size());
//     auto junk_data = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data.data(), junk_data.size());
//     auto junk_data1 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data1.data(), junk_data1.size());
//     auto junk_data2 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data2.data(), junk_data2.size());
//     auto junk_data3 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data3.data(), junk_data3.size());
//     auto junk_data4 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data4.data(), junk_data4.size());
//     auto junk_data5 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data5.data(), junk_data5.size());
//     auto junk_data6 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data6.data(), junk_data6.size());
//     auto junk_data7 = PF::Util::Containers::make_array<uint8_t>(0x01, 0x02);
//     mock_device.add_read(junk_data7.data(), junk_data7.size());

//     PF::Driver::I2C::SFM3019::StateMachine state_machine;
//     PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
//     auto status = sensor.setup();
//     REQUIRE(status == PF::InitializableState::setup);
//     auto state = sensor.get_state();
//     REQUIRE(state == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_warmup);
//     time.set_micros(40000);
//     auto status1 = sensor.setup();
//     REQUIRE(status1 == PF::InitializableState::setup);
//     for (size_t i = 0; i < 3; i++) {
//       auto status2 = sensor.setup();
//       REQUIRE(status2 == PF::InitializableState::ok);
//     }
//     auto state1 = sensor.get_state();
//     REQUIRE(state1 == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
//     // Normal case, output returns ok state
//     // updates flow according to read buffer
//     WHEN(
//         "The output method is called thrice with mock time set to [40000,1000, 2000]us and output
//         " "parameter flow is initialized to 20.5") {
//       auto output_status = sensor.output(flow);
//       THEN("The first output method call returns ok state") {
//         REQUIRE(output_status == PF::InitializableState::ok);
//       }
//       THEN(
//           "After the first output method call, get_state method returns wait_measurement action
//           ") {
//         auto status = sensor.get_state();
//         REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
//       }
//       THEN("The output parameter flow remains 20.5") { REQUIRE(flow == 20.5F); }
//       time.set_micros(1000);
//       for(size_t i=0;i<2;i++){
//         auto output_status1 = sensor.output(flow);
//         THEN("The second output method call returns ok state") {
//           REQUIRE(output_status1 == PF::InitializableState::ok);
//         }
//       }
//       THEN("After the second output method call, get_state method returns wait_measurement
//       action") {
//         auto status = sensor.get_state();
//         REQUIRE(status == PF::Driver::I2C::SFM3019::StateMachine::Action::wait_measurement);
//       }
//       THEN("The output parameter flow is set to -13.22353F value") { REQUIRE(flow == -13.22353F);
//       } THEN("The mock_device I2C's write buffer returns command byte [E1 02]") {
//         mock_device.get_write(input_buffer.buffer(), count);
//         REQUIRE(input_buffer[0] == 0xE1);
//         REQUIRE(input_buffer[1] == 0x02);
//       }
//       THEN("The mock_device I2C's read buffer returns junk bytes [01 02]") {
//         auto read_status = mock_device.read(input_buffer.buffer(), count);
//         REQUIRE(read_status == PF::I2CDeviceStatus::ok);
//         REQUIRE(input_buffer[0] == 0x01);
//         REQUIRE(input_buffer[1] == 0x02);
//       }
//       THEN("The global_device I2C's read buffer is empty") {
//         constexpr size_t buffer_size = 254UL;
//         PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
//         size_t count = buffer_size;
//         PF::Util::Containers::ByteVector<buffer_size> input_buffer;
//         auto read_status = global_device.read(input_buffer.buffer(), count);
//         REQUIRE(read_status == PF::I2CDeviceStatus::no_new_data);
//       }
//       THEN("The global_device I2C's write buffer remains unchanged") {
//         constexpr size_t buffer_size = 254UL;
//         PF::Util::Containers::ByteVector<buffer_size> input_buffer1;
//         size_t count = buffer_size;
//         PF::Util::Containers::ByteVector<buffer_size> global_buffer;
//         auto input_data = PF::Util::Containers::make_array<uint8_t>(0x01);
//         global_device.write(input_data.data(), input_data.size());
//         global_device.get_write(global_buffer.buffer(), count);
//         REQUIRE(global_buffer[0] == 0x01);
//       }
//     }
//   }
// }
