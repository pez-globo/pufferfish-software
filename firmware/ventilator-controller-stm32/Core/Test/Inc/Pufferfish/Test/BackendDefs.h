#pragma once

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Application/mcu_pb.h"
#include "Pufferfish/Protocols/Messages.h"
#include "Pufferfish/Protocols/States.h"
#include "Pufferfish/Util/Array.h"

namespace Pufferfish::Driver::Serial::Backend {

// State Synchronization

using StateOutputScheduleEntry = Protocols::StateOutputScheduleEntry<Application::MessageTypes>;

constexpr auto state_sync_schedule = Util::make_array<const StateOutputScheduleEntry>(
    StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::parameters},
    StateOutputScheduleEntry{10, Application::MessageTypes::alarm_limits},
    StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::cycle_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::alarm_limits_request},
    StateOutputScheduleEntry{10, Application::MessageTypes::sensor_measurements},
    StateOutputScheduleEntry{10, Application::MessageTypes::parameters_request},
    StateOutputScheduleEntry{10, Application::MessageTypes::cycle_measurements});
}  // namespace Pufferfish::Driver::Serial::Backend
