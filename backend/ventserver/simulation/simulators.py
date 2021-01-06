"""Servicing of Parameters and ParametersRequest states."""

import random
import time
import typing
from typing import Dict, Mapping, Optional, Type

import attr

import betterproto

from ventserver.protocols.protobuf import mcu_pb


SENSOR_UPDATE_INTERVAL = 2


# Simulators


@attr.s
class Service:
    """Base class for a breathing circuit simulator."""

    current_time: float = attr.ib(default=0)  # ms after initial_time
    previous_time: float = attr.ib(default=0)  # ms after initial_time
    initial_time: float = attr.ib(default=time.time() * 1000)  # ms, Unix time

    # FiO2
    fio2_responsiveness: float = attr.ib(default=0.01)  # ms
    fio2_noise: float = attr.ib(default=0.005)  # % FiO2

    # Log Events
    next_log_event_id: int = attr.ib(default=0)
    active_alarm_ids: Dict[mcu_pb.LogEventCode, int] = attr.ib(default={})

    def transform(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements,
            cycle_measurements: mcu_pb.CycleMeasurements
    ) -> None:
        """Update the simulation."""

    # Timing

    @property
    def _time_step(self) -> float:
        """Compute the time step."""
        return self.current_time - self.previous_time

    # Update methods

    def update_clock(self, current_time: float) -> None:
        """Update the internal state for timing."""
        self.previous_time = self.current_time
        self.current_time = current_time * 1000 - self.initial_time

    def transform_sensors(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements,
            cycle_measurements: mcu_pb.CycleMeasurements
    ) -> None:
        """Update the simulated sensors."""

    # FiO2 simulation

    def _transform_fio2(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements
    ) -> None:
        """Update FiO2 measurements."""
        sensor_measurements.fio2 += (
            (
                parameters.fio2 - sensor_measurements.fio2
            )
            * self.fio2_responsiveness / SENSOR_UPDATE_INTERVAL
        )
        sensor_measurements.fio2 += self.fio2_noise * random.random()


@attr.s
class PCAC(Service):
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
    rr_noise: float = 0.5  # breaths/min
    peep_noise: float = 1.0  # cm H2O
    pip_noise: float = 1.0  # cm H2O

    def transform(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements,
            cycle_measurements: mcu_pb.CycleMeasurements
    ) -> None:
        """Implement Service.transform."""
        if self._time_step == 0:
            return

        if not parameters.ventilating:
            return

        if parameters.mode != mcu_pb.VentilationMode.pc_ac:
            return

        sensor_measurements.time = int(self.current_time)
        cycle_period = 60000 / parameters.rr
        if self.current_time - self.cycle_start_time > cycle_period:
            self._init_cycle(cycle_period, parameters, sensor_measurements)
            self._transform_cycle_measurements(parameters, cycle_measurements)

        if self.current_time - self.cycle_start_time < self.insp_period:
            self._transform_airway_inspiratory(parameters, sensor_measurements)
        else:
            self._transform_airway_expiratory(parameters, sensor_measurements)
        self._transform_fio2(parameters, sensor_measurements)

    def _init_cycle(
            self, cycle_period: float, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements
    ) -> None:
        """Initialize at the start of the cycle."""
        self.cycle_start_time = self.current_time
        sensor_measurements.flow = self.insp_init_flow_rate
        sensor_measurements.volume = 0
        self.insp_period = cycle_period / (1 + 1 / parameters.ie)
        sensor_measurements.cycle += 1

    def _transform_cycle_measurements(
            self, parameters: mcu_pb.Parameters,
            cycle_measurements: mcu_pb.CycleMeasurements
    ) -> None:
        """Update cycle measurements."""
        cycle_measurements.rr = \
            parameters.rr + self.rr_noise * (random.random() - 0.5)
        cycle_measurements.peep = \
            parameters.peep + self.peep_noise * (random.random() - 0.5)
        cycle_measurements.pip = \
            parameters.pip + self.pip_noise * (random.random() - 0.5)

    def _transform_airway_inspiratory(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements
    ) -> None:
        """Update sensor measurements in inspiratory phase."""
        sensor_measurements.paw += (
            parameters.pip - sensor_measurements.paw
        ) * self.insp_responsiveness / self._time_step
        sensor_measurements.flow *= (
            1 - self.insp_flow_responsiveness / self._time_step
        )
        sensor_measurements.volume += (
            sensor_measurements.flow / 60 * self._time_step
        )

    def _transform_airway_expiratory(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements
    ) -> None:
        """Update sensor measurements in expiratory phase."""
        sensor_measurements.paw += (
            parameters.peep - sensor_measurements.paw
        ) * self.exp_responsiveness / self._time_step
        if sensor_measurements.flow >= 0:
            sensor_measurements.flow = self.exp_init_flow_rate
        else:
            sensor_measurements.flow *= (
                1 - self.exp_flow_responsiveness / self._time_step
            )
        sensor_measurements.volume += (
            sensor_measurements.flow / 60 * self._time_step
        )


@attr.s
class HFNC(Service):
    """Breathing circuit simulator in HFNC mode."""

    cycle_start_time: float = attr.ib(default=0)  # ms
    flow_responsiveness: float = attr.ib(default=0.01)  # ms
    flow_noise: float = 0.05  # L/min
    spo2_fio2_scale: float = attr.ib(default=2.5)  # % SpO2 / % FiO2
    spo2_responsiveness: float = attr.ib(default=0.0005)  # ms
    spo2_noise: float = 0.1  # % SpO2
    rr_noise: float = 5  # b/min

    def transform(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements,
            cycle_measurements: mcu_pb.CycleMeasurements,
    ) -> None:
        """Implement Simulator.transform."""
        if self._time_step == 0:
            return

        if not parameters.ventilating:
            return

        if parameters.mode != mcu_pb.VentilationMode.hfnc:
            return

        sensor_measurements.time = int(self.current_time)

        self._transform_flow(parameters, sensor_measurements)
        self._transform_fio2(parameters, sensor_measurements)
        self._transform_spo2(sensor_measurements)

    def _transform_flow(
            self, parameters: mcu_pb.Parameters,
            sensor_measurements: mcu_pb.SensorMeasurements
    ) -> None:
        """Update flow measurements."""
        sensor_measurements.flow += (
            (
                parameters.flow - sensor_measurements.flow
            )
            * self.flow_responsiveness / SENSOR_UPDATE_INTERVAL
        )
        sensor_measurements.flow += \
            self.flow_noise * (random.random() - 0.5)

    def _transform_spo2(
            self, sensor_measurements: mcu_pb.SensorMeasurements
    ) -> None:
        """Update SpO2 measurements."""
        sensor_measurements.spo2 += (
            (
                self.spo2_fio2_scale * sensor_measurements.fio2
                - sensor_measurements.spo2
            )
            * self.spo2_responsiveness / SENSOR_UPDATE_INTERVAL
        )
        sensor_measurements.spo2 += \
            self.spo2_noise * (random.random() - 0.5)
        sensor_measurements.spo2 = min(
            sensor_measurements.spo2, 100
        )
        sensor_measurements.spo2 = max(
            sensor_measurements.spo2, 21
        )

    def _init_cycle(self) -> None:
        """Initialize at the start of the cycle."""
        self.cycle_start_time = self.current_time


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
            ],
    ) -> None:
        """Update the parameters for the requested mode."""
        parameters = typing.cast(
            mcu_pb.Parameters, all_states[mcu_pb.Parameters]
        )

        self._active_service = self._services.get(parameters.mode, None)

        if self._active_service is None:
            return

        sensor_measurements = typing.cast(
            mcu_pb.SensorMeasurements,
            all_states[mcu_pb.SensorMeasurements]
        )
        cycle_measurements = typing.cast(
            mcu_pb.CycleMeasurements, all_states[mcu_pb.CycleMeasurements]
        )
        self._active_service.update_clock(current_time)
        self._active_service.transform(
            parameters, sensor_measurements, cycle_measurements
        )
