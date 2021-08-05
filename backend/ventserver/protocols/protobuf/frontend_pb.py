# Generated by the protocol buffer compiler.  DO NOT EDIT!
# sources: frontend_pb.proto
# plugin: python-betterproto
from dataclasses import dataclass

import betterproto


class Unit(betterproto.Enum):
    """TODO: should metric be 0? (i.e. the default)"""

    imperial = 0
    metric = 1


class ThemeVariant(betterproto.Enum):
    dark = 0
    light = 1


@dataclass
class RotaryEncoder(betterproto.Message):
    step: int = betterproto.int32_field(1)
    last_step_change: float = betterproto.float_field(2)
    button_pressed: bool = betterproto.bool_field(3)
    last_button_down: float = betterproto.float_field(4)
    last_button_up: float = betterproto.float_field(5)


@dataclass
class FrontendDisplaySetting(betterproto.Message):
    """
    TODO: we also need a request version of this message,
    FrontendDisplaySettingsRequest TODO: rename this to FrontendDisplaySettings
    (with the "s" at the end)
    """

    theme: "ThemeVariant" = betterproto.enum_field(1)
    unit: "Unit" = betterproto.enum_field(2)


@dataclass
class SystemSettings(betterproto.Message):
    date: float = betterproto.double_field(1)
    # TODO: move display_brightness into FrontendDisplaySetting
    display_brightness: int = betterproto.uint32_field(2)
    seq_num: int = betterproto.uint32_field(3)


@dataclass
class SystemSettingsRequest(betterproto.Message):
    date: float = betterproto.double_field(1)
    display_brightness: int = betterproto.uint32_field(2)
    seq_num: int = betterproto.uint32_field(3)
