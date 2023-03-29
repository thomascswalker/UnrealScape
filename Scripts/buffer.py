from enum import Flag, auto
import struct
from typing import Any, overload

import logging

logger = logging.getLogger()


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


class ByteBuffer:
    readerIndex: int = 0
    writerIndex: int = 0
    data: bytearray

    LITTLE_ENDIAN = "<"
    BIG_ENDIAN = ">"

    def __init__(self, *args):
        if len(args) == 0 or len(args) > 2:
            raise TypeError(f"Invalid argument count: {len(args)}")
        if isinstance(args[0], bytearray):
            self.data = args[0]
        elif isinstance(args[0], int):
            self.data = bytearray(args[0])
        else:
            raise TypeError(f"Invalid arguments: {args}")

    def __getitem__(self, index: int) -> int:
        return self.data[self.readerIndex + index]

    def size(self) -> int:
        return len(self.data)

    def seek(self, pos: int) -> int:
        self.readerIndex = pos
        return self.readerIndex

    def remaining(self) -> int:
        return (len(self.data) - 1) - self.readerIndex

    def readValue(self, fmt: str, size: int) -> Any:
        value = self.data[self.readerIndex : self.readerIndex + size]  # noqa
        self.readerIndex += size
        return struct.unpack(self.LITTLE_ENDIAN + fmt, value)[0]

    def readByte(self) -> bytes:
        return self.readValue("c", CHAR)

    def readBytes(self, size: int) -> bytearray:
        buffer = bytes()
        i = 0
        while i < size:
            b = self.readByte()
            buffer += b
            i += 1
        return bytearray(buffer)

    def readBytesAt(self, size: int, offset: int) -> bytearray:
        oldPos = self.readerIndex
        self.seek(offset)
        buffer = self.readBytes(size)
        self.seek(oldPos)
        return buffer

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

    def readUnsignedMedium(self) -> int:
        value = self.data[self.readerIndex : self.readerIndex + 3]  # noqa
        self.readerIndex += 3
        return int.from_bytes(value, "big", signed=False)

    def readSignedMedium(self) -> int:
        value = self.data[self.readerIndex : self.readerIndex + 3]  # noqa
        self.readerIndex += 3
        return int.from_bytes(value, "big", signed=True)

    def readMedium(self) -> int:
        return self.readUnsignedMedium()

    def readLong(self) -> int:
        return self.readValue("l", LONG)

    def readDouble(self) -> float:
        return self.readValue("d", DOUBLE)

    def readString(self) -> str:
        string = ""
        while True:
            char = self.readByte()
            if char == b"\x00" or char is None:
                break
            string += char.decode("ascii")
        return string

    def writeValue(self, src: bytearray, srcIndex: int, length: int) -> None:
        """Transfers the specified `src` bytearray's data to this buffer at the
        current `writerIndex` and increases the `writerIndex` by the number
        of transferred bytes (`length`)."""
        self.data += src[srcIndex : srcIndex + length]  # noqa
        self.writerIndex += length
