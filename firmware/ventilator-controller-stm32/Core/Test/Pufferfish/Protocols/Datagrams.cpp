/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * Chunks.cpp
 *
 *  Created on: Nov 3, 2020
 *      Author: Renji Panicker
 *
 * Unit tests to confirm behavior of chunk splitter
 *
 */

#include "Pufferfish/Protocols/Datagrams.h"
#include "Pufferfish/Test/Util.h"

#include "catch2/catch.hpp"

namespace PF = Pufferfish;

SCENARIO("Protocols::Datagram behaves correctly", "[Datagram]") {
  GIVEN("A Datagram") {
    constexpr size_t buffer_size = 254UL;
    using TestDatagramProps = PF::Protocols::DatagramProps<buffer_size>;
    using TestDatagram = PF::Protocols::Datagram<TestDatagramProps::PayloadBuffer>;

    WHEN("a valid body is parsed") {
      auto body = std::string("\x01\x05\x01\x02\x03\x04\x05");
      auto expected_payload = std::string("\x01\x02\x03\x04\x05");

      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convertStringToByteVector(body, input_buffer);

      TestDatagramProps::PayloadBuffer payload;
      TestDatagram datagram{payload, 0};
      auto parse_status = datagram.parse(input_buffer);
      THEN("the final statuses should be ok") {
        REQUIRE(parse_status == PF::IndexStatus::ok);
        REQUIRE(payload == expected_payload);
        REQUIRE(datagram.length() == expected_payload.length());
      }
    }

    WHEN("an invalid body is parsed") {
      auto body = std::string("\x01\x00\x01\x02\x03\x04\x05");
      auto expected_payload = std::string("\x01\x02\x03\x04\x05");

      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convertStringToByteVector(body, input_buffer);

      TestDatagramProps::PayloadBuffer payload;
      TestDatagram datagram{payload, 0};
      auto parse_status = datagram.parse(input_buffer);
      THEN("the final statuses should be ok") {
        REQUIRE(parse_status != PF::IndexStatus::ok);
        REQUIRE(payload != expected_payload);
        REQUIRE(datagram.length() != expected_payload.length());
      }
    }

    WHEN("a valid payload is written") {
      auto body = std::string("\x01\x05\x01\x02\x03\x04\x05");
      auto expected_payload = std::string("\x01\x02\x03\x04\x05");

      PF::Util::ByteVector<buffer_size> input_buffer;
      PF::Util::convertStringToByteVector(body, input_buffer);

      TestDatagramProps::PayloadBuffer payload;
      TestDatagram datagram{payload, 0};
      auto parse_status = datagram.parse(input_buffer);
      THEN("the final statuses should be ok") {
        REQUIRE(parse_status == PF::IndexStatus::ok);
        REQUIRE(payload == expected_payload);
        REQUIRE(datagram.length() == expected_payload.length());
      }
    }

    PF::Util::ByteVector<buffer_size> output_buffer;
    WHEN("data is written to it") {
      TestDatagramProps::PayloadBuffer payload;
      TestDatagram datagram{payload, 0};
      auto write_status = datagram.write(output_buffer);
      THEN("the final status should be ok") {
        REQUIRE(write_status == PF::IndexStatus::ok);
      }
    }

    WHEN("data is written to it and parsed back") {
      TestDatagramProps::PayloadBuffer payload;
      TestDatagram datagram{payload, 0};
      auto write_status = datagram.write(output_buffer);
      auto parse_status = datagram.parse(output_buffer);
      THEN("the final statuses should be ok") {
        REQUIRE(write_status == PF::IndexStatus::ok);
        REQUIRE(parse_status == PF::IndexStatus::ok);
      }
    }
  }

  GIVEN("A Datagram") {
    constexpr size_t buffer_size = 254UL;
    using TestDatagramProps = PF::Protocols::DatagramProps<buffer_size>;
    using TestDatagram = PF::Protocols::Datagram<TestDatagramProps::PayloadBuffer>;
    TestDatagramProps::PayloadBuffer buffer;
    TestDatagram datagram{buffer};
    PF::Util::ByteVector<buffer_size> output_buffer;

    WHEN("data is written to it") {
      auto write_status = datagram.write(output_buffer);
      THEN("the final status should be ok") {
        REQUIRE(write_status == PF::IndexStatus::ok);
      }
    }

    WHEN("data is written to it and parsed back") {
      auto write_status = datagram.write(output_buffer);
      auto parse_status = datagram.parse(output_buffer);
      THEN("the final statuses should be ok") {
        REQUIRE(write_status == PF::IndexStatus::ok);
        REQUIRE(parse_status == PF::IndexStatus::ok);
      }
    }

  }

  GIVEN("A Datagram receiver") {
    constexpr size_t buffer_size = 254UL;
    using TestDatagramProps = PF::Protocols::DatagramProps<buffer_size>;
    using TestDatagram = PF::Protocols::Datagram<TestDatagramProps::PayloadBuffer>;
    TestDatagramProps::PayloadBuffer buffer;
    TestDatagram datagram{buffer};
    PF::Util::ByteVector<buffer_size> output_buffer;
    PF::Protocols::DatagramReceiver<buffer_size> datagram_receiver{};

    WHEN("data is written to it") {
      auto write_status = datagram.write(output_buffer);
      auto status = datagram_receiver.transform(output_buffer, datagram);
      THEN("the final status should be ok") {
        REQUIRE(write_status == PF::IndexStatus::ok);
        REQUIRE(status == PF::Protocols::DatagramReceiver<buffer_size>::Status::ok);
      }
    }
  }

  GIVEN("A Datagram sender") {
    constexpr size_t buffer_size = 254UL;
    using TestDatagramProps = PF::Protocols::DatagramProps<buffer_size>;
    using TestDatagram = PF::Protocols::Datagram<TestDatagramProps::PayloadBuffer>;
    TestDatagramProps::PayloadBuffer buffer;
    TestDatagram datagram{buffer};
    PF::Util::ByteVector<buffer_size> output_buffer;
    PF::Protocols::DatagramSender<buffer_size> datagram_sender{};

    WHEN("data is written to it") {
      auto write_status = datagram.write(buffer);
      PF::Util::ByteVector<buffer_size> output_buffer;
      auto status = datagram_sender.transform(buffer, output_buffer);
      THEN("the final status should be ok") {
        REQUIRE(write_status == PF::IndexStatus::ok);
        REQUIRE(status == PF::Protocols::DatagramSender<buffer_size>::Status::ok);
      }
    }
  }

}
