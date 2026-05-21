#pragma once

#include <QtGui/QImage>

#include <map>
#include "Meta.h"

struct ImageData;

#define MAKE_SPEC(name) void name(const void*, void*, int)

#ifdef TEXVIEWER_BPTC
#define BCDECDEF inline
#define BCDEC_BC4BC5_PRECISE
#define BCDEC_BC3_PRECISE
#include "bcdec.h"

// Specializations of bcdec's block decode functions
MAKE_SPEC(bcdec_bc4u);
MAKE_SPEC(bcdec_bc5u);
MAKE_SPEC(bcdec_bc6hu);
MAKE_SPEC(bcdec_bc6hs);
#endif

#ifdef TEXVIEWER_ASTC
int convertAstc(char* inData, size_t inDataLen, char* outData, size_t outDataLen, int width, int height, const ImageData &info);
#endif

#ifdef TEXVIEWER_ETC
MAKE_SPEC(etcpack_etc1);
MAKE_SPEC(etcpack_etc2);
MAKE_SPEC(etcpack_etc2a1);
MAKE_SPEC(etcpack_etc2a8);
MAKE_SPEC(etcpack_eac1);
MAKE_SPEC(etcpack_eac2);
#endif

#undef MAKE_SPEC

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
    ETC2,
    ETC2A1,
    ETC2A8,
    EAC1,
    EAC2,
};

#ifdef TEXVIEWER_BPTC
#define BPTC_DATA(blockSize, pitch, func, format) \
        { blockSize, 4, 4, pitch, &func, &convertBlocksSafe, QImage::format }
#else
#define BPTC_DATA(blockSize, pitch, func, format) \
        { blockSize, 4, 4, pitch, nullptr, nullptr, QImage::format }
#endif

#ifdef TEXVIEWER_ASTC
#define ASTC_DATA(blockSize, blockWidth, blockHeight) \
        { blockSize, blockWidth, blockHeight, 4, nullptr, &convertAstc, QImage::Format_RGBA8888 }
#else
#define ASTC_DATA(blockSize, blockWidth, blockHeight) \
        { blockSize, blockWidth, blockHeight, 4, nullptr, nullptr, QImage::Format_RGBA8888 }
#endif

#ifdef TEXVIEWER_ETC
#define ETC_DATA(blockSize, pitch, func, format) \
        { blockSize, 4, 4, pitch, &func, &convertBlocksSafe, QImage::format }
#else
#define ETC_DATA(blockSize, pitch, func, format) \
        { blockSize, 4, 4, pitch, nullptr, nullptr, QImage::format }
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

    { ASTC4x4,   ASTC_DATA(16,  4,  4) },
    { ASTC5x4,   ASTC_DATA(16,  5,  4) },
    { ASTC5x5,   ASTC_DATA(16,  5,  5) },
    { ASTC6x5,   ASTC_DATA(16,  6,  5) },
    { ASTC6x6,   ASTC_DATA(16,  6,  6) },
    { ASTC8x5,   ASTC_DATA(16,  8,  5) },
    { ASTC8x6,   ASTC_DATA(16,  8,  6) },
    { ASTC8x8,   ASTC_DATA(16,  8,  8) },
    { ASTC10x5,  ASTC_DATA(16, 10,  5) },
    { ASTC10x6,  ASTC_DATA(16, 10,  6) },
    { ASTC10x8,  ASTC_DATA(16, 10,  8) },
    { ASTC10x10, ASTC_DATA(16, 10, 10) },
    { ASTC12x10, ASTC_DATA(16, 12, 10) },
    { ASTC12x12, ASTC_DATA(16, 12, 12) },

    {ETC1,   ETC_DATA( 8, 3,   etcpack_etc1, Format_RGB888)},
    {ETC2,   ETC_DATA( 8, 3,   etcpack_etc2, Format_RGB888)},
    {ETC2A1, ETC_DATA( 8, 4, etcpack_etc2a1, Format_RGBA8888)},
    {ETC2A8, ETC_DATA(16, 4, etcpack_etc2a8, Format_RGBA8888)},
    {EAC1,   ETC_DATA( 8, 1,   etcpack_eac1, Format_Grayscale8)},
    {EAC2,   ETC_DATA(16, 3,   etcpack_eac2, Format_RGB888)},
};

#undef BPTC_DATA
#undef ASTC_DATA
#undef ETC_DATA
