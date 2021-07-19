"""alarm muting request and response"""

import logging
import random
import typing
from typing import Mapping, Optional

import attr

import betterproto

from ventserver.protocols.backend import alarms, log, states
from ventserver.protocols.protobuf import mcu_pb


@attr.s
class Service:
    """Implement Alarm Mute Service"""
    MUTE_MAX_DURATION = 120000  # ms

    _logger = logging.getLogger('.'.join((__name__, 'Service')))

    active: bool = attr.ib(default=False)
    mute_start_time: Optional[float] = attr.ib(default=None)
    seq_num: int = attr.ib(default=random.getrandbits(32))
    source: mcu_pb.AlarmMuteSource = \
        attr.ib(default=mcu_pb.AlarmMuteSource.initialization)

    # TODO: also allow canceling alarm mute upon loss of connection, maybe with
    # an AlarmMute client (rather than a service)
    def transform(
            self, current_time: float,
            store: Mapping[states.StateSegment, Optional[betterproto.Message]],
            events_log: alarms.Manager
    ) -> None:
        """Update the parameters for alarm mute service"""
        alarm_mute_request = typing.cast(
            mcu_pb.AlarmMuteRequest,
            store[states.StateSegment.ALARM_MUTE_REQUEST]
        )
        alarm_mute = typing.cast(
            mcu_pb.AlarmMute, store[states.StateSegment.ALARM_MUTE]
        )
        backend_connections = typing.cast(
            mcu_pb.BackendConnections,
            store[states.StateSegment.BACKEND_CONNECTIONS]
        )
        self.transform_mute(
            current_time, alarm_mute_request, alarm_mute, events_log
        )
        if (
                backend_connections is not None and
                not backend_connections.has_frontend
        ):
            self.transform_mute_internal(
                current_time, False,
                mcu_pb.AlarmMuteSource.backend_frontend_loss,
                alarm_mute, events_log
            )

    def transform_mute(
            self, current_time: float,
            request: mcu_pb.AlarmMuteRequest, response: mcu_pb.AlarmMute,
            events_log: alarms.Manager
    ) -> None:
        """Implement alarm muting."""
        if request.seq_num == self.seq_num + 1:
            self._update_internal_state(
                request.active, current_time, request.source, events_log
            )
        self._update_response(current_time, response, events_log)

    def transform_mute_internal(
            self, current_time: float, mute: bool,
            source: mcu_pb.AlarmMuteSource,
            response: mcu_pb.AlarmMute, events_log: alarms.Manager
    ) -> None:
        """Implement alarm muting."""
        self._update_internal_state(mute, current_time, source, events_log)
        self._update_response(current_time, response, events_log)

    def _update_internal_state(
            self, active: bool, current_time: float,
            source: mcu_pb.AlarmMuteSource, events_log: alarms.Manager
    ) -> None:
        """Update internal state."""
        if self.active == active:
            return

        self.seq_num += 1
        self.active = active
        self.source = source
        if active:
            self.mute_start_time = current_time * 1000
            if source == mcu_pb.AlarmMuteSource.user_software:
                log_event_code = mcu_pb.LogEventCode.alarms_muted_user_software
            else:
                self._logger.error(
                    'Unexpected alarm mute activation source %s', source
                )
                log_event_code = mcu_pb.LogEventCode.alarms_muted_unknown
        else:
            self.mute_start_time = None
            if source == mcu_pb.AlarmMuteSource.initialization:
                log_event_code = \
                    mcu_pb.LogEventCode.alarms_unmuted_initialization
            elif source == mcu_pb.AlarmMuteSource.user_software:
                log_event_code = \
                    mcu_pb.LogEventCode.alarms_unmuted_user_software
            elif source == mcu_pb.AlarmMuteSource.timeout:
                log_event_code = \
                    mcu_pb.LogEventCode.alarms_unmuted_timeout
            elif source == mcu_pb.AlarmMuteSource.backend_frontend_loss:
                log_event_code = \
                    mcu_pb.LogEventCode.alarms_unmuted_backend_frontend_loss
            elif source == mcu_pb.AlarmMuteSource.frontend_backend_loss:
                log_event_code = \
                    mcu_pb.LogEventCode.alarms_unmuted_frontend_backend_loss
            else:
                self._logger.error(
                    'Unexpected alarm mute cancellation source %s', source
                )
                log_event_code = mcu_pb.LogEventCode.alarms_unmuted_unknown
        events_log.input(log.LocalLogInputEvent(new_event=mcu_pb.LogEvent(
            code=log_event_code, type=mcu_pb.LogEventType.system
        )))

    def _update_response(
            self, current_time: float, response: mcu_pb.AlarmMute,
            events_log: alarms.Manager
    ) -> None:
        """Update response based on internal state."""
        response.active = self.active
        response.seq_num = self.seq_num
        response.source = self.source
        self._update_remaining(current_time, response)
        if response.remaining > 0:
            return

        self._update_internal_state(
            False, current_time, mcu_pb.AlarmMuteSource.timeout, events_log
        )
        response.active = self.active
        response.seq_num = self.seq_num
        response.source = self.source
        self._update_remaining(current_time, response)

    def _update_remaining(
            self, current_time: float, response: mcu_pb.AlarmMute
    ) -> None:
        """Update remaining field of response based on internal state."""
        if not self.active:
            response.remaining = int(self.MUTE_MAX_DURATION / 1000)
            return

        if self.mute_start_time is None:
            self.mute_start_time = current_time * 1000
        mute_duration = current_time * 1000 - self.mute_start_time
        remaining = int((self.MUTE_MAX_DURATION - mute_duration) / 1000)
        response.remaining = max(
            0, min(remaining, int(self.MUTE_MAX_DURATION / 1000))
        )
