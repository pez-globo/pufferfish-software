/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */

#pragma once

#include <Pufferfish/Driver/Serial/Nonin/FrameReceiver.h>

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

/**
 * @brief  Inline function to get the SpO2 data
 * @param  Spo2Data - Byte of SpO2 Data received from packet
 * @return Masked value of SpO2 from input Spo2Data
 */
inline uint16_t getSpO2Data(uint8_t Spo2Data) {
  /* Mask Bit0 to Bit6 for SpO2 data  */
  return (Spo2Data & 0x07F);
}

/**
 * @brief  Inline function to get the Heart rate from HR MSB and HR LSB
 * @param  hrMSB - Heart rate MSB
 * @param  hrLSB - Heart rate LSB
 * @return Calculated Heart Rate from MSB and LSB
 */
inline uint16_t getHeartRateData(uint8_t hrMSB  , uint8_t hrLSB) {
  /* Pack 2 bits of MSB and 6 bits of LSB for 9 bits of heart rate data  */
  return (((static_cast<uint16_t>(hrMSB) << 7) & 0x18) | (static_cast<uint16_t>(hrLSB) & 0x7F)) & 0x01FF;
}

/* Enum class for amplitude representation of signal quality  */
enum class SignalAmplitude {
  ok = 0,
  redPerfusion,   /// Red Perfusion – Amplitude representation of low signal quality
  yellowPerfusion,    /// YPRF: Yellow Perfusion – Amplitude representation of medium signal quality
  greenPerfusion      /// Green Perfusion – Amplitude representation of high signal quality
  };

/* Packet */
using Packet = std::array<Frame, 25>;

using SignalPerfusion = std::array<SignalAmplitude, 25>;

/* Structure defines the sensor data in packet for measurements */
struct PacketMeasurements {
  uint16_t heartRate;
  uint8_t  SpO2;
  uint8_t SpO2D;
  uint8_t SpO2DFast;
  uint8_t SpO2DBeat;
  uint16_t eHeartRate;
  uint8_t eSpO2;
  uint8_t eSpO2D;
  uint16_t HeartRateD;
  uint16_t eHeartRateD;
  uint8_t  noninOEMRevision;
};

/*
 * PacketReceiver class for
 */
class PacketReceiver {
 public:
  /* PacketReceiver Input status */
  enum class PacketInputStatus {
    available = 0,     /// Input is available to read output
    waiting,           /// Input is wait to read more bytes
    notAvailable
    };

  /* PacketReceiver Output status */
  enum class PacketOutputStatus {
    available = 0,   /// Output measurements are available
    waiting          /// Output is waiting to receive more byte for measurements
    };


  /* Structure defines the sensor data in packet for measurements */
  struct StatusByteError {
    bool sensorDisconnect;
    bool artifact;
    bool outOfTrack;
    bool sensorAlarm;
  };

  /**
   * @brief  Constructor for PacketReceiver
   * @param  None
   */
  PacketReceiver(){
  }

  /**
   * @brief  Reset the packet on error occurred in input
   * @param  None
   * @return None
   */
  void reset();

  /**
   * @brief  Input is called until packet is filled
   * @param  frame input frame of 5 bytes
   * @param  frameIndex frame index of a packet
   * @return Packet input status on available of packet
   */
  PacketInputStatus input(const Frame &frame, uint8_t frameIndex,
                          StatusByteError &frameErrorStatus,
                          SignalPerfusion &perfusionStatus);

  /**
   * @brief  Output is called after the input status is available to read measurements
   * @param  sensorMeasurements is updated on available of measurements
   * @return Packet Output status on available of measurements
   */
  PacketOutputStatus output(PacketMeasurements &sensorMeasurements);


 private:
  /*
   * @brief  Validates the status byte of frame
   * @param  byteValue input status byte value received from sensor
   * @param  statusByte structure is updated based on input byteValue
   * @return None
   */
  void readStatusByte(const uint8_t byteValue, uint8_t frameIndex,
                      StatusByteError &frameErrorStatus,
                      SignalPerfusion &perfusionStatus);

  /* */
  void readPacketMeasurements(PacketMeasurements &SensorMeasurements);

  /* Size of Packet */
  static const uint8_t packetSize = 25;

  /* Size of Frame */
  static const uint8_t PacketframeSize = FrameReceiver::frameSize;

  Packet packetData;

  bool packetReset;

  /* Packet Buffer length red from PacketReceiver input */
  uint8_t packetBufferLength;

  /* Input status for a packet */
  PacketInputStatus inputStatus;
};

} // Nonin
} // Serial
} // Driver
} // Pufferfish
