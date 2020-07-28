/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#pragma once

namespace Pufferfish {

/**
 * Possible alarms that could be raised by the system, must by sorted by
 * priority in ascending order
 */
enum class BufferReadStatus {
  ok = 0,  /// buffer read succeeded
  empty  /// buffer is empty so no read is possible
};

enum class BufferWriteStatus {
  ok = 0,   /// buffer write succeeded
  full  /// buffer is full so no write is possible
};

/**
 * Possible alarms that could be raised by the system, must by sorted by
 * priority in ascending order
 */
enum class AlarmStatus {
  highPriority = 0,  /// High Priority Alarm as defined in IEC 60601-1-8
  mediumPriority,  /// Medium Priority Alarm as defined in IEC 60601-1-8
  lowPriority,  /// Low Priority Alarm as defined in IEC 60601-1-8
  technical1,  /// Technical Alarm 1 for debugging
  technical2,  /// Technical Alarm 2 for debugging
  noAlarm  /// no alarm is present, must be last
};

/**
 * Error code of the Alarm Manager module
 */
enum class AlarmManagerStatus {
  ok = 0, /// no error
  invalidAlarm, /// invalid alarm in argument
  notSet, /// the specified alarm has not been set
  hardwareError /// there is a hardware error when driving an alarm
};
  
/**
 * An outcome of performing an operation on I2C bus
 */
enum class I2CDeviceStatus {
  ok = 0, /// success
  notSupported, /// the operation is not supported
  writeError, /// an error occur when writing to an I2C device
  readError, /// an error occur when reading from an I2C device
  invalidArguments, /// invalid input to the function
  crcCheckFailed, /// The CRC code received is inconsistent
  invalidExtSlot, /// The MUX slot of ExtendedI2CDevice is invalid
  testFailed, /// unit tests are failing
  noNewData /// no new data is received from the sensor
};

}  // namespace Pufferfish
