/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 * SFM3000.h
 *
 *      Author: March Boonyapaluk
 *
 *  A driver for the SFM3000 Low Pressure Drop Digital Flow Meter
 */

#pragma once

#include <array>
#include <cstdint>

#include "Pufferfish/Util/TaggedUnion.h"
#include "Pufferfish/Util/Vector.h"

namespace Pufferfish::Driver::Serial::FDO2 {

enum class CommandTypes : uint8_t { vers = 0, mraw = 1, logo = 2, bcst = 3, erro = 4 };

// Command headers
namespace Headers {
static const size_t length = 5;
static const char start = '#';
using Header = std::array<char, length>;

static const Header vers{{start, 'V', 'E', 'R', 'S'}};
static const Header mraw{{start, 'M', 'R', 'A', 'W'}};
static const Header logo{{start, 'L', 'O', 'G', 'O'}};
static const Header bcst{{start, 'B', 'C', 'S', 'T'}};
static const Header erro{{start, 'E', 'R', 'R', 'O'}};
}  // namespace Headers

// Command arguments
static const int arg_base = 10;
static const char arg_delimiter = ' ';
static const size_t max_field_digits = 11;  /// number of digits in -2^31

constexpr size_t max_frame_len(size_t num_fields) noexcept {
  return Headers::length + num_fields * (1 + max_field_digits) + 1;
}

// Responses

namespace Responses {

enum class ParseStatus { ok = 0, missing_arg, unexpected_arg, invalid_arg_delimiter };

static const size_t mraw_num_fields = 8;
// Note: this is optimized for MOXY, RDUM and WRUM may not fit as they require up to ~778 bytes
static const size_t max_len = max_frame_len(mraw_num_fields);
using ChunkBuffer = Util::Vector<char, max_len>;

struct Vers {
  uint8_t device_id;
  uint8_t num_channels;
  uint16_t firmware_rev;
  uint8_t type;  // TODO(lietk12): parse bits into bools
};
bool operator==(const Vers &left, const Vers &right);

struct Mraw {
  int32_t po2;
  int32_t temperature;
  uint32_t status;  // TODO(lietk12): parse bits into bools
  int32_t phase_shift;
  int32_t signal_intensity;
  int32_t ambient_light;
  int32_t ambient_pressure;
  int32_t relative_humidity;
};
struct Logo {
  uint16_t interval;
};
struct Bcst {
  uint16_t interval;
};
bool operator==(const Bcst &left, const Bcst &right);

struct Erro {
  int32_t code;
};

template <typename Response>
ParseStatus parse(const ChunkBuffer &input_buffer, Response &response);

union Union {
  Vers vers;
  Mraw mraw;
  Logo logo;
  Bcst bcst;
  Erro erro;
};

}  // namespace Responses
using Response = Util::TaggedUnion<Responses::Union, CommandTypes>;

// Requests

namespace Requests {

static const size_t max_num_fields = 2;
// Note: this excludes WRUM, which would require up to ~778 bytes
static const size_t max_len = max_frame_len(max_num_fields);
using ChunkBuffer = Util::Vector<char, max_len>;

struct Vers {};
struct Logo {};
struct Bcst {
  static constexpr const char *args_format = "%d";

  uint16_t interval;
};

template <typename Request>
void write(const Request &request, ChunkBuffer &output_buffer);

union Union {
  Vers vers;
  Logo logo;
  Bcst bcst;
};

}  // namespace Requests
using Request = Util::TaggedUnion<Requests::Union, CommandTypes>;

// Command sending/receiving

class CommandReceiver {
 public:
  enum class Status { ok = 0, invalid_header, invalid_args };

  static Status transform(const Responses::ChunkBuffer &input_buffer, Response &output_response);
};

class CommandSender {
 public:
  enum class Status { ok = 0, invalid_command };

  static Status transform(const Request &input_request, Requests::ChunkBuffer &output_buffer);
};

}  // namespace Pufferfish::Driver::Serial::FDO2
