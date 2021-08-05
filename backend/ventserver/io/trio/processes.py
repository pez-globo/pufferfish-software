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
