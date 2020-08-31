""" Generic support for File I/O connections.
TODO: Change name of the module"""

import os
from typing import Optional

import attr

@attr.s(auto_attribs=True)
class FileProps:
    """ Defines protobuf file details."""

    filename: Optional[str] = "state.pb"
    filedir: str = os.path.join(os.getcwd(), "ventserver", "statestore")
    mode: str = 'rb'
