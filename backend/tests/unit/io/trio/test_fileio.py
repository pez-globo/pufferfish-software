"""Tests ventserver.io.trio.fileio"""
import os
from typing import Tuple, List 

import pytest as pt  # type: ignore

from ventserver.io.trio import fileio

FILES_PATH = os.path.join(
    os.getcwd(), "tests", "unit", 
    "io", "trio", "fileio_tests"
    )

examples = [
    (
        b'U\xa3\x13\xae\x04\x08\xdd\xe1\xce\xce\xcd.\x10\x06\x1d\x00\x00' +
        b'\xc0@%\x00\x00\xf0A-\x00\x00HC5\x00\x00\xf0A=\x00\x00\x00@E\x00' +
        b'\x00pBM\x00\x00\x08B', 'Parameters'
    ), 
    (
        b'\xff\r\xca<\x05\x08\x90\xf5\xce\xce\xcd.\x10\x06\x1d\x00\x00' +
        b'\xe0@%\x00\x00\xa0A-\x00\x00\x96C5\x00\x00\xa0B=\x00\x00@@E' +
        b'\x00\x00HBM\x00\x00\xa0A', 'ParametersRequest'
    ),
    (
        b'\xacO\xb2\x8c\x03\x08\x89\x99\xcf\xce\xcd.\x15\x00\x00zC\x1d' +
        b'\x00\x00\xc8A%\x00\x00pA-\x00\x00 A5\x00\x00\xc0@=\x00\x00pA',
        'CycleMeasurements'
    ),
    (   
        b'%W\xb29\x02\x08\xe9\xc0\xe1\xce\xcd.\x10\n\x1d\x00\x00\xa0A%' +
        b'\x00\x00\xc8A-\x00\x00HB5\x00\x00HB=\x00\x00\xc2B',
        'SensorMeasurements'
    ),
    (
        b'/3Z\xf6\x01\x08\xe9\xc0\xe1\xce\xcd.\x10\x01', 'Alarms'
    ),
    (
        b'*Z\x1e|\x05\x08\x90\xf5\xce\xce\xcd.\x10\x06\x14\x00\x00\xe0@%' +
        b'\x00\x00\xa0A-\x00\x00\x96C5\x00\x00\xa0B=\xcd\xcc\x0c?E\x00' +
        b'\x00HBM\x00\x00\xa0A', 'ParametersRequest_bad_crc'
    ),
    (
        b'C\x82\xfc,\x04\x08\xdd\xe1\xce\xce\xcd.\x10\x06\x1d\x00\x00' +
        b'\xc0@%\xf0A-\x00\x00HC5\x00\x00\xf0A=\x9a\x99\x19?E\x00' +
        b'\x00pBM\x00\x00\x08B', 'Parameters_bad_crc'
    ),
    (
        b'\xac1\xb2\x8c\x03\x08\x89\x99\xcf\xce\xcd.\x15\x00\x00zC\x1d' +
        b'\x00\x00\xc8A%\x00\x00pA-\x00\x00 A5\x00\x00\xc0@=\x00\x00pA',
        'CycleMeasurements_bad_crc'
    ),
    (
        b'/3Z\xf6\x01\x08\xe9\xc0\xe1\xce\xcd.\x10\x02', 'Alarms_bad_crc'
    ),
    (
        b'\xacO\xb2\x8c\x03\x08\x89\x99\xcf\xce\xcd.\x16\x00\x00zC\x1d\x00' +
        b'\x00\xc8A%\x00\x00pA-\x00\x00 A5\x00\x00\xc0@=\x00\x00pA',
        'Parameters_bad_data'
    )
]

@pt.mark.trio
@pt.mark.parametrize("data_list", [examples[:2]])
async def test_fileio_write_read_ow(
        data_list: List[Tuple[bytes, str]]) -> None:
    """"""
    filehandler = fileio.Handler()
    filehandler.props.filedir = FILES_PATH
    filename = "fileio_write_read_ow"
    for data, _ in data_list:
        filehandler.set_props(filename=filename, mode='wb')
        await filehandler.open()
        async with filehandler:
            await filehandler.send(data)
        filehandler.set_props(filename=filename, mode='rb')
        await filehandler.open()
        async with filehandler:
            output = await filehandler.receive()
        assert output == data

@pt.mark.trio
@pt.mark.parametrize("data_list", [examples])
async def test_fileio_multi_read_write(
        data_list: List[Tuple[bytes, str]]) -> None:
    """"""
    filehandler = fileio.Handler()
    filehandler.props.filedir = FILES_PATH
    for data, filename in data_list:
        filehandler.set_props(filename=filename, mode='wb')
        await filehandler.open()
        async with filehandler:
            await filehandler.send(data)
        filehandler.set_props(filename=filename, mode='rb')
        await filehandler.open()
        async with filehandler:
            output = await filehandler.receive()
        assert output == data

@pt.mark.trio
@pt.mark.parametrize("data,_", [examples[0]])
async def test_fileio_read_repeated(
        data: str, _: bytes) -> None:
    """"""
    filehandler = fileio.Handler()
    filehandler.props.filedir = FILES_PATH
    filename = "fileio_read_repeated"
    filehandler.set_props(filename=filename, mode='wb')
    await filehandler.open()
    async with filehandler:
        await filehandler.send(data)

    filehandler.set_props(filename=filename, mode='rb')
    for _ in range(3): 
        await filehandler.open()
        async with filehandler:
            output = await filehandler.receive()
        assert output == data

@pt.mark.trio
@pt.mark.parametrize("data,_", [examples[0]])
async def test_fileio_read_new(
        data: str, _: bytes) -> None:
    """"""
    filehandler = fileio.Handler()
    filehandler.props.filedir = FILES_PATH
    filename = "fileio_read_new"
    if os.path.exists(os.path.join(FILES_PATH, filename + '.pb')):
        os.remove(os.path.join(FILES_PATH, filename + '.pb'))
    filehandler.set_props(filename=filename, mode='rb')
    with pt.raises(OSError):
        await filehandler.open()

    filehandler.set_props(filename=filename, mode='wb')
    await filehandler.open()
    async with filehandler:
        await filehandler.send(data)
