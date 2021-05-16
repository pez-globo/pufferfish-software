"""Defines function for spawning subprocess to kill frozen frontend."""

import subprocess
import logging

import trio


logger = logging.getLogger(__name__)

# Kill frontend

# TODO: move this into io.trio, and generalize subprocess running
async def kill_frozen_frontend() -> None:
    """Spawns subprocess to kill the frontend"""
    try:
        kill_proc = await trio.run_process(
            ["killall", "/usr/lib/chromium-browser/chromium-browser-v7"]
        )
        logger.warning(
            "No message received from frontend for more "
            "than a 1s; killing frontend process: %s", kill_proc
        )
    except subprocess.CalledProcessError as exc:
        logger.error("Failed to kill the frontend: %s", exc)
