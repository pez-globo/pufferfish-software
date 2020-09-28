"""Tests ventserver.io.trio.fileio"""
from typing import Tuple, List 

import pytest as pt  # type: ignore

from ventserver.io.trio import fileio


examples = [(),()]

@pt.mark.parametrize("data_list",[examples])
async def test_fileio_write_read_ow(
        data_list: List[Tuple[bytes, str]]) -> None:
    """"""
    filehandler = fileio.Handler()
    for data, filename in data_list:
        filehandler.set_props(filename=filename, mode='wb')
        await filehandler.open()
        await filehandler.send(data)
        await filehandler.close()
        filehandler.set_props(filename=filename, mode='rb')
        await filehandler.open()
        output = await filehandler.receive()
        await filehandler.close()
        assert output == data

@pt.mark.parametrize("data_list",[examples])
async def test_fileio_multi_read_write(
        data_list: List[Tuple[bytes, str]]) -> None:
    """"""
    filehandler = fileio.Handler()
    for data, filename in data_list:
        filehandler.set_props(filename=filename, mode='wb')
        await filehandler.open()
        await filehandler.send(data)
        await filehandler.close()
        filehandler.set_props(filename=filename, mode='rb')
        await filehandler.open()
        output = await filehandler.receive()
        await filehandler.close()
        assert output == data

@pt.mark.parametrize("filename,data", examples)
async def test_fileio_read_repeated(
        filename: str, data: bytes) -> None:
    """"""
    filehandler = fileio.Handler()
    filehandler.set_props(filename=filename, mode='wb')
    await filehandler.open()
    await filehandler.send(data)
    await filehandler.close()

    filehandler.set_props(filename=filename, mode='rb')
    for _ in range(3): 
        await filehandler.open()
        output = await filehandler.receive()
        assert output == data
        await filehandler.close()

@pt.mark.parametrize("filename,data",examples)
async def test_fileio_read_new(
        filename: str, data: bytes) -> None:
    """"""
    filehandler = fileio.Handler()
    filehandler.set_props(filename=filename, mode='rb')
    with pt.raises(OSError):
        await filehandler.open()

    filehandler.set_props(filename=filename, mode='wb')
    await filehandler.open()
    await filehandler.send(data)
