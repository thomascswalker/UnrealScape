from __future__ import annotations

import logging
import abc
import os
from typing import List
from logger import getLogger
from enum import IntEnum
from buffer import ByteBuffer
import zlib

import crc
import xtea

logger = getLogger(__file__, logging.INFO)


# https://www.rune-server.ee/runescape-development/rs-503-client-and-server/snippets/659324-model-encoder.html

def getJagexCache() -> str:
    userProfile = os.environ.get("userprofile")
    if not userProfile:
        raise Exception("Unable to find user profile.")
    jagexCache = os.path.join(userProfile, "jagexcache\\oldschool\\LIVE")
    if not os.path.exists(jagexCache):
        raise FileNotFoundError(f"Unable to find Jagex Cache at {jagexCache}")
    return jagexCache


class Format(IntEnum):
    Unversioned = 5
    Versioned = 6
    VersionedLarge = 7


class IndexType(IntEnum):
    Animations = 0
    Skeletons = 1
    Configs = 2
    Interfaces = 3
    SoundEffects = 4
    Maps = 5
    MusicTracks = 6
    Models = 7
    Sprites = 8
    Textures = 9
    Binary = 10
    Master = 255


class Archive:
    pass


class Index:
    id: int
    protocol: int = 6
    revision: int = 0
    crc: int = 0
    compression: int = 0
    archives: List[Archive]

    def __init__(self, id: int):
        self.id = id
        self.archives = []


class IndexFile:  # type: ignore # noqa
    filename: str
    indexEntrySize: int
    buffer: ByteBuffer
    indexFileId: int

    def __init__(self, filename: str):
        ...

    def getIndexCount(self) -> int:
        ...

    def read(self, id: int) -> IndexEntry:
        ...


class Storage:
    @abc.abstractmethod
    def initialize(self, store: Store) -> None:
        pass

    @abc.abstractmethod
    def load(self, store: Store) -> None:
        pass


class DiskStorage(Storage):
    pass


class Format(IntEnum):
    Unversioned = 5
    Versioned = 6
    VersionedLarge = 7


class IndexType(IntEnum):
    Animations = 0
    Skeletons = 1
    Configs = 2
    Interfaces = 3
    SoundEffects = 4
    Maps = 5
    MusicTracks = 6
    Models = 7
    Sprites = 8
    Textures = 9
    Binary = 10
    MusicJingles = 11
    ClientScript = 12
    Fonts = 13
    MusicSamples = 14
    MusicPatches = 15
    WorldMapGeography = 18
    WorldMap = 19
    WorldMapGround = 20
    DbTableIndex = 21
    Master = 255

    @staticmethod
    def isValidIndex(index: int) -> bool:
        try:
            IndexType(index)
            return True
        except ValueError:
            return False


class IndexEntry:
    indexFile: IndexFile = IndexFile("")
    id: int = 0
    sector: int = 0
    length: int = 0

    def __str__(self) -> str:
        return f"Index Id: {self.id}, Sector: {self.sector}, Length: {self.length}"


class DataFile:
    filename: str = ""
    sectorSize: int = 520
    data: ByteBuffer

    def __init__(self, filename: str):
        self.filename = filename
        with open(filename, "rb") as f:
            self.data = ByteBuffer(bytearray(f.read()))
            logger.debug(
                f"Loaded data file into buffer: {self.data.size()} bytes")

    # https://github.com/runelite/runelite/blob/0358f03227d1282a58d7d50e7354a71286342087/cache/src/main/java/net/runelite/cache/fs/jagex/DataFile.java#L36
    def read(self, indexId: int, archiveId: int, sector: int, size: int) -> bytearray:
        logger.info(
            f"Reading data for {IndexType(archiveId).name}, sector {sector}, size {size}")

        # Create new buffers
        readBuffer = bytearray()
        outBuffer = bytearray()

        readBytesCount = 0
        nextSector = 0
        currentIndex = 0

        logger.debug(f"Size: {size}")
        while size > readBytesCount:
            logger.debug(f"Current index: {currentIndex}")
            if sector == 0:
                logger.warning("Unexpected end of file; sector == 0.")
                return bytearray()

            self.data.seek(self.sectorSize * sector)
            logger.debug(
                f"Reading sector {sector} at position {self.data.readerIndex}")

            dataBlockSize = size - readBytesCount

            logger.debug(f"Archive ID: {archiveId}")
            if archiveId > 65535:
                headerSize = 10
                diff = self.sectorSize - headerSize
                dataBlockSize = diff if dataBlockSize > diff else dataBlockSize

                readSize = headerSize + dataBlockSize
                readBuffer = self.data.readBytes(readSize)

                currentArchive = (
                    ((readBuffer[0] & 0xFF) << 24)
                    | ((readBuffer[1] & 0xFF) << 16)
                    | ((readBuffer[2] & 0xFF) << 8)
                    | (readBuffer[3] & 0xFF)
                )
                currentPart = ((readBuffer[4] & 0xFF)
                               << 8) + (readBuffer[5] & 0xFF)
                nextSector = (
                    ((readBuffer[6] & 0xFF) << 16)
                    | ((readBuffer[7] & 0xFF) << 8)
                    | (readBuffer[8] & 0xFF)
                )
                currentIndex = readBuffer[9] & 0xFF
            else:
                headerSize = 8
                diff = self.sectorSize - headerSize
                dataBlockSize = diff if dataBlockSize > diff else dataBlockSize

                readSize = headerSize + dataBlockSize
                readBuffer = self.data.readBytes(readSize)

                currentArchive = ((readBuffer[0] & 0xFF) << 8) | (
                    readBuffer[1] & 0xFF)
                currentPart = ((readBuffer[2] & 0xFF) << 8) | (
                    readBuffer[3] & 0xFF)
                nextSector = (
                    ((readBuffer[4] & 0xFF) << 16)
                    | ((readBuffer[5] & 0xFF) << 8)
                    | (readBuffer[6] & 0xFF)
                )
                currentIndex = readBuffer[7] & 0xFF

            outBuffer.extend(readBuffer)
            readBytesCount += dataBlockSize
            logger.debug(f"Read bytes count: {readBytesCount}")
            logger.debug(f"Next sector: {nextSector}")
            sector = nextSector

        return outBuffer


class IndexFile:  # type: ignore # noqa
    filename: str = ""
    indexEntrySize: int = 6
    buffer: ByteBuffer = ByteBuffer(0)
    indexFileId: int = 0

    def __init__(self, filename: str):
        self.filename = filename
        with open(filename, "rb") as f:
            self.buffer = ByteBuffer(bytearray(f.read()))
            logger.debug(
                f"Loaded new index into buffer: {self.buffer.size()} bytes")

    def getIndexCount(self) -> int:
        return int(self.buffer.size() / self.indexEntrySize)

    def read(self, id: int) -> IndexEntry:
        # Seek to the new offset position
        offset = id * self.indexEntrySize
        self.buffer.seek(offset)

        # Read the 6 bytes at this position
        tempBuffer = self.buffer.readBytes(self.indexEntrySize)

        # Generate the index entry from this 6-byte buffer
        entry = IndexEntry()
        entry.indexFile = self
        entry.id = id
        entry.length = (
            ((tempBuffer[0] & 0xFF) << 16)
            | ((tempBuffer[1] & 0xFF) << 8)
            | ((tempBuffer[2] & 0xFF))
        )
        entry.sector = (
            ((tempBuffer[3] & 0xFF) << 16)
            | ((tempBuffer[4] & 0xFF) << 8)
            | ((tempBuffer[5] & 0xFF))
        )
        return entry


class Store:
    storage: Storage
    indexes: List[Index]

    def __init__(self, *args):
        if len(args) != 1:
            raise ValueError("Wanted only one argument.")
        if isinstance(args[0], str):
            self.storage = DiskStorage()
        elif isinstance(args[0], Storage):
            self.storage = args[0]

        self.indexes = []
        self.storage.initialize(self)

    def addIndex(self, id: int) -> Index:
        index = Index(id)
        self.indexes.append(index)
        return index

    def load(self) -> None:
        self.storage.load(self)

    def getIndexes(self) -> List[Index]:
        return self.indexes


class CompressionType(IntEnum):
    Uncompressed = 0
    Bz2 = 1
    Gz = 2


class Container:
    data: bytearray = bytearray()
    compression: int = 0
    revision: int = 0
    crc: int = 0

    def __init__(self, compression, revision):
        self.compression = compression
        self.revision = revision

    @staticmethod
    def decrypt(data: bytearray, length: int, keys: List[int] = []) -> bytearray:
        if len(keys) == 0:
            return data

    # https://github.com/runelite/runelite/blob/0358f03227d1282a58d7d50e7354a71286342087/cache/src/main/java/net/runelite/cache/fs/Container.java#L93
    @staticmethod
    def decompress(b: bytearray, keys: List[int] = []) -> Container:
        stream = ByteBuffer(b)
        compression = stream.readUnsignedByte()
        logger.info(f"Compression type: {compression}")
        compressedLength = stream.readInt()
        logger.info(f"Compressed length: {compressedLength}")
        if compressedLength < 0 or compressedLength > 1000000:
            raise RuntimeError("Invalid data.")
        crc32 = crc.Calculator(crc.Crc32.CRC32, optimized=True)

        data = bytearray()
        revision = -1
        if compression == CompressionType.Uncompressed:
            stream.readBytes(compressedLength)  # TODO: Resolve this
            if stream.remaining() >= 2:
                revision = stream.readUnsignedShort()
                logger.info(f"Found revision: {revision}")
                assert revision != -1
            data = stream.getRemaining()
        else:
            raise ValueError(f"Unknown compression type: {compression}")

        container = Container(compression, revision)
        container.data = b
        container.crc = crc32.checksum(data)

        logger.info(f"Container checksum: {container.crc}")

        return container


class DiskStorage(Storage):  # type: ignore # noqa
    root: str = ""

    mainFileCacheDat: str = "main_file_cache.dat2"
    mainFileCacheIdx: str = "main_file_cache.idx"

    dataFile: DataFile
    indexFileMaster: IndexFile
    indexFiles: List[IndexFile]

    def __init__(self):
        self.root = getJagexCache()
        self.indexFiles = []
        self.dataFile = DataFile(os.path.join(
            self.root, self.mainFileCacheDat))
        indexMasterFilename = f"{self.mainFileCacheIdx}255"
        self.indexFileMaster = IndexFile(
            os.path.join(self.root, indexMasterFilename))

    # Override
    def initialize(self, store: Store) -> None:
        for i in range(self.indexFileMaster.getIndexCount() + 1):
            if not IndexType.isValidIndex(i):
                continue
            if not i == IndexType.Configs:
                continue
            store.addIndex(i)
            self.addIndexFile(i)

    # Override
    def load(self, store: Store) -> None:
        for index in store.getIndexes():
            self.loadIndex(index)

    def readIndex(self, id: int) -> ByteBuffer:
        logger.debug(f"Reading index: {IndexType(id).name}")
        entry = self.indexFileMaster.read(id)
        indexData = self.dataFile.read(
            self.indexFileMaster.indexFileId, entry.id, entry.sector, entry.length
        )
        return ByteBuffer(indexData)

    def loadIndex(self, index: Index) -> None:
        logger.debug(f"Loading index: {index.id}")
        indexData = self.readIndex(index.id)
        logger.success(
            f"Read index {IndexType(index.id).name}: {indexData.size()}")
        if indexData.size() == 0:
            return
        container = Container.decompress(indexData.data)
        logger.info(container.data)
        logger.info(container.data.decode())
        return

    def addIndexFile(self, id: int) -> None:
        # Build the index filename from the given id
        indexFilename = os.path.join(self.root, f"{self.mainFileCacheIdx}{id}")

        # Create a new IndexFile repr
        indexFile = IndexFile(indexFilename)

        # Add this file to the list of index files
        self.indexFiles.append(indexFile)

        # Log this
        count = indexFile.getIndexCount()
        logger.info(f"Added index file: {indexFilename} ({count})")


if __name__ == "__main__":
    cache = getJagexCache()
    store = Store(cache)
    store.load()
