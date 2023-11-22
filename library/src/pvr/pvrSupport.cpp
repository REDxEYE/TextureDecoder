#include <fstream>

#include "zstd.h"

#include "pvr/pvrSupport.h"
#include "decoders/etc//etc1.h"
#include "logging.h"
#include "textureApi.h"
#include "texture.h"
#include "shared.h"


bool loadPVR(const std::filesystem::path &filename, sTexture *texture) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }

    size_t fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(fsize);
    if (!file.read(reinterpret_cast<char *>(data.data()), fsize)) {
        return false;
    }

    bool res = loadPVR(data.data(), fsize, texture);
    file.close();
    return res;
}

bool loadPVR(uint8_t *data, size_t dataSize, sTexture *texture) {
    if (dataSize < sizeof(sPvrHeader)) {
        loggerEx(eLogLevel::ERROR, "Not enough data to read PVR ident. Possibly truncated file.\n");
        return false;
    }
    sPvrHeader header{};
    memcpy(&header, data, sizeof(sPvrHeader));
    data += sizeof(sPvrHeader);
    dataSize -= sizeof(sPvrHeader);

    if (header.version != MAKE_FOURCC('P', 'V', 'R', '\x03')) {
        loggerEx(eLogLevel::ERROR, "Invalid PVR3 header.\n");
        return false;
    }
    uint32_t metaDataSize = header.metaDataSize;
    std::vector<sPvrMetaData> metas;
    while (metaDataSize > 12) {
        sPvrMetaData metaData{};
        memcpy(&metaData, data, 12); // It's okay to copy data to first 12 bytes of this struct.
        data += 12;
        dataSize -= 12;
        if (metaData.size > 0) {
            if (metaData.size > dataSize) {
                loggerEx(eLogLevel::ERROR, "Not enough data to read metadata data. Possibly truncated file.\n");
                return false;
            }
            metaData.data.assign(data, data + metaData.size);
            data += metaData.size;
            dataSize -= metaData.size;
        }
        metaDataSize -= 12 + metaData.size;
        metas.emplace_back(metaData);
    }
    std::vector<uint8_t> mip0Data;
    for (const auto &item: metas) {
        if (item.ident == MAKE_FOURCC('z', 's', 't', 'd') && item.key == MAKE_FOURCC('j', 'e', 't', 'c')) {
            if (item.size < 108) {
                loggerEx(eLogLevel::ERROR, "Unexpected size of zstd block.\n");
                return false;
            }
            const uint32_t *metaData = reinterpret_cast<const uint32_t *>(item.data.data());
            mip0Data.resize(metaData[2]);
            auto dSize = ZSTD_decompress(mip0Data.data(), metaData[2], data, metaData[1]);
            if (ZSTD_isError(dSize)) {
                loggerEx(eLogLevel::ERROR, "Unexpected size of zstd block.\n");
                return false;
            }
            mip0Data.resize(dSize);
        }
    }
    if (mip0Data.empty()) {
        mip0Data.assign(data, data + dataSize);
    }

    ePixelFormat outputFormat = ePixelFormat::INVALID;
    if (header.channelBBP[0] == 0 && header.channelBBP[1] == 0 && header.channelBBP[2] == 0 && header.channelBBP[3] == 0) {
        switch (header.pixelFormat) {

            case ePvrPixelFormat::PVRTC_2bpp_RGB:
                outputFormat = ePixelFormat::RGB888;
                break;
            case ePvrPixelFormat::PVRTC_2bpp_RGBA:
                outputFormat = ePixelFormat::RGBA8888;
                break;
            case ePvrPixelFormat::PVRTC_4bpp_RGB:
                outputFormat = ePixelFormat::RGB888;
                break;
            case ePvrPixelFormat::PVRTC_4bpp_RGBA:
                outputFormat = ePixelFormat::RGBA8888;
                break;
            case ePvrPixelFormat::PVRTCII_2bpp:
                break;
            case ePvrPixelFormat::PVRTCII_4bpp:
                break;
            case ePvrPixelFormat::ETC1:
                outputFormat = ePixelFormat::RGB888;
                break;
            case ePvrPixelFormat::BC1:
            case ePvrPixelFormat::DXT1:
                outputFormat = ePixelFormat::BC1;
                break;
            case ePvrPixelFormat::BC2:
            case ePvrPixelFormat::DXT2:
            case ePvrPixelFormat::DXT3:
                outputFormat = ePixelFormat::BC2;
                break;
            case ePvrPixelFormat::BC3:
            case ePvrPixelFormat::DXT4:
            case ePvrPixelFormat::DXT5:
                outputFormat = ePixelFormat::BC3;
                break;
            case ePvrPixelFormat::BC4:
                outputFormat = ePixelFormat::BC4;
                break;
            case ePvrPixelFormat::BC5:
                outputFormat = ePixelFormat::BC5;
                break;
            case ePvrPixelFormat::BC6:
                outputFormat = ePixelFormat::BC6;
                break;
            case ePvrPixelFormat::BC7:
                outputFormat = ePixelFormat::BC7;
                break;
            case ePvrPixelFormat::UYVY:
                break;
            case ePvrPixelFormat::YUY2:
                break;
            case ePvrPixelFormat::BW1bpp:
                break;
            case ePvrPixelFormat::R9G9B9E5_SharedExponent:
                break;
            case ePvrPixelFormat::RGBG8888:
                break;
            case ePvrPixelFormat::GRGB8888:
                break;
            case ePvrPixelFormat::ETC2_RGB:
                break;
            case ePvrPixelFormat::ETC2_RGBA:
                break;
            case ePvrPixelFormat::ETC2_RGB_A1:
                break;
            case ePvrPixelFormat::EAC_R11_Unsigned:
                break;
            case ePvrPixelFormat::EAC_R11_Signed:
                break;
            case ePvrPixelFormat::EAC_RG11_Unsigned:
                break;
            case ePvrPixelFormat::EAC_RG11_Signed:
                break;
            case ePvrPixelFormat::ASTC_4x4:
                break;
            case ePvrPixelFormat::ASTC_5x4:
                break;
            case ePvrPixelFormat::ASTC_5x5:
                break;
            case ePvrPixelFormat::ASTC_6x5:
                break;
            case ePvrPixelFormat::ASTC_6x6:
                break;
            case ePvrPixelFormat::ASTC_8x5:
                break;
            case ePvrPixelFormat::ASTC_8x6:
                break;
            case ePvrPixelFormat::ASTC_8x8:
                break;
            case ePvrPixelFormat::ASTC_10x5:
                break;
            case ePvrPixelFormat::ASTC_10x6:
                break;
            case ePvrPixelFormat::ASTC_10x8:
                break;
            case ePvrPixelFormat::ASTC_10x10:
                break;
            case ePvrPixelFormat::ASTC_12x10:
                break;
            case ePvrPixelFormat::ASTC_12x12:
                break;
        }
    } else {
        loggerEx(eLogLevel::ERROR, "Custom PVR pixel formats are not supported.\n");
        return false;
    }
    if (outputFormat == ePixelFormat::INVALID) {
        loggerEx(eLogLevel::ERROR, std::format("Unsupported PVR pixel format {}.\n", (uint32_t) header.pixelFormat));
        return false;
    }
    texture->m_width = header.width;
    texture->m_height = header.height;
    texture->m_pixelFormat = outputFormat;
    if (isCompressedPixelFormat(outputFormat)) {
        if (mip0Data.size() < calculateTextureSize(texture)) {
            loggerEx(eLogLevel::ERROR, "Not enough data for texture.\n");
            return false;
        }
        texture->m_rawPixelData.assign(mip0Data.begin(), mip0Data.begin() + calculateTextureSize(texture));

    } else {
        texture->m_rawPixelData.resize(calculateTextureSize(texture));

        if(header.pixelFormat==ePvrPixelFormat::ETC1) {
            const uint64_t *src = reinterpret_cast<uint64_t *>(mip0Data.data());
            uint8_t *dst = texture->m_rawPixelData.data();
            uint8_t *output = dst;
            uint32_t i, j;
            for (i = 0; i < texture->m_height; i += 4) {
                for (j = 0; j < texture->m_width; j += 4) {
                    dst = output + (i * texture->m_width + j) * 3;
                    decode_etc1(*src, dst, texture->m_width);
                    src++;
                }
            }
        }else{
            loggerEx(eLogLevel::ERROR, std::format("Unsupported PVR pixel format {}.\n", (uint32_t) header.pixelFormat));
            return false;
        }
    }
    return true;
}
