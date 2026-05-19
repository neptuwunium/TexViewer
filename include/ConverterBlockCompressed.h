#pragma once

#include <QtGui/QImage>

#include <map>
#include "Meta.h"

#ifdef TEXVIEWER_BPTC
#define BCDEC_BC4BC5_PRECISE
#define BCDEC_BC3_PRECISE
#include "bcdec.h"

// Specializations of bcdec's block decode functions
void bcdec_bc4u(const void* compressedBlock, void* decompressedBlock, int destinationPitch);
void bcdec_bc5u(const void* compressedBlock, void* decompressedBlock, int destinationPitch);
void bcdec_bc6hu(const void* compressedBlock, void* decompressedBlock, int destinationPitch);
void bcdec_bc6hs(const void* compressedBlock, void* decompressedBlock, int destinationPitch);
#endif

struct ImageData;

typedef void (*decodeBlockFunc)(const void*, void*, int);
typedef int (*decodeFunc)(char*, size_t, char*, size_t, int, int, const ImageData &info);

struct ImageData
{
    int blockSize;
    int blockWidth;
    int blockHeight;
    int pitch; // bpp
    decodeBlockFunc decodeBlock;
    decodeFunc decode;
    QImage::Format qFormat;
};

int getDecompressedHeight(const ImageData &info, int width, size_t dataSize);
int convertBlocksSafe(char* inData, size_t inDataLen, char* outData, size_t outDataLen, int width, int height, const ImageData &info);
int convertAstc(char* inData, size_t inDataLen, char* outData, size_t outDataLen, int width, int height, const ImageData &info);

enum ImageFormat
{
    BC1,
    BC2,
    BC3,
    BC4,
    BC5,
    BC6HS,
    BC6HU,
    BC7,

    ASTC4x4,
    ASTC5x4,
    ASTC5x5,
    ASTC6x5,
    ASTC6x6,
    ASTC8x5,
    ASTC8x6,
    ASTC8x8,
    ASTC10x5,
    ASTC10x6,
    ASTC10x8,
    ASTC10x10,
    ASTC12x10,
    ASTC12x12,

    ETC1,
    EAC1U,
    EAC1S,
    EAC2U,
    EAC2S,
    ETC2,
    ETC2A,
};

#ifdef TEXVIEWER_BPTC
#define BPTC_DATA(blockSize, pitch, func, format) \
        { blockSize, 4, 4, pitch, &func, &convertBlocksSafe, QImage::format }
#else
#define BPTC_DATA(blockSize, pitch, func, format) \
        { blockSize, 4, 4, pitch, nullptr, nullptr, QImage::format }
#endif

#ifdef TEXVIEWER_ASTC
#define ASTC_DATA(blockSize, blockWidth, blockHeight, format) \
        { blockSize, blockWidth, blockHeight, 4, nullptr, &convertAstc, QImage::format }
#else
#define ASTC_DATA(blockSize, blockWidth, blockHeight, format) \
        { blockSize, blockWidth, blockHeight, 4, nullptr, nullptr, QImage::format }
#endif

#ifdef TEXVIEWER_ETC
#define ETC_DATA(blockSize, blockWidth, blockHeight, pitch, func, format) \
        { blockSize, blockWidth, blockHeight, pitch, &func, &convertBlocksSafe, QImage::format }
#else
#define ETC_DATA(blockSize, blockWidth, blockHeight, pitch, func, format) \
        { blockSize, blockWidth, blockHeight, pitch, nullptr, nullptr, QImage::format }
#endif

static const std::map<ImageFormat, ImageData> g_imageFormatData = {
    {BC1,   BPTC_DATA( 8,  4, bcdec_bc1,   Format_RGBA8888)},
    {BC2,   BPTC_DATA(16,  4, bcdec_bc2,   Format_RGBA8888)},
    {BC3,   BPTC_DATA(16,  4, bcdec_bc3,   Format_RGBA8888)},
    {BC4,   BPTC_DATA( 8,  1, bcdec_bc4u,  Format_Grayscale8)},
    {BC5,   BPTC_DATA(16,  3, bcdec_bc5u,  Format_RGB888)},
    {BC6HU, BPTC_DATA(16, 16, bcdec_bc6hu, Format_RGBX32FPx4)},
    {BC6HS, BPTC_DATA(16, 16, bcdec_bc6hs, Format_RGBX32FPx4)},
    {BC7,   BPTC_DATA(16,  4, bcdec_bc7,   Format_RGBA8888)},

    { ASTC4x4, ASTC_DATA(16, 4, 4, Format_RGBA8888) },
    { ASTC5x4, ASTC_DATA(16, 5, 4, Format_RGBA8888) },
    { ASTC5x5, ASTC_DATA(16, 5, 5, Format_RGBA8888) },
    { ASTC6x5, ASTC_DATA(16, 6, 5, Format_RGBA8888) },
    { ASTC6x6, ASTC_DATA(16, 6, 6, Format_RGBA8888) },
    { ASTC8x5, ASTC_DATA(16, 8, 5, Format_RGBA8888) },
    { ASTC8x6, ASTC_DATA(16, 8, 6, Format_RGBA8888) },
    { ASTC8x8, ASTC_DATA(16, 8, 8, Format_RGBA8888) },
    { ASTC10x5, ASTC_DATA(16, 10, 5, Format_RGBA8888) },
    { ASTC10x6, ASTC_DATA(16, 10, 6, Format_RGBA8888) },
    { ASTC10x8, ASTC_DATA(16, 10, 8, Format_RGBA8888) },
    { ASTC10x10, ASTC_DATA(16, 10, 10, Format_RGBA8888) },
    { ASTC12x10, ASTC_DATA(16, 12, 10, Format_RGBA8888) },
    { ASTC12x12, ASTC_DATA(16, 12, 12, Format_RGBA8888) },
};

#undef BPTC_DATA
#undef ASTC_DATA
#undef ETC_DATA
