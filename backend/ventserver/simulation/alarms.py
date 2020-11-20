"""Servicing of Parameters and ParametersRequest states."""

import time
import typing
from typing import Dict, Mapping, Optional, Type, Union

import attr

import betterproto

from ventserver.protocols.application import lists
from ventserver.protocols.protobuf import mcu_pb


# Simulators


@attr.s
class Service:
    """Base class for a breathing circuit simulator."""

    current_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time

    # Log Events
    # TODO: we should be able to get the next log event ID from the list sender
    # TODO: both of these attributes should be shared across services
    next_log_event_id: int = attr.ib(default=0)
    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    def transform(
            self, parameters: mcu_pb.Parameters,
            alarm_limits: mcu_pb.AlarmLimits,
            sensor_measurements: mcu_pb.SensorMeasurements,
            active_log_events: mcu_pb.ActiveLogEvents,
            log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the simulation."""
        if not parameters.ventilating:
            return

        self.transform_alarms(
            alarm_limits.spo2_min, alarm_limits.spo2_max,
            sensor_measurements.spo2,
            mcu_pb.LogEventCode.spo2_too_low,
            mcu_pb.LogEventCode.spo2_too_high,
            log_events_sender
        )
        self.transform_alarms(
            alarm_limits.fio2_min, alarm_limits.fio2_max,
            sensor_measurements.fio2,
            mcu_pb.LogEventCode.fio2_too_low,
            mcu_pb.LogEventCode.fio2_too_high,
            log_events_sender
        )
        self._transform_active_log_event_ids(active_log_events)

    # Update methods

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        self.current_time = current_time * 1000 - self.initial_time

    def transform_alarms(
            self, lower_limit: int, upper_limit: int, value: Union[float, int],
            too_low_code: mcu_pb.LogEventCode,
            too_high_code: mcu_pb.LogEventCode,
            log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the alarms for a particular parameter."""
        if value < lower_limit:
            self._activate_alarm(too_low_code, value, log_events_sender)
        else:
            self._deactivate_alarm(too_low_code)

        if value > upper_limit:
            self._activate_alarm(too_high_code, value, log_events_sender)
        else:
            self._deactivate_alarm(too_high_code)

    # Alarms

    def _transform_active_log_event_ids(
            self, active_log_events: mcu_pb.ActiveLogEvents
    ) -> None:
        """Update the list of active log events."""
        active_log_events.id = list(self.active_alarm_ids.values())

    # Log Events

    def _activate_alarm(
            self, code: mcu_pb.LogEventCode, value: float,
            log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Create a new Log Event if it is not active."""
        if code in self.active_alarm_ids:
            return

        log_event = mcu_pb.LogEvent(
            id=self.next_log_event_id, time=int(self.current_time),
            code=code, new_value=value
        )
        log_events_sender.input(lists.UpdateEvent(new_element=log_event))
        self.active_alarm_ids[code] = self.next_log_event_id
        self.next_log_event_id += 1

    def _deactivate_alarm(self, code: mcu_pb.LogEventCode) -> None:
        """Dectivate the Log Event if it's active."""
        if code not in self.active_alarm_ids:
            return

        self.active_alarm_ids.pop(code)


@attr.s
class PCAC(Service):
    """Breathing circuit simulator in PC-AC mode."""


@attr.s
class HFNC(Service):
    """Breathing circuit simulator in HFNC mode."""


# Aggregation


@attr.s
class Services:
    """Aggregator and selector of services for different modes."""

    _active_service: Optional[Service] = attr.ib(default=None)
    _services = {
        mcu_pb.VentilationMode.pc_ac: PCAC(),
        mcu_pb.VentilationMode.hfnc: HFNC()
    }

    def transform(
            self, current_time: float, all_states: Mapping[
                Type[betterproto.Message], Optional[betterproto.Message]
            ], log_events_sender: lists.SendSynchronizer[mcu_pb.LogEvent]
    ) -> None:
        """Update the parameters for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, all_states[mcu_pb.Parameters]
        )
        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        alarm_limits = typing.cast(
            mcu_pb.AlarmLimits, all_states[mcu_pb.AlarmLimits]
        )
        sensor_measurements = typing.cast(
            mcu_pb.SensorMeasurements,
            all_states[mcu_pb.SensorMeasurements]
        )
        active_log_events = typing.cast(
            mcu_pb.ActiveLogEvents, all_states[mcu_pb.ActiveLogEvents]
        )
        self._active_service.update_clock(current_time)
        self._active_service.transform(
            parameters, alarm_limits, sensor_measurements,
            active_log_events, log_events_sender
        )
