#include "ConverterBlockCompressed.h"

#define BCDEC_IMPLEMENTATION
#define BCDEC_BC4BC5_PRECISE
#define BCDEC_BC3_PRECISE
#include "bcdec.h"

void
_bcdec_bc6h_as_RGBA(const void* compressedBlock, void* decompressedBlock, int destinationPitch, bool isSigned)
{
    unsigned short block[16*3];
    float* decompressed;
    const unsigned short* b;
    int i, j;

    bcdec_bc6h_half(compressedBlock, block, 4*3, isSigned);
    b = block;
    decompressed = (float*)decompressedBlock;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            // Previously: `j * 3`
            // Currently : `j * 4`
            decompressed[j * 4 + 0] = bcdec__half_to_float_quick(*b++);
            decompressed[j * 4 + 1] = bcdec__half_to_float_quick(*b++);
            decompressed[j * 4 + 2] = bcdec__half_to_float_quick(*b++);
        }
        decompressed += destinationPitch;
    }
}

void
bcdec_bc6hu(const void* compressedBlock, void* decompressedBlock, int destinationPitch)
{
    _bcdec_bc6h_as_RGBA(compressedBlock, decompressedBlock, destinationPitch / 4, false);
}

void
bcdec_bc6hs(const void* compressedBlock, void* decompressedBlock, int destinationPitch)
{
    _bcdec_bc6h_as_RGBA(compressedBlock, decompressedBlock, destinationPitch / 4, true);
}

void
bcdec_bc4u(const void* compressedBlock, void* decompressedBlock, int destinationPitch)
{
    bcdec_bc4(compressedBlock, decompressedBlock, destinationPitch / 4, false);
}

void
bcdec_bc5u(const void* compressedBlock, void* decompressedBlock, int destinationPitch)
{
    bcdec__bc4_block(compressedBlock, decompressedBlock, destinationPitch, 3, false);
    bcdec__bc4_block(((char*)compressedBlock) + 8, ((char*)decompressedBlock) + 1, destinationPitch, 3, false);
}
