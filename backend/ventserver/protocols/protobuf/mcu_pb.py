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
    """Log Events"""

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
    mcu_connection_down = 130
    backend_connection_down = 131
    frontend_connection_down = 132
    mcu_connection_up = 133
    backend_connection_up = 134
    frontend_connection_up = 135
    battery_low = 136


class LogEventType(betterproto.Enum):
    patient = 0
    control = 1
    alarm_limits = 2
    system = 3


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
class SensorMeasurements(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    cycle: int = betterproto.uint32_field(2)
    fio2: float = betterproto.float_field(3)
    spo2: float = betterproto.float_field(4)
    hr: float = betterproto.float_field(5)
    paw: float = betterproto.float_field(6)
    flow: float = betterproto.float_field(7)
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
class Ping(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    id: int = betterproto.uint32_field(2)


@dataclass
class Announcement(betterproto.Message):
    time: int = betterproto.uint64_field(1)
    announcement: bytes = betterproto.bytes_field(2)


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
    session_id: int = betterproto.uint32_field(2)


@dataclass
class NextLogEvents(betterproto.Message):
    next_expected: int = betterproto.uint32_field(1)
    total: int = betterproto.uint32_field(2)
    remaining: int = betterproto.uint32_field(3)
    session_id: int = betterproto.uint32_field(4)
    elements: List["LogEvent"] = betterproto.message_field(5)


@dataclass
class ActiveLogEvents(betterproto.Message):
    id: List[int] = betterproto.uint32_field(1)


@dataclass
class Battery(betterproto.Message):
    power_left: int = betterproto.uint32_field(1)
    charging: bool = betterproto.bool_field(2)


@dataclass
class ScreenStatus(betterproto.Message):
    lock: bool = betterproto.bool_field(1)


@dataclass
class AlarmMute(betterproto.Message):
    active: bool = betterproto.bool_field(1)
    remaining: float = betterproto.float_field(2)


@dataclass
class AlarmMuteRequest(betterproto.Message):
    active: bool = betterproto.bool_field(1)
    remaining: float = betterproto.float_field(2)
