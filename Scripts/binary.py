from enum import Enum, Flag, auto
from io import BufferedReader
import json
import os
import struct
from typing import Union
import glob


class DataTypes(Flag):
    Char = auto()
    Short = auto()
    Int = auto()
    Float = auto()
    Long = auto()
    Double = auto()


CHAR = 1
SHORT = 2
INT = 4
FLOAT = 4
LONG = 8
DOUBLE = 8


class Endian(Enum):
    Little: str = "<"
    Big: str = ">"


class BinaryReader:
    _endian: Endian
    _fileObj: BufferedReader

    def __init__(self, fileObj: BufferedReader, endian: Endian = Endian.Little):
        self._fileObj = fileObj
        self._endian = endian

    def seek(self, offset: Union[int, DataTypes], whence: int = 0) -> int:
        if isinstance(offset, DataTypes):
            offset = offset.value
        return self._fileObj.seek(offset, whence)

    def read(self, size: int) -> bytes:
        return self._fileObj.read(size)

    def readValue(self, fmt: str, size: int) -> Union[int, float]:
        value = self._fileObj.read(size)
        return struct.unpack(self._endian.value + fmt, value)[0]

    def readByte(self) -> bytes:
        return self.readValue("c", CHAR)

    def readSignedByte(self) -> int:
        return self.readValue("b", CHAR)

    def readUnsignedByte(self) -> int:
        return self.readValue("B", CHAR)

    def readShort(self) -> int:
        return self.readValue("h", SHORT)

    def readUnsignedShort(self) -> int:
        return self.readValue("H", SHORT)

    def readInt(self) -> int:
        return self.readValue("i", INT)

    def readUnsignedInt(self) -> int:
        return self.readValue("I", INT)

    def readFloat(self) -> float:
        return self.readValue("f", FLOAT)

    def readLong(self) -> int:
        return self.readValue("l", LONG)

    def readDouble(self) -> float:
        return self.readValue("d", DOUBLE)

    def readString(self) -> str:
        string = ""
        while True:
            char = self.read(1)
            if char == b"\x00":
                break
            string += char.decode('ascii')
        return string
