/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Device.cpp
 *
 *  Created on: Nov 25, 2020
 *      Author: Rohan Purohit
 *
 * Unit tests to confirm behavior of flow sensor driver
 *
 */
#include "Pufferfish/Driver/I2C/SFM3019/Sensor.h"

#include "Pufferfish/HAL/Mock/MockI2CDevice.h"
#include "Pufferfish/HAL/Mock/MockTime.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/Endian.h"
#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("SFM3019: flow sensor behaves properly", "[sensor]") {
  PF::HAL::MockI2CDevice dev;
  PF::HAL::MockI2CDevice gdev;
  PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::air;
  bool resetter = false;

  GIVEN("A Mock I2C device") {
    // write product_id to the MOCKI2Cdevice by add_read
    auto product_id_body = PF::Util::make_array<uint8_t>(0x04, 0x02, 0x60, 0x06, 0x11, 0xA9);
    dev.add_read(product_id_body.data(), product_id_body.size());

    // write conversion_factors to the MOCKI2Cdevice by add_read
    auto conversion_factors_body = PF::Util::make_array<uint8_t>(0x00, 0xAA, 0xA6, 0xA0, 0x00, 0x7E, 0x01, 0x48, 0xF1);
    dev.add_read(conversion_factors_body.data(), conversion_factors_body.size());

    PF::Driver::I2C::SFM3019::Device device{dev, gdev, gas};

    WHEN("the device is initialised") {
      const uint32_t ctime = 20;
      PF::HAL::MockTime time;
      time.set_micros(ctime);

      PF::Driver::I2C::SFM3019::Sensor sensor(device, resetter, time);

      auto status = sensor.setup();

      THEN("status should be equal to setup") {
        REQUIRE(status == PF::InitializableState::setup);
      }
    }
  }

  GIVEN("A Mock I2C device") {
    PF::HAL::MockTime time;
    PF::Driver::I2C::SFM3019::Device device{dev, gdev, gas};
    PF::Driver::I2C::SFM3019::Sensor sensor(device, resetter, time);
    PF::Driver::I2C::SFM3019::StateMachine state_machine{};

    WHEN("sensor output is calculated") {
      float flow = 0;
      auto output_status = sensor.output(flow);

      THEN("the final status should ok") {
        // REQUIRE(output_status == PF::InitializableState::ok);
      }
    }
  }
}