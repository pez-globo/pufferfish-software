"""Defines function for managing other processes on the system."""

import logging
import subprocess
import time

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


async def set_system_time(wall_time: float) -> bool:
    """Spawn subprocess to change the system (wall clock) time."""
    try:
        logger.info(
            'Changing system time from %s to: %s', time.time(), int(wall_time)
        )
        await trio.run_process([
            'date', '+%s', '-s', '@{}'.format(int(wall_time))
        ], capture_stdout=True, capture_stderr=True)
        logger.info('Changed system time to: %s', int(wall_time))
        return True
    except subprocess.CalledProcessError as err:
        logger.error(
            'Failed change system time to %s: %s',
            int(wall_time), err.stderr.decode('utf-8').rstrip()
        )
        return False


async def synchronize_rtc() -> None:
    """Spawn subprocess to update the hardware RTC to the system time."""
    try:
        await trio.run_process(['hwclock', '-w'], capture_stderr=True)
    except subprocess.CalledProcessError as err:
        logger.error(
            'Failed to update hardware RTC with system time: %s',
            err.stderr.decode('utf-8').rstrip()
        )


async def set_system_time_and_rtc(wall_time: float) -> None:
    """Set the system time and RTC from a unix timestamp in units of sec."""
    time_adjusted = await set_system_time(wall_time)
    if not time_adjusted:
        return

    await synchronize_rtc()
