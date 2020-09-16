/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Hemanth Gowda S
 */
#include "Pufferfish/Driver/Serial/Nonin/PacketReceiver.h"

namespace Pufferfish {
namespace Driver {
namespace Serial {
namespace Nonin {

void PacketReceiver::readStatusByte(const uint8_t byteValue, uint8_t frameIndex,
                                  StatusByteError &frameErrorStatus,
                                  SignalPerfusion &perfusionStatus)
{
  /* BIT6: SNSD: Sensor Disconnect – Sensor is not connected to oximeter or sensor is inoperable */
  frameErrorStatus.sensorDisconnect = ((byteValue & 0x40) == 0x00) ? false : true;
  /* BIT5: ARTF: Artifact – A detected pulse beat didn’t match the current pulse interval */
  frameErrorStatus.artifact = ((byteValue & 0x20) == 0x00) ? false : true;
  /* BIT4: OOT: Out Of Track – An absence of consecutive good pulse signals */
  frameErrorStatus.outOfTrack = ((byteValue & 0x10) == 0x00) ? false : true;
  /* BIT3: SNSA: Sensor Alarm – Sensor is providing unusable data for analysis */
  frameErrorStatus.sensorAlarm = ((byteValue & 0x08) == 0x00) ? false : true;

  /**
   * BIT2 and BIT 1: YPRF: Yellow Perfusion – Amplitude representation of medium signal quality.
   * BIT2: RPRF: Red Perfusion – Amplitude representation of low signal quality.
   * BIT1: GPRF: Green Perfusion – Amplitude representation of high signal quality.
   */
  if (byteValue & 0x06) {
    perfusionStatus[frameIndex] = SignalAmplitude::yellowPerfusion;
  } else if (byteValue & 0x04) {
    perfusionStatus[frameIndex] = SignalAmplitude::redPerfusion;
  } else if (byteValue & 0x02) {
    perfusionStatus[frameIndex] = SignalAmplitude::greenPerfusion;
  } else {
    perfusionStatus[frameIndex] = SignalAmplitude::ok;
  }
}

void PacketReceiver::readPacketMeasurements(PacketMeasurements &sensorMeasurements) {
  /* 4th byte of a frame contains heart beat and SpO2 data */
  const uint8_t Byte3 = 3;

  /**
   * Heart Rate: 4-beat average values in standard mode.
   * E-HR MSB : sensorData 1st frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR LSB : sensorData 2nd frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.heartRate = getHeartRateData(packetData[0][Byte3], packetData[1][Byte3]);

  /**
   * SpO2: 4-beat average values in standard mode.
   * sensorData 3rd frame of packet contains SpO2-Bit6 t0 SpO2-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2 = getSpO2Data(packetData[2][Byte3]);

  /**
   * Nonin Oximeter Firmware Revision Level
   * sensorData 4th frame of packet contains Firmware Revision Level
   */
  sensorMeasurements.noninOEMRevision = getSpO2Data(packetData[3][Byte3]);

  /**
   * SpO2-D: 4-beat average displayed values in display mode
   * sensorData 9th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2D = getSpO2Data(packetData[8][Byte3]);

  /**
   * SpO2 Fast: Non-slew limited saturation with 4-beat averaging in standard mode.
   * sensorData 10th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2DFast = getSpO2Data(packetData[9][Byte3]);

  /**
   * SpO2 B-B: Un-averaged, non-slew limited, beat to beat value in standard mode
   * sensorData 11th frame of packet contains SpO2-D-Bit6 t0 SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.SpO2DBeat = getSpO2Data(packetData[10][Byte3]);

  /**
   * E-Heart Rate: 8-beat average values in standard mode.
   * E-HR MSB : sensorData 14th frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR LSB : sensorData 15th frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eHeartRate = getHeartRateData(packetData[13][Byte3], packetData[14][Byte3]);

  /**
   * E-SpO2: 8-beat average values in standard mode.
   * sensorData 16th frame of packet contains SpO2-Bit6 t0 SpO2-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eSpO2 = getSpO2Data(packetData[15][Byte3]);

  /**
   * E-SpO2-D: 8-beat average displayed values in display mode
   * sensorData 17th frame of packet contains SpO2-D-Bit6 to E-SpO2-D-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eSpO2D = getSpO2Data(packetData[16][Byte3]);

  /**
   * HR-D: 8-beat average values in standard mode.
   * HR-D MSB : sensorData 20th frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * HR-D LSB : sensorData 21st frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.HeartRateD = getHeartRateData(packetData[19][Byte3], packetData[20][Byte3]);

  /**
   * E-HR-D: 8-beat average displayed values in display mode
   * E-HR-D MSB : sensorData 22nd frame of packet contains HR-Bit8 and HR-Bit7 in bit1 and bit0
   * E-HR-D LSB : sensorData 23rd frame of packet contains HR-Bit6 t0 HR-Bit0 in bit6 to bit0
   */
  sensorMeasurements.eHeartRateD = getHeartRateData(packetData[21][Byte3], packetData[22][Byte3]);

}

PacketReceiver::PacketInputStatus PacketReceiver::input(const Frame &frame, uint8_t frameIndex,
                                                    StatusByteError &frameErrorStatus,
                                                    SignalPerfusion &perfusionStatus)
{
  const uint8_t StatusError = 0x78;
  /* */
  packetBufferLength = frameIndex;

  /* Read the status byte errors */
  this->readStatusByte(frame[1], frameIndex, frameErrorStatus, perfusionStatus);

  /* Validate errors for SNSD(bit6), ARTF(bit5), OOT(bit4) and SNSA(bit3) */
  if((frame[1] & StatusError) != 0x00) {
    packetReset = true;
    inputStatus = PacketInputStatus::notAvailable;
    return inputStatus;
  }

  /* Update the frame received to packet */
  packetData[packetBufferLength] = frame;

  /* Check for the packet data is complete */
  if ( (packetBufferLength + 1) != packetSize) {
    /* Update input status is waiting to receive more frames */
    inputStatus = PacketInputStatus::waiting;
    return inputStatus;
  }

  /* Update input status is available for measurements */
  inputStatus = PacketInputStatus::available;
  return inputStatus;
}

PacketReceiver::PacketOutputStatus PacketReceiver::output(PacketMeasurements &SensorMeasurements) {
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
