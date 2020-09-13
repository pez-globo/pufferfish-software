/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */
#include "Pufferfish/Driver/Serial/Nonin/PacketParser.h"
#include "Pufferfish/HAL/STM32/Time.h"

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

void PacketParser::readPacketMeasurements(PacketMeasurements &sensorMeasurements) {
  /* 4th byte of a frame contains heart beat and SpO2 data */
  const uint8_t sensorDataByte = 3;

  /**
   * Heart Rate: 4-beat average values in standard mode.
   * E-HR MSB : sensorData 1st frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR LSB : sensorData 2nd frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.heartRate = (((packetData[0][sensorDataByte] << 7) & 0x18) |
                                   (packetData[1][sensorDataByte] & 0x7F)) & 0x01FF;

  /**
   * SpO2: 4-beat average values in standard mode.
   * sensorData 3rd frame of packet contains SpO2-Bit6 t0 SpO2-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2 = packetData[2][sensorDataByte] & 0x07F;

  /**
   * Nonin Oximeter Firmware Revision Level
   * sensorData 4th frame of packet contains Firmware Revision Level
   */
  sensorMeasurements.noninOEMRevision = packetData[3][sensorDataByte];

  /**
   * SpO2-D: 4-beat average displayed values in display mode
   * sensorData 9th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2D = packetData[8][sensorDataByte] & 0x07F;

  /**
   * SpO2 Fast: Non-slew limited saturation with 4-beat averaging in standard mode.
   * sensorData 10th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2DFast = packetData[9][sensorDataByte] & 0x07F;

  /**
   * SpO2 B-B: Un-averaged, non-slew limited, beat to beat value in standard mode
   * sensorData 11th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2DBeat = packetData[10][sensorDataByte]& 0x07F;

  /**
   * E-Heart Rate: 8-beat average values in standard mode.
   * E-HR MSB : sensorData 14th frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR LSB : sensorData 15th frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eHeartRate = (((packetData[13][sensorDataByte] << 7) & 0x18) |
                                    (packetData[14][sensorDataByte] & 0x7F)) & 0x01FF;

  /**
   * E-SpO2: 8-beat average values in standard mode.
   * sensorData 16th frame of packet contains SpO2-Bit6 t0 SpO2-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eSpO2 = packetData[15][sensorDataByte] & 0x07F;

  /**
   * E-SpO2-D: 8-beat average displayed values in display mode
   * sensorData 17th frame of packet contains SpO2-D-Bit6 to E-SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eSpO2D = packetData[16][sensorDataByte] & 0x07F;

  /**
   * HR-D: 8-beat average values in standard mode.
   * HR-D MSB : sensorData 20th frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * HR-D LSB : sensorData 21st frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.HeartRateD = (((packetData[19][sensorDataByte] << 7) & 0x18) |
                                    (packetData[20][sensorDataByte] & 0x7F)) & 0x01FF;

  /**
   * E-HR-D: 8-beat average displayed values in display mode
   * E-HR-D MSB : sensorData 22nd frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR-D LSB : sensorData 23rd frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eHeartRateD = (((packetData[21][sensorDataByte] << 7) & 0x18) |
                                     (packetData[22][sensorDataByte] & 0x7F)) & 0x01FF;

}

PacketParser::PacketInputStatus PacketParser::input(std::array<uint8_t, FrameReceiver::frameSize> frame, uint8_t frameIndex)
{
  uint8_t frameByte;
  /* */
  packetBufferLength = frameIndex;

  /* Update the frame to packet */
  for (frameByte = 0; frameByte < PacketframeSize; frameByte ++)
  {
    packetData[packetBufferLength][frameByte] = frame[frameByte];
  }
  /* Check for the packet data is complete */
  if ( (packetBufferLength + 1) == packetSize) {
    /* Update input status is available for measurements */
    inputStatus = PacketInputStatus::available;
  } else {
    /* Update input status is waiting to recieve more frames */
    inputStatus = PacketInputStatus::waiting;
  }
  return inputStatus;
}

PacketParser::PacketOutputStatus PacketParser::output(PacketMeasurements &SensorMeasurements) {
    /* Check for the frame availability in the buffer */
    if (inputStatus == PacketInputStatus::available){
      /* Read PacketBuffer and Update the measurements */
      this->readPacketMeasurements(SensorMeasurements);
      /* Return Packet Output status as available */
      return PacketOutputStatus::available;

    } else {
      /* Return PacketOutputStatus as waiting to receive packet data */
      return PacketOutputStatus::waiting;
    }
}

} // Nonin
} // Serial
} // Driver
} // Pufferfish
