/*
 * Serial.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstdint>

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Util/Containers/Array.h"
#include "Pufferfish/Util/Enums.h"

namespace Pufferfish::Driver::Serial::Backend {

using Application::MessageTypes;

using ReceivableStates = Util::EnumValues<
    MessageTypes,
    MessageTypes::parameters_request,
    MessageTypes::alarm_limits_request,
    MessageTypes::expected_log_event,
    MessageTypes::alarm_mute_request>;

enum class StateSendEntryTypes : uint8_t { fast_sched = 0, slow_sched };
static const StateSendEntryTypes last_state_send_entry_type = StateSendEntryTypes::slow_sched;

static const auto state_send_root_sched = Util::Containers::make_array<StateSendEntryTypes>(
    StateSendEntryTypes::fast_sched,
    StateSendEntryTypes::slow_sched,
    StateSendEntryTypes::slow_sched);
static const auto state_send_fast_sched =
    Util::Containers::make_array<MessageTypes>(MessageTypes::sensor_measurements);
static const auto state_send_slow_sched = Util::Containers::make_array<MessageTypes>(
    MessageTypes::cycle_measurements,
    MessageTypes::parameters,
    MessageTypes::alarm_limits,
    MessageTypes::next_log_events,
    MessageTypes::active_log_events,
    MessageTypes::alarm_mute,
    MessageTypes::mcu_power_status);

}  // namespace Pufferfish::Driver::Serial::Backend
