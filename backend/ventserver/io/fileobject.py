""" Generic support for File I/O connections.
TODO: Change name of the module"""

import os
from typing import Optional, Text

import attr

@attr.s(auto_attribs=True)
class FileProps:
    """ Defines protobuf file details."""

    filename: str = "state.pb"
    filedir: str = os.path.join(os.getcwd(), "ventserver", "statestore")
    mode: Text = 'rb'
