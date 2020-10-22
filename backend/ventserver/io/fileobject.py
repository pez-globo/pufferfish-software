""" Generic support for File I/O connections."""

from typing import Text

import attr

@attr.s(auto_attribs=True)
class FileProps:
    """ Defines protobuf file details."""

    filename: str = "state.pb"
    filedir: str = ""
    mode: Text = 'rb'
