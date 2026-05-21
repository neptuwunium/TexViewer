
#include "ConverterBlockCompressed.h"

#include "../lib/etc/source/etcdec.cxx"

#ifdef _WIN32
#include <cstdlib>
#define BSWAP_32 _byteswap_ulong
#elif defined(__clang__) || defined(__GNUC__)
#define BSWAP_32 __builtin_bswap32
#else
static uint32_t BSWAP_32 (const uint32_t x) {
    return
    x << 24 & 0xff000000 |
    x <<  8 & 0x00ff0000 |
    x >>  8 & 0x0000ff00 |
    x >> 24 & 0x000000ff;
}
#endif

void etcpack_etc1(const void* compressedBlock, void* decompressedBlock, int destinationPitch) {
    uint32_t block1 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[0]);
    uint32_t block2 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[1]);
    decompressBlockETC2c(block1, block2, static_cast<uint8*>(decompressedBlock), destinationPitch / 3, 4, 0, 0, 3);
}

void etcpack_etc2(const void* compressedBlock, void* decompressedBlock, int destinationPitch) {
    uint32_t block1 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[0]);
    uint32_t block2 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[1]);
    decompressBlockETC2c(block1, block2, static_cast<uint8*>(decompressedBlock), destinationPitch / 3, 4, 0, 0, 3);
}

void etcpack_etc2a1(const void* compressedBlock, void* decompressedBlock, int destinationPitch) {
    uint32_t block1 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[0]);
    uint32_t block2 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[1]);
    decompressBlockETC21BitAlphaC(block1, block2, static_cast<uint8*>(decompressedBlock), nullptr, destinationPitch / 4, 4, 0, 0, 4);
}

void etcpack_etc2a8(const void* compressedBlock, void* decompressedBlock, int destinationPitch) {
    decompressBlockAlphaC((uint8*) compressedBlock, static_cast<uint8*>(decompressedBlock) + 3, destinationPitch / 4, 4, 0, 0, 4);
    uint32_t block1 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[2]);
    uint32_t block2 = BSWAP_32(static_cast<const uint32_t *>(compressedBlock)[3]);
    decompressBlockETC2c(block1, block2, static_cast<uint8*>(decompressedBlock), destinationPitch / 4, 4, 0, 0, 4);
}

void etcpack_eac1(const void* compressedBlock, void* decompressedBlock, int destinationPitch) {
    decompressBlockAlphaC((uint8*) compressedBlock, static_cast<uint8*>(decompressedBlock), destinationPitch, 4, 0, 0, 1);
}

void etcpack_eac2(const void* compressedBlock, void* decompressedBlock, int destinationPitch) {
    decompressBlockAlphaC((uint8*) compressedBlock, static_cast<uint8*>(decompressedBlock), destinationPitch / 3, 4, 0, 0, 3);
    decompressBlockAlphaC((uint8*) compressedBlock + 8, static_cast<uint8*>(decompressedBlock) + 1, destinationPitch / 3, 4, 0, 0, 3);
}