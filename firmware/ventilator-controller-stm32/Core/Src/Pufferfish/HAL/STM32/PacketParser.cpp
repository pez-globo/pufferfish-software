/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */
#include "Pufferfish/HAL/STM32/PacketParser.h"

namespace Pufferfish {
namespace HAL {

void PacketParser::reset() {

}

PacketMeasurements PacketParser::readPacketMeasurements(uint8_t *PacketBuffer) {
  uint8_t frameIndex, sensorData[25];

  /* Read the byte4 for sensor data */
  for (frameIndex = 0; frameIndex < 25; frameIndex++)
  {
    sensorData[frameIndex] = PacketBuffer[(frameIndex*5)+3];
  }

  /**
   * Heart Rate: 4-beat average values in standard mode.
   * E-HR MSB : sensorData 1st frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR LSB : sensorData 2nd frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  mMeasurements.heartRate = (((sensorData[0] << 7) & 0x18) | (sensorData[1] & 0x7F)) & 0x01FF;

  /**
   * SpO2: 4-beat average values in standard mode.
   * sensorData 3rd frame of packet contains SpO2-Bit6 t0 SpO2-Bit0 in bit6 to bit0
   */
  mMeasurements.SpO2 = sensorData[2] & 0x07F;

  /**
   * SpO2-D: 4-beat average displayed values in display mode
   * sensorData 9th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  mMeasurements.SpO2D = sensorData[8] & 0x07F;

  /**
   * SpO2 Fast: Non-slew limited saturation with 4-beat averaging in standard mode.
   * sensorData 10th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  mMeasurements.SpO2DFast = sensorData[9] & 0x07F;

  /**
   * SpO2 B-B: Un-averaged, non-slew limited, beat to beat value in standard mode
   * sensorData 11th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  mMeasurements.SpO2DBeat = sensorData[10] & 0x07F;

  /**
   * E-Heart Rate: 8-beat average values in standard mode.
   * E-HR MSB : sensorData 14th frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR LSB : sensorData 15th frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  mMeasurements.eHeartRate = (((sensorData[13] << 7) & 0x18) | (sensorData[14] & 0x7F)) & 0x01FF;

  /**
   * E-SpO2: 8-beat average values in standard mode.
   * sensorData 16th frame of packet contains SpO2-Bit6 t0 SpO2-Bit0 in bit6 to bit0
   */
  mMeasurements.eSpO2 = sensorData[15] & 0x07F;

  /**
   * E-SpO2-D: 8-beat average displayed values in display mode
   * sensorData 17th frame of packet contains SpO2-D-Bit6 to E-SpO2-D-Bit0 in bit6 to bit0
   */
  mMeasurements.eSpO2D = sensorData[16] & 0x07F;

  /**
   * HR-D: 8-beat average values in standard mode.
   * HR-D MSB : sensorData 20th frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * HR-D LSB : sensorData 21st frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  mMeasurements.HeartRateD = (((sensorData[19] << 7) & 0x18) | (sensorData[20] & 0x7F)) & 0x01FF;

  /**
   * E-HR-D: 8-beat average displayed values in display mode
   * E-HR-D MSB : sensorData 22nd frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR-D LSB : sensorData 23rd frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  mMeasurements.eHeartRateD = (((sensorData[21] << 7) & 0x18) | (sensorData[22] & 0x7F)) & 0x01FF;

  return mMeasurements;
}
PacketParser::PacketInputStatus PacketParser::input(uint8_t &readByte) {
  uint8_t frameByte, newFrame[PacketframeSize];

  if( frames.input(readByte) != FrameSplitter::frameInputStatus::available)
  {
    inputStatus = PacketInputStatus::notAvailable;
  }

  FrameSplitter::frameOutputStatus frameOutput = frames.output(newFrame);
  if (frameOutput == FrameSplitter::frameOutputStatus::available){
    for (frameByte = 0; frameByte < PacketframeSize;  frameByte ++)
    {
      PacketBuffer[(PacketLength*PacketframeSize) + frameByte] = newFrame[frameByte];
    }
    PacketLength++;

    /* Validate the buffer length is equal to expected frame size */
    if (PacketLength == PacketframeSize) {
      inputStatus = PacketInputStatus::available;
      return inputStatus;
    } else {
      inputStatus = PacketInputStatus::inputReady;
      return inputStatus;
    }
  } else if (frameOutput == FrameSplitter::frameOutputStatus::checksumError || frameOutput == FrameSplitter::frameOutputStatus::statusByteError){
    this->reset();
  }
  inputStatus = PacketInputStatus::notAvailable;
  return inputStatus;
}

PacketParser::PacketOutputStatus PacketParser::output(PacketMeasurements SensorMeasurements) {

  uint8_t byteIndex;
    /* Check for the frame availability in the buffer */
    if (inputStatus == PacketInputStatus::available){
      for (byteIndex = 0; byteIndex < (PacketLength*PacketframeSize); byteIndex++)
      {
        /* PacketBuffer */
        SensorMeasurements = this->readPacketMeasurements(PacketBuffer);
      }
      return PacketOutputStatus::available;

    } else {
      return PacketOutputStatus::waiting;
    }
}

} // HAL
} // Pufferfish
