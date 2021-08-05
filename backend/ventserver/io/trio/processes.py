"""Defines function for managing other processes on the system."""

import subprocess
import logging

import trio


logger = logging.getLogger(__name__)


async def kill_process(process_name: str) -> None:
    """Spawn subprocess to kill the specified process."""
    try:
        logger.warning('Killing process: %s', process_name)
        await trio.run_process(['killall', process_name], capture_stderr=True)
        logger.warning('Killed process: %s', process_name)
    except subprocess.CalledProcessError as err:
        logger.error(
            'Failed to kill process: %s', err.stderr.decode('utf-8').rstrip()
        )


async def make_dialog(
    message: str, level: str = 'error', width: int = 300, height: int = 150
) -> None:
    """Spawn subprocess to make a GUI dialog.

    level may be error, warning, info, or question.
    """
    try:
        await trio.run_process([
            'zenity', '--{}'.format(level), '--text', message,
            '--width={}'.format(width), '--height={}'.format(height)
        ], capture_stderr=True)
    except subprocess.CalledProcessError as err:
        logger.error(
            'Failed to make %s-level dialog "%s": %s',
            level, message, err.stderr.decode('utf-8').rstrip()
        )


async def set_system_time(
    year: int, month: int, day: int, hour: int, minute: int, second: int
) -> None:
    """Spawn subprocess to change the system (wall clock) time."""
    date_time_string = '{:0>4d}-{:0>2d}-{:0>2d} {:0>2d}:{:0>2d}:{:0>2d}'.format(
        year, month, day, hour, minute, second
    )
    try:
        logger.info('Changing system time to: %s', date_time_string)
        await trio.run_process([
            'timedatectl', 'set-time', date_time_string
        ], capture_stderr=True)
        logger.info('Changed system time to: %s', date_time_string)
    except subprocess.CalledProcessError as err:
        logger.error(
            'Failed change system time to %s: %s',
            date_time_string, err.stderr.decode('utf-8').rstrip()
        )


async def synchronize_rtc() -> None:
    """Spawn subprocess to update the hardware RTC to the system time."""
    try:
        await trio.run_process(['hwclock', '-w'], capture_stderr=True)
    except subprocess.CalledProcessError as err:
        logger.error(
            'Failed to update hardware RTC with system time: %s',
            err.stderr.decode('utf-8').rstrip()
        )
