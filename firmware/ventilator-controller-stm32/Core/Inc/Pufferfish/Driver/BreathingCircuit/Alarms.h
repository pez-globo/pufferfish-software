/*
 * BreathingCircuit.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include "Pufferfish/Application/Alarms.h"
#include "Pufferfish/Application/mcu_pb.h"
#include "Pufferfish/Util/Containers/Array.h"

namespace Pufferfish::Driver::BreathingCircuit {

// These are all the alarms which the BreathingCircuit driver is responsible for.
// The driver resets their debouncers when ventilation is started/stopped or the mode is changed.
// The driver deactivates them when ventilation is stopped.
static const uint8_t debounce_max = 100;
static const uint32_t debounce_interval = 10;  // ms
static const Application::DebouncersInit debouncers = {
    {LogEventCode::LogEventCode_fio2_too_low, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_fio2_too_high, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_flow_too_low, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_flow_too_high, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_spo2_too_low, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_spo2_too_high, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_hr_too_low, {debounce_max, debounce_interval}},
    {LogEventCode::LogEventCode_hr_too_high, {debounce_max, debounce_interval}}};

// These are all the alarms which the BreathingCircuit driver has initialization wait timers for.
// The driver resets their initialization wait timers when ventilation is started/stopped or the
// mode is changed
static const uint32_t fio2_init_wait = 5000;  // ms
static const uint32_t flow_init_wait = 2000;  // ms
static const Application::InitWaitersInit init_waiters = {
    {LogEventCode::LogEventCode_fio2_too_low, {fio2_init_wait}},
    {LogEventCode::LogEventCode_fio2_too_high, {fio2_init_wait}},
    {LogEventCode::LogEventCode_flow_too_low, {flow_init_wait}},
    {LogEventCode::LogEventCode_flow_too_high, {flow_init_wait}}};

void reset_alarms(Application::AlarmsManager &alarms_manager);

}  // namespace Pufferfish::Driver::BreathingCircuit
