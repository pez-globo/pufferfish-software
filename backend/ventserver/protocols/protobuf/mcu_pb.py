# Generated by the protocol buffer compiler.  DO NOT EDIT!
# sources: mcu_pb.proto
# plugin: python-betterproto
from dataclasses import dataclass
from typing import List

import betterproto


class VentilationMode(betterproto.Enum):
    hfnc = 0
    pc_ac = 1
    vc_ac = 2
    niv_pc = 3
    niv_ps = 4
    psv = 5
    prvc = 6


class LogEventCode(betterproto.Enum):
    # Patient alarms
    fio2_too_low = 0
    fio2_too_high = 1
    flow_too_low = 2
    flow_too_high = 3
    spo2_too_low = 4
    spo2_too_high = 5
    hr_too_low = 6
    hr_too_high = 7
    # Control settings
    ventilation_operation_changed = 64
    ventilation_mode_changed = 65
    fio2_setting_changed = 66
    flow_setting_changed = 67
    # Alarm limits settings
    fio2_alarm_limits_changed = 80
    flow_alarm_limits_changed = 81
    spo2_alarm_limits_changed = 82
    hr_alarm_limits_changed = 83
    # System settings & alarms
    screen_locked = 129
    mcu_backend_connection_down = 130
    backend_mcu_connection_down = 131
    backend_frontend_connection_down = 132
    frontend_backend_connection_down = 133
    mcu_backend_connection_up = 134
    # The following code isn't actually used, but we reserve space for it. We
    # don't use it because if the backend received an mcu_backend_connection_up
    # event from the MCU, then we know that the backend received a connection
    # from the MCU. We only care for technical troubleshooting (of the UART
    # wires) about the case where the backend receives a connection from the MCU
    # but the MCU hasn't received a connection from the backend; it would be good
    # to log it, but we don't need to show it in the frontend, and right now the
    # frontend has no way to filter out events from its display.
    # backend_mcu_connection_up = 135;  // backend detected mcu
    backend_frontend_connection_up = 136
    # The following code isn't actually used, but we reserve space for it. We
    # don't use it because the frontend can't generate LogEvents with IDs.
    # frontend_backend_connection_up = 137;
    battery_low = 138
    battery_critical = 139
    charger_disconnected = 140
    mcu_started = 141
    backend_started = 142
    mcu_shutdown = 143
    backend_shutdown = 144
    # Alarm muting/unmuting
    alarms_muted_user_software = 145
    alarms_muted_user_hardware = 146
    alarms_muted_unknown = 147
    alarms_unmuted_user_software = 148
    alarms_unmuted_user_hardware = 149
    alarms_unmuted_initialization = 150
    alarms_unmuted_timeout = 151
    alarms_unmuted_mcu_backend_loss = 152
    alarms_unmuted_backend_mcu_loss = 153
    alarms_unmuted_backend_frontend_loss = 154
    alarms_unmuted_frontend_backend_loss = 155
    alarms_unmuted_unknown = 156
    # Sensor loss
    sfm3019_air_disconnected = 160
    sfm3019_o2_disconnected = 161
    fdo2_disconnected = 162


class LogEventType(betterproto.Enum):
    patient = 0
    control = 1
    alarm_limits = 2
    system = 3


class AlarmMuteSource(betterproto.Enum):
    initialization = 0
    user_software = 1
    user_hardware = 2
    timeout = 3
    mcu_backend_loss = 4
    backend_mcu_loss = 5
    backend_frontend_loss = 6
    frontend_backend_loss = 7


@dataclass
class SensorMeasurements(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    cycle: int = betterproto.uint32_field(2)
    fio2: float = betterproto.float_field(3)
    flow: float = betterproto.float_field(4)
    spo2: float = betterproto.float_field(5)
    hr: float = betterproto.float_field(6)
    paw: float = betterproto.float_field(7)
    volume: float = betterproto.float_field(8)


@dataclass
class CycleMeasurements(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    vt: float = betterproto.float_field(2)
    rr: float = betterproto.float_field(3)
    peep: float = betterproto.float_field(4)
    pip: float = betterproto.float_field(5)
    ip: float = betterproto.float_field(6)
    ve: float = betterproto.float_field(7)


@dataclass
class Parameters(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    ventilating: bool = betterproto.bool_field(2)
    mode: "VentilationMode" = betterproto.enum_field(3)
    fio2: float = betterproto.float_field(4)
    flow: float = betterproto.float_field(5)
    pip: float = betterproto.float_field(6)
    peep: float = betterproto.float_field(7)
    vt: float = betterproto.float_field(8)
    rr: float = betterproto.float_field(9)
    ie: float = betterproto.float_field(10)


@dataclass
class ParametersRequest(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    ventilating: bool = betterproto.bool_field(2)
    mode: "VentilationMode" = betterproto.enum_field(3)
    fio2: float = betterproto.float_field(4)
    flow: float = betterproto.float_field(5)
    pip: float = betterproto.float_field(6)
    peep: float = betterproto.float_field(7)
    vt: float = betterproto.float_field(8)
    rr: float = betterproto.float_field(9)
    ie: float = betterproto.float_field(10)


@dataclass
class Range(betterproto.Message):
    lower: int = betterproto.int32_field(1)
    upper: int = betterproto.int32_field(2)


@dataclass
class AlarmLimits(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    fio2: "Range" = betterproto.message_field(2)
    flow: "Range" = betterproto.message_field(3)
    spo2: "Range" = betterproto.message_field(4)
    hr: "Range" = betterproto.message_field(5)
    rr: "Range" = betterproto.message_field(6)
    pip: "Range" = betterproto.message_field(7)
    peep: "Range" = betterproto.message_field(8)
    ip_above_peep: "Range" = betterproto.message_field(9)
    insp_time: "Range" = betterproto.message_field(10)
    paw: "Range" = betterproto.message_field(11)
    mve: "Range" = betterproto.message_field(12)
    tv: "Range" = betterproto.message_field(13)
    etco2: "Range" = betterproto.message_field(14)
    apnea: "Range" = betterproto.message_field(15)


@dataclass
class AlarmLimitsRequest(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    fio2: "Range" = betterproto.message_field(2)
    flow: "Range" = betterproto.message_field(3)
    spo2: "Range" = betterproto.message_field(4)
    hr: "Range" = betterproto.message_field(5)
    rr: "Range" = betterproto.message_field(6)
    pip: "Range" = betterproto.message_field(7)
    peep: "Range" = betterproto.message_field(8)
    ip_above_peep: "Range" = betterproto.message_field(9)
    insp_time: "Range" = betterproto.message_field(10)
    paw: "Range" = betterproto.message_field(11)
    mve: "Range" = betterproto.message_field(12)
    tv: "Range" = betterproto.message_field(13)
    etco2: "Range" = betterproto.message_field(14)
    apnea: "Range" = betterproto.message_field(15)


@dataclass
class LogEvent(betterproto.Message):
    id: int = betterproto.uint32_field(1)
    time: int = betterproto.uint64_field(2)
    code: "LogEventCode" = betterproto.enum_field(3)
    type: "LogEventType" = betterproto.enum_field(4)
    alarm_limits: "Range" = betterproto.message_field(5)
    old_float: float = betterproto.float_field(6)
    new_float: float = betterproto.float_field(7)
    old_uint32: int = betterproto.uint32_field(8)
    new_uint32: int = betterproto.uint32_field(9)
    old_bool: bool = betterproto.bool_field(10)
    new_bool: bool = betterproto.bool_field(11)
    old_range: "Range" = betterproto.message_field(12)
    new_range: "Range" = betterproto.message_field(13)
    old_mode: "VentilationMode" = betterproto.enum_field(14)
    new_mode: "VentilationMode" = betterproto.enum_field(15)


@dataclass
class ExpectedLogEvent(betterproto.Message):
    id: int = betterproto.uint32_field(1)
    # session_id is checked by the NextLogEvents sender if the sender's log is
    # ephemeral; the sender will ignore any ExpectedLogEvent whose session_id
    # doesn't match the sender's session_id, which is announced in NextLogEvents.
    session_id: int = betterproto.uint32_field(2)


@dataclass
class NextLogEvents(betterproto.Message):
    """
    Note: NextLogEvents has a custom equality operator in the firmware which
    must be updated if you add/remove/modify the fields of the protobuf
    definition!
    """

    next_expected: int = betterproto.uint32_field(1)
    total: int = betterproto.uint32_field(2)
    remaining: int = betterproto.uint32_field(3)
    session_id: int = betterproto.uint32_field(4)
    elements: List["LogEvent"] = betterproto.message_field(5)


@dataclass
class ActiveLogEvents(betterproto.Message):
    """
    Note: NextLogEvents has a custom equality operator in the firmware which
    must be updated if you add/remove/modify the fields of the protobuf
    definition!
    """

    id: List[int] = betterproto.uint32_field(1)


@dataclass
class AlarmMute(betterproto.Message):
    active: bool = betterproto.bool_field(1)
    # seq_num is a logical clock and advances in the firmware after each local
    # action in the firmware (such as a button-press) or the servicing of each
    # external request.
    seq_num: int = betterproto.uint32_field(2)
    source: "AlarmMuteSource" = betterproto.enum_field(3)
    remaining: int = betterproto.uint64_field(4)


@dataclass
class AlarmMuteRequest(betterproto.Message):
    active: bool = betterproto.bool_field(1)
    # seq_num is a logical clock which also acts as an idempotency key for
    # requests: the firmware only services an AlarmMuteRequest if the request's
    # seq_num is one greater than the seq_num in the firmware's copy of
    # AlarmMute.
    seq_num: int = betterproto.uint32_field(2)
    source: "AlarmMuteSource" = betterproto.enum_field(3)


@dataclass
class MCUPowerStatus(betterproto.Message):
    power_left: float = betterproto.float_field(1)
    charging: bool = betterproto.bool_field(2)


@dataclass
class BackendConnections(betterproto.Message):
    has_mcu: bool = betterproto.bool_field(1)
    has_frontend: bool = betterproto.bool_field(2)


@dataclass
class ScreenStatus(betterproto.Message):
    lock: bool = betterproto.bool_field(1)


@dataclass
class Ping(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    id: int = betterproto.uint32_field(2)


@dataclass
class Announcement(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    announcement: bytes = betterproto.bytes_field(2)
