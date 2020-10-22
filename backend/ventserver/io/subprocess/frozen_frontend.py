"""Defines function for spawning subprocess to kill frozen frontend."""

import subprocess
import logging
import time

import trio


logger = logging.getLogger(__name__)

# Kill frontend

async def kill_frozen_frontend(
    websocket_connected: bool, websocket_connection_time: float
) -> None:
    """Spawns subprocess to kill the frontend"""
    connection_duration = int(time.time() - websocket_connection_time)
    if websocket_connected and connection_duration > 2:
        try:
            kill_proc = await trio.run_process(
                ["killall", "/usr/lib/chromium-browser/chromium-browser-v7"]
            )
            logger.info(
                "No message received from frontend for more "
                "than a 1s; killed frontend process: %s", kill_proc
            )
        except subprocess.CalledProcessError as exc:
            logger.warning("Failed to kill the frontend: %s", exc)
