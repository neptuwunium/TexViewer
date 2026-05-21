#include "ConverterBlockCompressed.h"

int
getDecompressedHeight(const ImageData &info, const int width, const size_t dataSize)
{
    const int blocksX = (width + info.blockWidth - 1) / info.blockWidth;
    const int totalBlocks = static_cast<int>(dataSize) / info.blockSize;
    const int blocksY = totalBlocks / blocksX;
    return blocksY * info.blockHeight;
}

int
convertBlocksSafe(char* inData, const size_t inDataLen, char* outData, const size_t outDataLen, const int width, const int, const ImageData &info)
{
    if (inData == nullptr || outData == nullptr)
        return 0;

    const decodeBlockFunc func = info.decodeBlock;

    if (func == nullptr)
        return 0;

    const int dstRowPitch = width * info.pitch;

    const char* src = inData;

    int y = 0;
    while (true)
    {
        for (int x = 0; x < width; x += info.blockWidth)
        {
            // input oob
            if (src + info.blockSize - inData > static_cast<ptrdiff_t>(inDataLen))
                return y;

            // output oob
            const size_t maxWriteOffset =
                (y * width + x) * info.pitch +
                (info.blockHeight - 1) * dstRowPitch +
                info.blockWidth * info.pitch;

            if (maxWriteOffset > outDataLen)
                return y;

            char *dst = outData + (y * width + x) * info.pitch;

            func(src, dst, dstRowPitch);
            src += info.blockSize;
        }
        y += info.blockHeight;
    }
}
