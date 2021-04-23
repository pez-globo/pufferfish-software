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

    # Patient
    spo2_too_low = 0
    spo2_too_high = 1
    hr_too_low = 2
    hr_too_high = 3
    fio2_too_low = 4
    fio2_too_high = 5
    flow_too_low = 6
    flow_too_high = 7
    # System
    battery_low = 8
    screen_locked = 9
    # Control
    ventilation_operation_changed = 10
    ventilation_mode_changed = 11
    fio2_setting_changed = 12
    flow_setting_changed = 13
    # Alarm Limits
    spo2_alarm_limits_changed = 14
    hr_alarm_limits_changed = 15
    fio2_alarm_limits_changed = 16
    flow_alarm_limits_changed = 17


class LogEventType(betterproto.Enum):
    patient = 0
    system = 1
    control = 2
    alarm_limits = 3


@dataclass
class Range(betterproto.Message):
    lower: int = betterproto.uint32_field(1)
    upper: int = betterproto.uint32_field(2)


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


@dataclass
class NextLogEvents(betterproto.Message):
    next_expected: int = betterproto.uint32_field(1)
    total: int = betterproto.uint32_field(2)
    remaining: int = betterproto.uint32_field(3)
    elements: List["LogEvent"] = betterproto.message_field(4)


@dataclass
class ActiveLogEvents(betterproto.Message):
    id: List[int] = betterproto.uint32_field(1)


@dataclass
class BatteryPower(betterproto.Message):
    power_left: int = betterproto.uint32_field(1)
    charging_status: bool = betterproto.bool_field(2)


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
