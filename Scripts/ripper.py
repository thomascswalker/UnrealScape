import logging
import os
from logger import get_logger
from enum import IntEnum
from binary import BinaryReader

logger = get_logger(__file__, logging.INFO)


def getJagexCache() -> str:
    userProfile = os.environ.get("userprofile")
    jagexCache = os.path.join(userProfile, "jagexcache\\oldschool\\LIVE")
    if not os.path.exists(jagexCache):
        raise FileNotFoundError(f"Unable to find Jagex Cache at {jagexCache}")
    return jagexCache


class IndexType(IntEnum):
    Test = 2
    Models = 7


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

    # https://github.com/runelite/runelite/blob/e3840ca1fadef104ca473fb26029432b3063e855/cache/src/main/java/net/runelite/cache/index/IndexData.java
    def read(self) -> None:
        if self.filename == "":
            logger.error("Unable to read, invalid source file.")

        with open(self.filename, 'rb') as f:
            reader = BinaryReader(f)

            # Get the protocol
            protocol = reader.readUnsignedChar()
            logger.info(f"Protocol {protocol}")

            hashValue = reader.readUnsignedChar()
            logger.info(f"Hash {hashValue}")

            named = (1 & hashValue) != 0
            logger.info(f"Named {named}")

            archiveCount = reader.readUnsignedShort()
            logger.info(f"Archive count: {archiveCount}")

            archives = []
            for i in range(archiveCount):
                archiveId = reader.readUnsignedShort()
                logger.debug(f"Archive {i}: {archiveId}")


if __name__ == "__main__":
    modelsIndexFile = IndexFile(IndexType.Models)
    modelsIndexFile.read()
