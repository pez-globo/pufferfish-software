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
#include "catch2/catch.hpp"
namespace PF = Pufferfish;

SCENARIO("setup method works correctly") {
  GIVEN("Device, time and resetter") {
    PF::HAL::Mock::I2CDevice mock_device;
    PF::HAL::Mock::I2CDevice global_device;
    auto read_buffer = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer.data(), read_buffer.size());
    auto read_buffer1 = PF::Util::Containers::make_array<uint8_t>(
        0x04, 0x02, 0x60, 0x06, 0x11, 0xa9, 0x00, 0x00, 0x81);
    mock_device.add_read(read_buffer1.data(), read_buffer1.size());
    auto input_data = PF::Util::Containers::make_array<uint8_t>(0xae, 0xdc, 0x6a);
    mock_device.add_read(input_data.data(), input_data.size());
    auto input_data1 = PF::Util::Containers::make_array<uint8_t>(0xae, 0xdc, 0x6a);
    mock_device.add_read(input_data1.data(), input_data1.size());

    PF::HAL::Mock::Time time;
    static const uint32_t read_conv_delay_us = 30000;
    PF::Driver::I2C::SFM3019::GasType gas = PF::Driver::I2C::SFM3019::GasType::o2;
    PF::Driver::I2C::SFM3019::Device device{mock_device, global_device, gas};
    PF::Driver::I2C::SFM3019::Sensor sensor{device, false, time};
    PF::Driver::I2C::SFM3019::ConversionFactors conversion_;
    auto status = sensor.setup();
    REQUIRE(status == PF::InitializableState::setup);
    // PF::HAL::Interfaces::Time &time;
    time.delay_micros(read_conv_delay_us);
    auto status1 = sensor.setup();
    REQUIRE(status1 == PF::InitializableState::setup);
    auto status2 = sensor.setup();
    REQUIRE(status2 == PF::InitializableState::setup);
    // auto status3 = sensor.setup();
    // REQUIRE(status3 == PF::InitializableState::ok);
    // float flow = 22.37647;
    // auto op_status = sensor.output(flow);
    // REQUIRE(op_status == PF::InitializableState::ok);
    //   float flow = 1;
    //   auto status1 = sensor.output(flow);
    //   REQUIRE(status1 == PF::InitializableState::ok);
  }
}