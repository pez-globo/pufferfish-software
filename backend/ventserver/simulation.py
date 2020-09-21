"""Trio I/O with sans-I/O protocol, running application."""

import random
import time
import typing
from typing import Mapping, Optional, Type

import attr

import trio

from ventserver.integration import _trio
from ventserver.io.trio import channels
from ventserver.io.trio import websocket
from ventserver.protocols import application
from ventserver.protocols import server
from ventserver.protocols.protobuf import mcu_pb


@attr.s
class BreathingCircuitSimulator:
    """Base class for a breathing circuit simulator."""

    SENSOR_UPDATE_INTERVAL = 2

    all_states: Mapping[
        Type[application.PBMessage], Optional[application.PBMessage]
    ] = attr.ib()
    current_time: float = attr.ib(default=0)  # ms after initial_time
    previous_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time

    # FiO2
    fio2_responsiveness: float = attr.ib(default=0.01)  # ms
    fio2_noise: float = attr.ib(default=0.005)  # % FiO2

    # State segments

    @property
    def _sensor_measurements(self) -> mcu_pb.SensorMeasurements:
        """Access the SensorMeasurements state segment."""
        return typing.cast(
            mcu_pb.SensorMeasurements,
            self.all_states[mcu_pb.SensorMeasurements]
        )

    @property
    def _cycle_measurements(self) -> mcu_pb.CycleMeasurements:
        """Access the CycleMeasurements state segment."""
        return typing.cast(
            mcu_pb.CycleMeasurements, self.all_states[mcu_pb.CycleMeasurements]
        )

    @property
    def _parameters_request(self) -> mcu_pb.ParametersRequest:
        """Access the ParametersRequest state segment."""
        return typing.cast(
            mcu_pb.ParametersRequest, self.all_states[mcu_pb.ParametersRequest]
        )

    @property
    def _parameters(self) -> mcu_pb.Parameters:
        """Access the Parameters state segment."""
        return typing.cast(
            mcu_pb.Parameters, self.all_states[mcu_pb.Parameters]
        )

    # Timing

    @property
    def _time_step(self) -> float:
        """Compute the time step."""
        return self.current_time - self.previous_time

    # Update methods

    def update_parameters(self) -> None:
        """Update the parameters."""

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        self.previous_time = self.current_time
        self.current_time = current_time * 1000 - self.initial_time

    def update_sensors(self) -> None:
        """Update the simulated sensors."""

    def update_actuators(self) -> None:
        """Update the simulated actuators."""

    # FiO2 simulation

    def _update_fio2(self) -> None:
        """Update FiO2 measurements."""
        self._sensor_measurements.fio2 += (
            (
                self._parameters.fio2 - self._sensor_measurements.fio2
            )
            * self.fio2_responsiveness / self.SENSOR_UPDATE_INTERVAL
        )
        self._sensor_measurements.fio2 += self.fio2_noise * random.random()


@attr.s
class PCACSimulator(BreathingCircuitSimulator):
    """Breathing circuit simulator in PC-AC mode."""

    cycle_start_time: float = attr.ib(default=0)  # ms
    cycle_period: float = attr.ib(default=2000.0)  # ms
    insp_period: float = attr.ib(default=1000.0)  # ms
    insp_responsiveness: float = attr.ib(default=0.05)  # ms
    exp_responsiveness: float = attr.ib(default=0.05)  # ms
    insp_init_flow_rate: float = attr.ib(default=120)  # L / min
    exp_init_flow_rate: float = attr.ib(default=-120)  # L / min
    insp_flow_responsiveness: float = attr.ib(default=0.02)  # ms
    exp_flow_responsiveness: float = attr.ib(default=0.02)  # ms

    def update_parameters(self) -> None:
        """Implement BreathingCircuitSimulator.update_parameters."""
        if self._parameters_request.rr > 0:
            self._parameters.rr = self._parameters_request.rr
        if self._parameters_request.ie > 0:
            self._parameters.ie = self._parameters_request.ie
        if self._parameters_request.pip > 0:
            self._parameters.pip = self._parameters_request.pip
        self._parameters.peep = self._parameters_request.peep
        self._parameters.fio2 = self._parameters_request.fio2

    def update_sensors(self) -> None:
        """Implement BreathingCircuitSimulator.update_sensors."""
        if self._time_step == 0:
            return

        self._sensor_measurements.time = int(self.current_time)
        cycle_period = 60000 / self._parameters.rr
        if self.current_time - self.cycle_start_time > cycle_period:
            self._init_cycle(cycle_period)
            self._update_cycle_measurements()

        if self.current_time - self.cycle_start_time < self.insp_period:
            self._update_airway_inspiratory()
        else:
            self._update_airway_expiratory()
        self._update_fio2()

    def _init_cycle(self, cycle_period: float) -> None:
        """Initialize at the start of the cycle."""
        self.cycle_start_time = self.current_time
        self._sensor_measurements.flow = self.insp_init_flow_rate
        self._sensor_measurements.volume = 0
        self.insp_period = cycle_period / (1 + 1 / self._parameters.ie)

    def _update_cycle_measurements(self) -> None:
        """Update cycle measurements."""
        self._cycle_measurements.rr = \
            self._parameters.rr + random.random() - 0.5
        self._cycle_measurements.peep = \
            self._parameters.peep + random.random() - 0.5
        self._cycle_measurements.pip = \
            self._parameters.pip + random.random() - 0.5

    def _update_airway_inspiratory(self) -> None:
        """Update sensor measurements in inspiratory phase."""
        self._sensor_measurements.paw += (
            self._parameters.pip - self._sensor_measurements.paw
        ) * self.insp_responsiveness / self._time_step
        self._sensor_measurements.flow *= (
            1 - self.insp_flow_responsiveness / self._time_step
        )
        self._sensor_measurements.volume += (
            self._sensor_measurements.flow / 60 * self._time_step
        )

    def _update_airway_expiratory(self) -> None:
        """Update sensor measurements in expiratory phase."""
        self._sensor_measurements.paw += (
            self._parameters.peep - self._sensor_measurements.paw
        ) * self.exp_responsiveness / self._time_step
        if self._sensor_measurements.flow >= 0:
            self._sensor_measurements.flow = self.exp_init_flow_rate
        else:
            self._sensor_measurements.flow *= (
                1 - self.exp_flow_responsiveness / self._time_step
            )
        self._sensor_measurements.volume += (
            self._sensor_measurements.flow / 60 * self._time_step
        )


async def simulate_states(
        all_states: Mapping[
            Type[application.PBMessage], Optional[application.PBMessage]
        ]
) -> None:
    """Simulate evolution of all states."""
    pc_ac = PCACSimulator(all_states=all_states)
    while True:
        # Parameters
        pc_ac.update_parameters()
        # Timing
        await trio.sleep(pc_ac.SENSOR_UPDATE_INTERVAL / 1000)
        pc_ac.update_clock(time.time())
        pc_ac.update_sensors()
        pc_ac.update_actuators()


async def main() -> None:
    """Set up wiring between subsystems and process until completion."""
    # Sans-I/O Protocols
    protocol = server.Protocol()

    # I/O Endpoints
    websocket_endpoint = websocket.Driver()

    # Server Receive Outputs
    channel: channels.TrioChannel[
        server.ReceiveOutputEvent
    ] = channels.TrioChannel()

    # Initialize State
    all_states = protocol.receive.backend.all_states
    all_states[mcu_pb.Parameters] = mcu_pb.Parameters()
    all_states[mcu_pb.CycleMeasurements] = mcu_pb.CycleMeasurements()
    all_states[mcu_pb.SensorMeasurements] = mcu_pb.SensorMeasurements()
    all_states[mcu_pb.ParametersRequest] = mcu_pb.ParametersRequest(
        mode=mcu_pb.VentilationMode(
            support=mcu_pb.SpontaneousSupport.ac,
            cycling=mcu_pb.VentilationCycling.pc
        ),
        pip=30, peep=10, rr=30, ie=1, fio2=60
    )

    try:
        async with channel.push_endpoint:
            async with trio.open_nursery() as nursery:
                nursery.start_soon(
                    _trio.process_all, None, protocol,
                    websocket_endpoint, channel, channel.push_endpoint
                )
                nursery.start_soon(simulate_states, all_states)

                while True:
                    receive_output = await channel.output()
                    await _trio.process_protocol_send(
                        receive_output.server_send, protocol,
                        None, websocket_endpoint
                    )
                nursery.cancel_scope.cancel()
    except trio.EndOfChannel:
        print('Finished, quitting!')


if __name__ == '__main__':
    try:
        trio.run(main)
    except KeyboardInterrupt:
        print('Quitting!')
