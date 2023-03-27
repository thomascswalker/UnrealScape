import logging
import os
from typing import List
from logger import get_logger
from enum import IntEnum
from binary import BinaryReader, Endian
from collections import namedtuple
from dataclasses import dataclass, field

logger = get_logger(__file__, logging.INFO)


@dataclass
class FileData:
    id: int = 0
    nameHash: int = 0


@dataclass
class ArchiveData:
    id: int = 0
    nameHash: int = 0
    crc: int = 0
    """The cyclic redendancy check code. Used to validate against corruption."""
    version: int = 0
    """The version of the Js5 archive."""
    fileCount: int = 0
    files: List[FileData] = field(default_factory=list)

    def __str__(self) -> str:
        return f"ID: {self.id}, File Count: {self.fileCount}"


def getJagexCache() -> str:
    userProfile = os.environ.get("userprofile")
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


class IndexFile:
    filename: str
    data: bytes

    def __init__(self, index: IndexType):
        self.filename = self.getCacheFile(index)

    def getCacheFile(self, index: IndexType) -> str:
        jagexCache = getJagexCache()
        cacheFile = os.path.join(
            jagexCache, f"main_file_cache.idx{index.value}")
        if not os.path.exists(cacheFile):
            logger.error(
                f"Unable to find cache index {index.value}, {cacheFile}")
            return ""
        logger.success(f"Loaded IndexFile {cacheFile}")
        return cacheFile

    # https://github.com/Guthix/Jagex-Store-5/blob/7a66f2aa232bc1a963521e727d607a6a9a21dbed/filestore/src/main/kotlin/io/guthix/js5/Js5Archive.kt#L316
    def read(self) -> None:
        if self.filename == "":
            logger.error("Unable to read, invalid source file.")

        with open(self.filename, 'rb') as f:
            stream = BinaryReader(f)

            # Get the protocol
            fmt = stream.readUnsignedByte()
            if fmt not in [e.value for e in Format]:
                logger.error(f"Archive format {fmt} not supported.")
                return
            logger.info(f"Format {fmt}")

            # Get the hash value
            hashValue = stream.readUnsignedByte()
            logger.info(f"Hash {hashValue}")

            # Get if this archive is named
            named = (1 & hashValue) != 0
            logger.info(f"Named {named}")

            # Get the number of archives
            archiveCount = stream.readUnsignedShort()
            logger.info(f"Archive count: {archiveCount}")

            # Create the ArchiveData repr objects and set their respective ids
            archives: List[ArchiveData] = []
            for i in range(archiveCount):
                archiveData = ArchiveData()
                archiveId = stream.readUnsignedShort()
                logger.debug(f"Archive {i}: {archiveId}")
                archiveData.id = archiveId
                archives.append(archiveData)

            # If it's named, read the hash value for each archive
            if named:
                for archive in archives:
                    nameHash = stream.readInt()
                    archive.nameHash = nameHash

            # Read the crc value for each archive
            for archive in archives:
                crc = stream.readInt()
                archive.crc = crc

            # Read the revision value for each archive
            for archive in archives:
                revision = stream.readInt()
                archive.version = revision

            # Read the file count for each archive
            for archive in archives:
                fileCount = stream.readUnsignedShort()
                archive.fileCount = fileCount

            # Read each file id within the archive
            for archive in archives:
                offset = 0
                for i in range(archive.fileCount):
                    offset += stream.readUnsignedShort()
                    fileId = offset
                    logger.info(fileId)
                    fileData = FileData()
                    fileData.id = fileId
                    archive.files.append(fileData)


if __name__ == "__main__":
    modelsIndexFile = IndexFile(IndexType.Master)
    modelsIndexFile.read()
