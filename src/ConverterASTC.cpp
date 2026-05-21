#include "ConverterBlockCompressed.h"

#include "astcenc.h"

int convertAstc(char* inData, size_t inDataLen, char* outData, size_t outDataLen, const int width, int height, const ImageData &info)
{
    if (inData == nullptr || outData == nullptr || width <= 0 || outDataLen == 0)
        return 0;

    int estHeight = static_cast<int>(outDataLen) / width / info.pitch;

    if (height == 0 || estHeight < height)
        height = estHeight;

    astcenc_config config {};
    astcenc_error status = astcenc_config_init(
        ASTCENC_PRF_LDR,
        info.blockWidth,
        info.blockHeight,
        1,
        ASTCENC_PRE_FASTEST,
        ASTCENC_FLG_DECOMPRESS_ONLY,
        &config
    );

    if (status != ASTCENC_SUCCESS)
        return 0;

    astcenc_context* context = nullptr;
    status = astcenc_context_alloc(&config, 1, &context, nullptr);
    if (status != ASTCENC_SUCCESS)
        return 0;

    void* slices[1] = { reinterpret_cast<void*>(outData) };

    astcenc_image image {};
    image.dim_x = width;
    image.dim_y = height;
    image.dim_z = 1;
    image.data_type = ASTCENC_TYPE_U8;
    image.data = slices;

    constexpr astcenc_swizzle swizzle = { ASTCENC_SWZ_R, ASTCENC_SWZ_G, ASTCENC_SWZ_B, ASTCENC_SWZ_A };
    status = astcenc_decompress_image(context, reinterpret_cast<const uint8_t*>(inData), inDataLen, &image, &swizzle, 0);
    astcenc_context_free(context);

    if (status != ASTCENC_SUCCESS)
        return 0;

    return height;
}