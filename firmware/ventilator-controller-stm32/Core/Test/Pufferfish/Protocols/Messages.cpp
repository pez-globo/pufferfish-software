/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Messages.cpp
 *
 *  Created on: Nov 3, 2020
 *      Author: Rohan Purohit
 *
 * Unit tests to confirm behavior of Message class
 *
 */

#include "Pufferfish/Protocols/Messages.h"

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Application/mcu_pb.h"
#include "Pufferfish/Driver/Serial/Backend/Backend.h"
#include "Pufferfish/Test/Util.h"
#include "Pufferfish/Util/Array.h"
#include "Pufferfish/Util/Vector.h"
#include "catch2/catch.hpp"
#include "nanopb/pb.h"

namespace PF = Pufferfish;
namespace BE = PF::Driver::Serial::Backend;
using namespace std::string_literals;

static constexpr size_t num_descriptors = 8;

SCENARIO(
    "Protocols::The message correctly writes to the output buffer and also updates type",
    "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 252 "
      "bytes") {
    constexpr size_t payload_max_size = 252UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> output_buffer;

    REQUIRE(output_buffer.empty() == true);

    WHEN(
        "Write method is called on a message object whose payload.tag value is equal to the size "
        "of the descriptor array") {
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>()   // 1
      );

      SensorMeasurements sensor_measurements;
      memset(&sensor_measurements, 0, sizeof(sensor_measurements));
      sensor_measurements.flow = 60;
      test_message.payload.set(sensor_measurements);

      auto write_status = test_message.write(output_buffer, message_descriptors);

      THEN("The write method reports invalid type status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN("After the write method is called, the type member remains unchanged") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.flow == 60);
      }
      THEN("After the write method is called, the output buffer remains unchanged") {
        REQUIRE(output_buffer.empty() == true);
      }
    }

    WHEN(
        "Write method is called on a message object whose payload.tag value is greater than the "
        "size of the descriptor array") {
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 1
          PF::Util::get_protobuf_descriptor<SensorMeasurements>(),             // 2
          PF::Util::get_protobuf_descriptor<CycleMeasurements>()               // 3
      );

      ParametersRequest parameters_request;
      memset(&parameters_request, 0, sizeof(parameters_request));
      parameters_request.flow = 60;
      test_message.payload.set(parameters_request);

      auto write_status = test_message.write(output_buffer, message_descriptors);

      THEN("The write method reports invalid type status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN("After the write method is called, the type member remains unchanged") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.flow == 60);
      }
      THEN("After the write method is called, the output buffer remains unchanged") {
        REQUIRE(output_buffer.empty() == true);
      }
    }

    WHEN(
        "Write method is called on a message object whose payload.tag value corresponds to an "
        "Unrecognized message descriptor in the descriptor array") {
      test_message.payload.tag = PF::Application::MessageTypes::unknown;

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write status is equal to invalid type") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 0);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
      THEN("The payload.values field data remains unchanged") {
        ParametersRequest parameters_request;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.flow == 0);
      }
      THEN("After the write method is called, the output buffer remains unchanged") {
        REQUIRE(output_buffer.empty() == true);
      }
    }
  }
}

SCENARIO(
    "Protocols::The message correctly writes to the output buffer and also updates type for each "
    "message type",
    "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 252 "
      "bytes") {
    const auto exp_sensor_measurements =
        std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
    const auto exp_cycle_measurements =
        std::string("\x03\x1D\x00\x00\x20\x41\x3D\x00\x00\x96\x43"s);
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    const auto exp_parameters_request = std::string("\x05\x10\x06\x45\x00\x00\xA0\x42\x50\x01"s);
    const auto exp_alarm_limits = std::string("\x06\x12\x04\x08\x15\x10\x64"s);
    const auto exp_alarm_limits_request = std::string("\x07\x12\x04\x08\x32\x10\x5C"s);

    constexpr size_t payload_max_size = 252UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> output_buffer;

    REQUIRE(output_buffer.empty() == true);

    // sensor measurments
    WHEN("The payload is a sensor measurements message and write method is called") {
      SensorMeasurements sensor_measurements;
      memset(&sensor_measurements, 0, sizeof(sensor_measurements));
      sensor_measurements.flow = 30;
      sensor_measurements.fio2 = 85;
      sensor_measurements.spo2 = 72;

      test_message.payload.set(sensor_measurements);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type member is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.flow == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.fio2 == 85);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.spo2 == 72);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x02);
      }
      THEN("The output buffer has an expected sequence of 16 bytes") {
        REQUIRE(output_buffer == exp_sensor_measurements);
      }
    }

    // cycle measurments
    WHEN("The payload is a cycle measurements message and write method is called") {
      CycleMeasurements cycle_measurements;
      memset(&cycle_measurements, 0, sizeof(cycle_measurements));
      cycle_measurements.ve = 300;
      cycle_measurements.rr = 10;

      test_message.payload.set(cycle_measurements);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);
      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type member is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 3);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::cycle_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.ve == 300);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.rr == 10);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x03);
      }
      THEN("The output buffer has an expected sequence of 11 bytes") {
        REQUIRE(output_buffer == exp_cycle_measurements);
      }
    }

    // parameters
    WHEN("The payload is a parameters message and write method is called") {
      Parameters parameters;
      memset(&parameters, 0, sizeof(parameters));
      parameters.fio2 = 60;
      parameters.mode = VentilationMode_hfnc;
      parameters.ventilating = true;
      test_message.payload.set(parameters);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type member is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.fio2 == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.ventilating == true);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x04);
      }
      THEN("The output buffer has an expected sequence of 10 bytes") {
        REQUIRE(output_buffer == exp_parameters);
      }
    }

    // parameters request
    WHEN("The payload is a parameters request message and write method is called") {
      ParametersRequest parameters_request;
      memset(&parameters_request, 0, sizeof(parameters_request));
      parameters_request.fio2 = 80;
      parameters_request.mode = VentilationMode_hfnc;
      parameters_request.ventilating = true;

      test_message.payload.set(parameters_request);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type member is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.fio2 == 80);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.ventilating == true);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x05);
      }
      THEN("The output buffer has an expected sequence of 10 bytes") {
        REQUIRE(output_buffer == exp_parameters_request);
      }
    }

    // alarm limits
    WHEN("The payload is a alarm limits message") {
      AlarmLimits alarm_limits = {};
      Range range = {};
      range.lower = 21;
      range.upper = 100;
      alarm_limits.has_fio2 = true;
      alarm_limits.fio2 = range;

      test_message.payload.set(alarm_limits);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type member is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 6);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.lower == 21);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.upper == 100);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x06);
      }
      THEN("The output buffer has an expected sequence of 7 bytes") {
        REQUIRE(output_buffer == exp_alarm_limits);
      }
    }

    // alarm limits
    WHEN("The alarm limits has_fio2 value is false") {
      AlarmLimits alarm_limits = {};
      Range range = {};
      range.lower = 21;
      range.upper = 100;
      alarm_limits.has_fio2 = false;
      alarm_limits.fio2 = range;

      test_message.payload.set(alarm_limits);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 6);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.lower == 21);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.upper == 100);
      }
      THEN("The fio2 field is not written to the buffer") {
        auto expected = std::string("\06", 1);
        REQUIRE(output_buffer == expected);
      }
    }

    // alarm limits request
    WHEN("The payload is a alarm limits request message") {
      AlarmLimitsRequest alarm_limits_request = {};
      Range range = {};
      range.lower = 50;
      range.upper = 92;
      alarm_limits_request.has_fio2 = true;
      alarm_limits_request.fio2 = range;

      test_message.payload.set(alarm_limits_request);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 7);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.fio2.lower == 50);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.fio2.upper == 92);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x07);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_alarm_limits_request);
      }
    }

    // alarm limits request
    WHEN("The alarm limits request has_fio2 value is false") {
      AlarmLimitsRequest alarm_limits_request = {};
      Range range = {};
      range.lower = 50;
      range.upper = 92;
      alarm_limits_request.has_fio2 = false;
      alarm_limits_request.fio2 = range;

      test_message.payload.set(alarm_limits_request);

      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 7);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.fio2.lower == 50);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.fio2.upper == 92);
      }
      THEN("The fio2 field is not written to the buffer") {
        auto expected = std::string("\07", 1);
        REQUIRE(output_buffer == expected);
      }
    }

    // field descriptor and payload values are different, yet message status is not invalid encoding
    // WHEN("Unkown message data is written") {
    //   test_message.payload.tag = PF::Application::MessageTypes::parameters;

    //   CycleMeasurements cycle_measurements;
    //   memset(&cycle_measurements, 0, sizeof(cycle_measurements));
    //   cycle_measurements.ve = 300;
    //   cycle_measurements.rr = 10;
    //   test_message.payload.value.cycle_measurements = cycle_measurements;

    //   auto write_status = test_message.write(buffer, BE::message_descriptors);

    //   THEN("The write method reports ok status") {
    //     REQUIRE(write_status == PF::Protocols::MessageStatus::invalid_encoding);
    //   }
    // }
  }
}

SCENARIO(
    "Protocols::The message correctly writes to the output buffer and also updates type for "
    "different max sizes",
    "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 126 "
      "bytes") {
    const auto exp_sensor_measurements =
        std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
    const auto exp_cycle_measurements =
        std::string("\x03\x1D\x00\x00\x20\x41\x3D\x00\x00\x96\x43"s);
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    const auto exp_parameters_request = std::string("\x05\x10\x06\x45\x00\x00\xA0\x42\x50\x01"s);
    const auto exp_alarm_limits = std::string("\x06\x12\x04\x08\x15\x10\x64"s);
    const auto exp_alarm_limits_request = std::string("\x07\x12\x04\x08\x32\x10\x5C"s);

    constexpr size_t payload_max_size = 126UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 126UL;
    PF::Util::ByteVector<buffer_size> buffer;

    WHEN("The sensor measurments message data is written") {
      SensorMeasurements sensor_measurements;
      memset(&sensor_measurements, 0, sizeof(sensor_measurements));
      sensor_measurements.flow = 30;
      sensor_measurements.fio2 = 85;
      sensor_measurements.spo2 = 72;

      test_message.payload.set(sensor_measurements);

      auto write_status = test_message.write(buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.flow == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.fio2 == 85);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.spo2 == 72);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(buffer[0] == 0x02);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(buffer == exp_sensor_measurements);
      }
    }
  }

  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 14 "
      "bytes") {
    constexpr size_t output_size = 14UL;
    using TestMessage = PF::Protocols::
        Message<PF::Application::StateSegment, PF::Application::MessageTypeValues, output_size>;
    TestMessage test_message;
    WHEN("The output buffer size is too small to hold the encoded data") {
      PF::Util::ByteVector<output_size> output_buffer;
      SensorMeasurements sensor_measurements;
      memset(&sensor_measurements, 0, sizeof(sensor_measurements));
      sensor_measurements.flow = 30;
      sensor_measurements.fio2 = 85;
      sensor_measurements.spo2 = 72;

      test_message.payload.set(sensor_measurements);
      auto write_status = test_message.write(output_buffer, BE::message_descriptors);

      THEN("The write status is equal to invalid length") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::invalid_length);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        ParametersRequest parameters_request;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.flow == 0);
      }
      THEN("After the write method is called, the output buffer remains unchanged") {
        REQUIRE(output_buffer.empty() == true);
      }
    }
  }

  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 508 "
      "bytes") {
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    constexpr size_t payload_max_size = 508UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 508UL;
    PF::Util::ByteVector<buffer_size> buffer;

    WHEN("The sensor measurments message data is written") {
      Parameters parameters;
      memset(&parameters, 0, sizeof(parameters));
      parameters.fio2 = 60;
      parameters.mode = VentilationMode_hfnc;
      parameters.ventilating = true;
      test_message.payload.set(parameters);

      test_message.payload.set(parameters);

      auto write_status = test_message.write(buffer, BE::message_descriptors);

      THEN("The write method reports ok status") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the write method is called, the type field is set to the value of the message "
          "payload tag") {
        REQUIRE(test_message.type == 4);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(buffer[0] == 0x04);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(buffer == exp_parameters);
      }
    }
  }
}

SCENARIO(
    "Protocols::The Messages class correctly parses the input buffer and updates type and payload "
    "fields",
    "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 252 "
      "bytes") {
    constexpr size_t payload_max_size = 254UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> buffer;
    PF::IndexStatus push_status;

    WHEN("An empty input buffer body is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_length);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 0);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
    }

    WHEN(
        "A body with an empty payload and 1 byte header whose value is not included in "
        "MessageTypes enum") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      push_status = input_buffer.push_back(0x08);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 8);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
    }

    WHEN(
        "A body with an empty payload and 1 byte header of value (0x04) equal to the message "
        "descriptor "
        "array size is parsed") {
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 1
          PF::Util::get_protobuf_descriptor<SensorMeasurements>(),             // 2
          PF::Util::get_protobuf_descriptor<CycleMeasurements>()               // 3
      );

      PF::Util::ByteVector<buffer_size> input_buffer;
      push_status = input_buffer.push_back(0x04);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto parse_status = test_message.parse(input_buffer, message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field is set to parameters, which corresponds to value 0x04") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
    }

    WHEN(
        "A body with an empty payload and 1 byte header of value (0x05) equal to the message "
        "descriptor "
        "array size is parsed") {
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 1
          PF::Util::get_protobuf_descriptor<SensorMeasurements>(),             // 2
          PF::Util::get_protobuf_descriptor<CycleMeasurements>()               // 3
      );

      PF::Util::ByteVector<buffer_size> input_buffer;
      push_status = input_buffer.push_back(0x05);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto parse_status = test_message.parse(input_buffer, message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload.tag field is set to parameters, which corresponds to value 0x05") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
    }

    WHEN("A body with an encoded payload and 1 byte header of value equal to 0 is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      push_status = input_buffer.push_back(0x00);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 0);
      }
      THEN("The payload.tag field is set to Unknown") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
    }

    WHEN("A body with a 1-byte header of value 2 and a payload of null bytes is parsed") {
      constexpr size_t buffer_size = 253UL;
      PF::Util::ByteVector<buffer_size> buffer;
      buffer.push_back(0x02);
      buffer.resize(buffer_size);
      auto status = test_message.parse(buffer, BE::message_descriptors);

      THEN("The parse method reports invalid encoding status") {
        REQUIRE(status == PF::Protocols::MessageStatus::invalid_encoding);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field is set to sensor_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
    }

    WHEN(
        "A body with a 1 byte header of value 2 and payload of sensor_measurements message with "
        "invalid encoding is parsed") {
      constexpr size_t buffer_size = 253UL;
      PF::Util::ByteVector<buffer_size> buffer;
      auto data = std::string("\x02\x08\xa0\x10\x0A\x1D\x00\x00\xA0\x41"s);
      PF::Util::convert_string_to_byte_vector(data, buffer);

      auto status = test_message.parse(buffer, BE::message_descriptors);

      THEN("The parse method reports invalid encoding status") {
        REQUIRE(status == PF::Protocols::MessageStatus::invalid_encoding);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field is set to sensor_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
    }
  }
}

SCENARIO(
    "Protocols::The Messages class correctly parses the input buffer for each message type and "
    "updates type and payload "
    "fields",
    "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 252 "
      "bytes") {
    const auto exp_sensor_measurements =
        std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
    const auto exp_cycle_measurements =
        std::string("\x03\x1D\x00\x00\x20\x41\x3D\x00\x00\x96\x43"s);
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    const auto exp_parameters_request = std::string("\x05\x10\x06\x45\x00\x00\xA0\x42\x50\x01"s);
    const auto exp_alarm_limits = std::string("\x06\x12\x04\x08\x15\x10\x64"s);
    const auto exp_alarm_limits_request = std::string("\x07\x12\x04\x08\x32\x10\x5C"s);

    constexpr size_t payload_max_size = 254UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> buffer;

    // sensor measurements
    WHEN("A body with 1 byte header and a paylod of sensor measurments message type is parsed") {
      auto data = std::string("\x02\x08\x02\x10\x0A\x1D\x00\x00\xA0\x41"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field is set to sensor_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.cycle == 10);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.paw == 20);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.time == 2);
      }
      THEN("The input buffer is unchanged after parse") { REQUIRE(input_buffer == data); }
    }

    // cycle measurements
    WHEN("A body with 1 byte header and a paylod of cycle measurements message type is parsed") {
      auto data = std::string("\x03\x25\x00\x00\xF0\x41\x35\x00\x00\x20\x41"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(test_message.type == 3);
      }
      THEN("The payload.tag field is set to cycle_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::cycle_measurements);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.peep == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.ip == 10);
      }
      THEN("The input buffer is unchanged after parse") { REQUIRE(input_buffer == data); }
    }

    // parameters
    WHEN("A body with 1 byte header and a paylod of parameters message type is parsed") {
      auto data = std::string("\x04\x10\x06\x2D\x00\x00\x34\x42\x50\x01"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.vt == 45);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.ventilating == true);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.mode == VentilationMode_hfnc);
      }
      THEN("The input buffer is unchanged after parse") { REQUIRE(input_buffer == data); }
    }

    // parameters request
    WHEN("A body with 1 byte header and a paylod of parameters request message type is parsed") {
      auto data = std::string("\x05\x3D\x00\x00\xA0\x41"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload.tag field is set to parameters_request") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.ie == 20);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.time == 0);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.ventilating == false);
      }
      THEN("The input buffer is unchanged after parse") { REQUIRE(input_buffer == data); }
    }

    // alarm limits
    WHEN("A body with 1 byte header and a paylod of alarm limits message type is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_alarm_limits, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(test_message.type == 6);
      }
      THEN("The payload.tag field is set to alarm_limits") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.lower == 21);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.upper == 100);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.has_fio2 == true);
      }
      THEN("The input buffer is unchanged after parse") {
        REQUIRE(input_buffer == exp_alarm_limits);
      }
    }

    // alarm limits request
    WHEN("A body with 1 byte header and a paylod of alarm limits request message type is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_alarm_limits_request, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(test_message.type == 7);
      }
      THEN("The payload.tag field is set to alarm_limits_request") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits_request);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.lower == 50);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.upper == 92);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.has_fio2 == true);
      }
      THEN("The input buffer is unchanged after parse") {
        REQUIRE(input_buffer == exp_alarm_limits_request);
      }
    }

    // sensor measurements
    WHEN("A body with 1-byte header whose value is inconsistent with the payload message type") {
      auto data = std::string("\x04\x08\x02\x10\x0A\x1D\x00\x00\xA0\x41"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field is set to sensor_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.cycle == 10);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.paw == 20);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.time == 2);
      }
      THEN("The input buffer is unchanged after parse") { REQUIRE(input_buffer == data); }
    }
  }
}

SCENARIO(
    "The parse method correctly parses bodies when the payload and field members have data",
    "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion, default message types and a max "
      "size of 252 bytes") {
    constexpr size_t payload_max_size = 254UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 254UL;
    PF::Util::ByteVector<buffer_size> buffer;
    PF::IndexStatus push_status;

    ParametersRequest parameters_request;
    memset(&parameters_request, 0, sizeof(parameters_request));
    parameters_request.fio2 = 80;
    parameters_request.mode = VentilationMode_hfnc;
    parameters_request.ventilating = true;

    test_message.payload.set(parameters_request);
    test_message.type = 5;

    WHEN("An empty input buffer body is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_length);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member remains "
          "unchanged") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload Taggedunion tag is set to parameters_request") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("the payload.values field remain unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.fio2 == 80);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.ventilating == true);
      }
    }

    WHEN(
        "A body with an empty payload and 1 byte header whose value is not included in "
        "MessageTypes enum") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      push_status = input_buffer.push_back(0x08);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports invalid length status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 8);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
    }

    // cycle measurements
    WHEN("A body with 1 byte header and a paylod of cycle measurements message type is parsed") {
      auto data = std::string("\x03\x25\x00\x00\xF0\x41\x35\x00\x00\x20\x41"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto parse_status = test_message.parse(input_buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(test_message.type == 3);
      }
      THEN("The payload.tag field is set to cycle_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::cycle_measurements);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.peep == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.ip == 10);
      }
      THEN("The input buffer is unchanged after parse") { REQUIRE(input_buffer == data); }
    }
  }
}

SCENARIO(
    "Protocols::The Message class correctly writes to a buffer then parses from it", "[messages]") {
  GIVEN(
      "A Message object constructed with StateSegment Taggedunion and a payload of size 252 "
      "bytes") {
    constexpr size_t payload_max_size = 252UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    TestMessage parse_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> buffer;

    WHEN("The parameters request message type is written and then parsed from the buffer") {
      ParametersRequest parameters_request;
      memset(&parameters_request, 0, sizeof(parameters_request));
      parameters_request.ventilating = true;
      parameters_request.fio2 = 40;
      parameters_request.mode = VentilationMode_hfnc;
      parameters_request.flow = 60;
      test_message.payload.set(parameters_request);

      PF::Util::ByteVector<buffer_size> buffer;

      // write
      auto write_status = test_message.write(buffer, BE::message_descriptors);

      THEN("The status of write function should be ok") {
        REQUIRE(write_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(buffer[0] == 0x05);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.fio2 == 40);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.flow == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.ventilating == true);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        auto expected =
            std::string("\x05\x10\x06\x45\x00\x00\x20\x42\x4D\x00\x00\x70\x42\x50\x01"s);
        REQUIRE(buffer == expected);
      }

      // parse
      auto parse_status = parse_message.parse(buffer, BE::message_descriptors);

      THEN("The parse method reports ok status") {
        REQUIRE(parse_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(parse_message.type == 5);
      }
      THEN("The payload.tag field is set to parameters_request") {
        REQUIRE(parse_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.fio2 == 40);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.flow == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.ventilating == true);
      }
      THEN("The input buffer is unchanged after parse") {
        PF::Util::ByteVector<buffer_size> expected;
        expected.copy_from(buffer.buffer(), buffer.size());
        REQUIRE(buffer == expected);
      }

      // write
      PF::Util::ByteVector<buffer_size> output_buffer;
      auto status = parse_message.write(output_buffer, BE::message_descriptors);

      THEN("The status of write function should be ok") {
        REQUIRE(status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the parse method is called, The value assigned to the type member is equal to the "
          "input_buffer body's header") {
        REQUIRE(parse_message.type == 5);
      }
      THEN("The payload.tag field is set to parameters_request") {
        REQUIRE(parse_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.fio2 == 40);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.flow == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(parse_message.payload.value.parameters_request.ventilating == true);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x05);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == buffer);
      }
    }
  }
}

SCENARIO(
    "Protocols::The Message Receiver class correctly transforms messages into paylaods",
    "[messages]") {
  GIVEN("A MessageReceiver object is constructed with default parameters") {
    const auto exp_sensor_measurements =
        std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
    const auto exp_cycle_measurements =
        std::string("\x03\x1D\x00\x00\x20\x41\x3D\x00\x00\x96\x43"s);
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    const auto exp_parameters_request = std::string("\x05\x10\x06\x45\x00\x00\xA0\x42\x50\x01"s);
    const auto exp_alarm_limits = std::string("\x06\x12\x04\x08\x15\x10\x64"s);
    const auto exp_alarm_limits_request = std::string("\x07\x12\x04\x08\x32\x10\x5C"s);

    constexpr size_t payload_max_size = 254UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> buffer;

    PF::Protocols::MessageReceiver<TestMessage, num_descriptors> receiver{BE::message_descriptors};

    WHEN("An empty input buffer body is parsed") {
      PF::Protocols::MessageReceiver<TestMessage, num_descriptors> receiver{
          BE::message_descriptors};
      PF::Util::ByteVector<buffer_size> input_buffer;

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports invalid_length status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_length);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 0);
      }
      THEN("The payload.tag field of the output message is set to {Message type}") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer.empty() == true);
      }
    }

    WHEN(
        "A body with an empty payload and 1-byte header whose value is not included in "
        "MessageTypes enum") {
      constexpr size_t buffer_size = 253UL;
      PF::Protocols::MessageReceiver<TestMessage, num_descriptors> receiver{
          BE::message_descriptors};

      PF::Util::ByteVector<buffer_size> input_buffer;
      auto push_status = input_buffer.push_back(0x08);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports invalid_type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 8);
      }
      THEN("The payload.tag field of the output message remains unchanged") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
      THEN("The input buffer is unchanged after transform") {
        auto expected = std::string("\x08");
        REQUIRE(input_buffer == expected);
      }
    }

    WHEN(
        "A body with an empty payload and 1 byte header of value equal to the message descriptor "
        "array size is parsed") {
      constexpr size_t num_descriptors = 4;
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 1
          PF::Util::get_protobuf_descriptor<SensorMeasurements>(),             // 2
          PF::Util::get_protobuf_descriptor<CycleMeasurements>()               // 3
      );

      PF::Protocols::MessageReceiver<TestMessage, num_descriptors> receiver{message_descriptors};

      PF::Util::ByteVector<buffer_size> input_buffer;
      auto push_status = input_buffer.push_back(0x04);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports invalid_type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field of the output message is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("The input buffer is unchanged after transform") {
        auto expected = std::string("\x04");
        REQUIRE(input_buffer == expected);
      }
    }

    WHEN(
        "A body with an empty payload and 1 byte header of value equal to the message descriptor "
        "array size is parsed") {
      constexpr size_t num_descriptors = 4;
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 1
          PF::Util::get_protobuf_descriptor<SensorMeasurements>(),             // 2
          PF::Util::get_protobuf_descriptor<CycleMeasurements>()               // 3
      );

      PF::Util::ByteVector<buffer_size> input_buffer;
      auto push_status = input_buffer.push_back(0x05);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto transform_status = test_message.parse(input_buffer, message_descriptors);

      THEN("The transform method reports invalid_type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload.tag field of the output message is set to parameters_request") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("The input buffer is unchanged after transform") {
        auto expected = std::string("\x05");
        REQUIRE(input_buffer == expected);
      }
    }

    WHEN("A body with an encoded payload and 1-byte header of value equal to 0 is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      auto push_status = input_buffer.push_back(0x00);
      REQUIRE(push_status == PF::IndexStatus::ok);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports invalid_type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 0);
      }
      THEN("The payload.tag field of the output message is set to unknown") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::unknown);
      }
      THEN("The input buffer is unchanged after transform") {
        auto expected = std::string("\x00"s);
        REQUIRE(input_buffer == expected);
      }
    }

    WHEN(
        "A body with a 1-byte header of value 2 and payload of sensor_measurements message with "
        "invalid encoding is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      // std::string("\x02\x08\x02\x10\x0A\x1D\x00\x00\xA0\x41"); original buffer
      auto data = std::string(
          "\x02\x08\xa0\x10\x0A\x1D\x00\x00\xA0\x41"s);  // 3rd byte changed to random value
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports invalid encoding") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_encoding);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field of the output message is set to sensor_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The input buffer is unchanged after transform") { REQUIRE(input_buffer == data); }
    }

    WHEN("A MessageReceiver object is initialised with a smaller descriptors array") {
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),  // 0
          PF::Util::get_protobuf_descriptor<ParametersRequest>(),              // 1
          PF::Util::get_protobuf_descriptor<AlarmLimits>(),                    // 2
          PF::Util::get_protobuf_descriptor<AlarmLimitsRequest>(),             // 3
          PF::Util::get_protobuf_descriptor<Parameters>()                      // 4
      );
      constexpr size_t number_desc = 5;
      PF::Protocols::MessageReceiver<TestMessage, number_desc> receiver{message_descriptors};

      auto data = std::string("\x04\x4D\x00\x00\xF0\x41"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(data, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field of the output message is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.flow == 30);
      }
    }

    WHEN("The output message has some data prior to transform") {
      CycleMeasurements cycle_measurements;
      memset(&cycle_measurements, 0, sizeof(cycle_measurements));
      cycle_measurements.ve = 300;
      cycle_measurements.rr = 10;

      test_message.payload.set(cycle_measurements);

      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_parameters, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field of the output message is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.fio2 == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.ventilating == true);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_parameters);
      }
    }

    WHEN("The input buffer is changed after transform is called on it") {
      auto parameters_data = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(parameters_data, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field of the output message is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.fio2 == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.ventilating == true);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_parameters);
      }

      auto push_status = input_buffer.push_back(0x02);
      REQUIRE(push_status == PF::IndexStatus::ok);

      THEN("The payload.tag field of the output message is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.fio2 == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.ventilating == true);
      }
    }
  }
}

SCENARIO(
    "Protocols::The Message Receiver class correctly transforms messages into paylaods for each "
    "message type",
    "[messages]") {
  GIVEN("A MessageReceiver object is constructed with default parameters") {
    const auto exp_sensor_measurements =
        std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
    const auto exp_cycle_measurements =
        std::string("\x03\x1D\x00\x00\x20\x41\x3D\x00\x00\x96\x43"s);
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    const auto exp_parameters_request = std::string("\x05\x10\x06\x45\x00\x00\xA0\x42\x50\x01"s);
    const auto exp_alarm_limits = std::string("\x06\x12\x04\x08\x15\x10\x64"s);
    const auto exp_alarm_limits_request = std::string("\x07\x12\x04\x08\x32\x10\x5C"s);

    constexpr size_t payload_max_size = 254UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> buffer;

    PF::Protocols::MessageReceiver<TestMessage, num_descriptors> receiver{BE::message_descriptors};

    // sensor measurements
    WHEN("A body with 1 byte header and a paylod of sensor measurements is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_sensor_measurements, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 2);
      }
      THEN("The payload.tag field of the output message is set to sensor_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.flow == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.fio2 == 85);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.sensor_measurements.spo2 == 72);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_sensor_measurements);
      }
    }

    // cycle measurements
    WHEN("A body with 1 byte header and a paylod of cycle measurements is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_cycle_measurements, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 3);
      }
      THEN("The payload.tag field of the output message is set to cycle_measurements") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::cycle_measurements);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.ve == 300);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.cycle_measurements.rr == 10);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_cycle_measurements);
      }
    }

    // parameters
    WHEN("A body with 1 byte header and a paylod of parameters is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_parameters, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 4);
      }
      THEN("The payload.tag field of the output message is set to parameters") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.fio2 == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters.ventilating == true);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_parameters);
      }
    }

    // parameters request
    WHEN("A body with 1 byte header and a paylod of parameters request is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_parameters_request, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 5);
      }
      THEN("The payload.tag field of the output message is set to parameters_request") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.fio2 == 80);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.parameters_request.ventilating == true);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_parameters_request);
      }
    }

    // alarm limits
    WHEN("A body with 1 byte header and a paylod of alarm limits is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_alarm_limits, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 6);
      }
      THEN("The payload.tag field of the output message is set to alarm_limits") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits);
      }
      THEN("the payload.values field of the output message are as expected") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.lower == 21);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits.fio2.upper == 100);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_alarm_limits);
      }
    }

    // alarm limits request
    WHEN("A body with 1 byte header and a paylod of alarm limits request is parsed") {
      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convert_string_to_byte_vector(exp_alarm_limits_request, input_buffer);

      auto transform_status = receiver.transform(input_buffer, test_message);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN(
          "After the transform method is called, The type member of the output message is set to "
          "the type field of the input_buffer body's header") {
        REQUIRE(test_message.type == 7);
      }
      THEN("The payload.tag field of the output message is set to alarm_limits_request") {
        REQUIRE(test_message.payload.tag == PF::Application::MessageTypes::alarm_limits_request);
      }
      THEN("the payload.values field of the output message are as expected") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.has_fio2 == true);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.fio2.lower == 50);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(test_message.payload.value.alarm_limits_request.fio2.upper == 92);
      }
      THEN("The input buffer is unchanged after transform") {
        REQUIRE(input_buffer == exp_alarm_limits_request);
      }
    }
  }
}

SCENARIO(
    "Protocols::The Message Sender class correctly transforms payloads into messages",
    "[messages]") {
  GIVEN("A MessageSender object is constructed with default parameters") {
    const auto exp_sensor_measurements =
        std::string("\x02\x25\x00\x00\xF0\x41\x35\x00\x00\xAA\x42\x3D\x00\x00\x90\x42"s);
    const auto exp_cycle_measurements =
        std::string("\x03\x1D\x00\x00\x20\x41\x3D\x00\x00\x96\x43"s);
    const auto exp_parameters = std::string("\x04\x10\x06\x45\x00\x00\x70\x42\x50\x01"s);
    const auto exp_parameters_request = std::string("\x05\x10\x06\x45\x00\x00\xA0\x42\x50\x01"s);
    const auto exp_alarm_limits = std::string("\x06\x12\x04\x08\x15\x10\x64"s);
    const auto exp_alarm_limits_request = std::string("\x07\x12\x04\x08\x32\x10\x5C"s);

    using TestTaggedUnion = PF::Application::StateSegment;
    constexpr size_t payload_max_size = 252UL;
    using TestMessage = PF::Protocols::Message<
        PF::Application::StateSegment,
        PF::Application::MessageTypeValues,
        payload_max_size>;
    TestMessage test_message;
    constexpr size_t buffer_size = 252UL;
    PF::Util::ByteVector<buffer_size> output_buffer;
    PF::Application::StateSegment tagged_union;

    PF::Protocols::MessageSender<TestMessage, PF::Application::StateSegment, num_descriptors>
        sender{BE::message_descriptors};

    WHEN(
        "transform is called on a payload whose tag value is equal to the size of descriptor "
        "array") {
      constexpr size_t num_descriptors = 3;
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),
          PF::Util::get_protobuf_descriptor<SensorMeasurements>());

      PF::Protocols::MessageSender<TestMessage, PF::Application::StateSegment, num_descriptors>
          sender{message_descriptors};

      tagged_union.tag = PF::Application::MessageTypes::cycle_measurements;

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports invalid type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::cycle_measurements);
      }
    }

    WHEN(
        "transform is called on a payload whose tag value is greater than the size of descriptor "
        "array") {
      constexpr size_t num_descriptors = 3;
      constexpr auto message_descriptors = PF::Util::make_array<PF::Util::ProtobufDescriptor>(
          // array index should match the type code value
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),
          PF::Util::get_protobuf_descriptor<PF::Util::UnrecognizedMessage>(),
          PF::Util::get_protobuf_descriptor<SensorMeasurements>());

      PF::Protocols::MessageSender<TestMessage, PF::Application::StateSegment, num_descriptors>
          sender{message_descriptors};

      tagged_union.tag = PF::Application::MessageTypes::parameters;

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports invalid type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::parameters);
      }
    }

    WHEN(
        "transform is called on a payload whose payload.tag value corresponds to an Unrecognized "
        "message descriptor in the descriptor array") {
      tagged_union.tag = PF::Application::MessageTypes::unknown;

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports invalid type status") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_type);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::unknown);
      }
    }

    // sensor measurments
    WHEN("The sensor measurments data from the message is written to the buffer") {
      constexpr size_t payload_max_size = 14UL;
      using TestMessage = PF::Protocols::Message<
          PF::Application::StateSegment,
          PF::Application::MessageTypeValues,
          payload_max_size>;
      constexpr size_t output_size = 14UL;
      PF::Util::ByteVector<output_size> output_buffer;

      PF::Protocols::MessageSender<TestMessage, PF::Application::StateSegment, num_descriptors>
          sender{BE::message_descriptors};

      SensorMeasurements sensor_measurements;
      memset(&sensor_measurements, 0, sizeof(sensor_measurements));
      sensor_measurements.flow = 30;
      sensor_measurements.fio2 = 85;
      sensor_measurements.spo2 = 72;
      test_message.payload.set(sensor_measurements);

      PF::Application::StateSegment tagged_union;
      tagged_union.set(sensor_measurements);

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::invalid_length);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.sensor_measurements.flow == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.sensor_measurements.fio2 == 85);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.sensor_measurements.spo2 == 72);
      }
      THEN("The output buffer remains unchanged") { REQUIRE(output_buffer.empty() == true); }
    }

    // sensor measurments
    WHEN("The sensor measurments data from the message is written to the buffer") {
      SensorMeasurements sensor_measurements;
      memset(&sensor_measurements, 0, sizeof(sensor_measurements));
      sensor_measurements.flow = 30;
      sensor_measurements.fio2 = 85;
      sensor_measurements.spo2 = 72;
      test_message.payload.set(sensor_measurements);

      PF::Application::StateSegment tagged_union;
      tagged_union.set(sensor_measurements);

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::sensor_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.sensor_measurements.flow == 30);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.sensor_measurements.fio2 == 85);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.sensor_measurements.spo2 == 72);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x02);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_sensor_measurements);
      }
    }

    // cycle measurements
    WHEN("The cycle measurments data from the message is written to the buffer") {
      CycleMeasurements cycle_measurements;
      memset(&cycle_measurements, 0, sizeof(cycle_measurements));
      cycle_measurements.ve = 300;
      cycle_measurements.rr = 10;

      tagged_union.set(cycle_measurements);

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::cycle_measurements);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.cycle_measurements.ve == 300);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.cycle_measurements.rr == 10);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x03);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_cycle_measurements);
      }
    }

    // parameters
    WHEN("The parameters data from the message is written to the buffer") {
      Parameters parameters;
      memset(&parameters, 0, sizeof(parameters));
      parameters.fio2 = 60;
      parameters.mode = VentilationMode_hfnc;
      parameters.ventilating = true;
      tagged_union.set(parameters);

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::parameters);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.parameters.fio2 == 60);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.parameters.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.parameters.ventilating == true);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x04);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_parameters);
      }
    }

    // parameters request
    WHEN("The parameters request data from the message is written to the buffer") {
      ParametersRequest parameters_request;
      memset(&parameters_request, 0, sizeof(parameters_request));
      parameters_request.fio2 = 80;
      parameters_request.mode = VentilationMode_hfnc;
      parameters_request.ventilating = true;

      tagged_union.set(parameters_request);

      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::parameters_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.parameters_request.fio2 == 80);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.parameters_request.mode == VentilationMode_hfnc);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.parameters_request.ventilating == true);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x05);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_parameters_request);
      }
    }

    // alarm limits
    WHEN("The alarm limits data from the message is written to the buffer") {
      AlarmLimits alarm_limits = {};
      Range range = {};
      range.lower = 21;
      range.upper = 100;
      alarm_limits.has_fio2 = true;
      alarm_limits.fio2 = range;

      tagged_union.set(alarm_limits);
      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::alarm_limits);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.alarm_limits.fio2.lower == 21);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.alarm_limits.fio2.upper == 100);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x06);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_alarm_limits);
      }
    }

    // alarm limits request
    WHEN("The alarm limits request data from the message is written to the buffer") {
      AlarmLimitsRequest alarm_limits_request = {};
      Range range = {};
      range.lower = 50;
      range.upper = 92;
      alarm_limits_request.has_fio2 = true;
      alarm_limits_request.fio2 = range;

      tagged_union.set(alarm_limits_request);
      auto transform_status = sender.transform(tagged_union, output_buffer);

      THEN("The transform method reports ok") {
        REQUIRE(transform_status == PF::Protocols::MessageStatus::ok);
      }
      THEN("The payload.tag field remains unchanged") {
        REQUIRE(tagged_union.tag == PF::Application::MessageTypes::alarm_limits_request);
      }
      THEN("The payload.values field data remains unchanged") {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.alarm_limits_request.fio2.lower == 50);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access);
        REQUIRE(tagged_union.value.alarm_limits_request.fio2.upper == 92);
      }
      THEN(
          "The type field of the body's header correctly stores the value of message type obtained "
          "from the message payload tag") {
        REQUIRE(output_buffer[0] == 0x07);
      }
      THEN("The output buffer has an expected sequence of bytes") {
        REQUIRE(output_buffer == exp_alarm_limits_request);
      }
    }
  }
}
