#include <cstring>
#include <fstream>
#include <istream>

#include "dds/ddsSupport.h"
#include "texture.h"
#include "logging.h"

namespace fs = std::filesystem;

#define MAKE_FOURCC(ch0, ch1, ch2, ch3) \
        ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | \
        ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))

int bitShift(uint32_t mask);

void setTextureFormatInfo(sDDSHeader *header, sTexture *texture);

bool loadDDS(const fs::path &filename, sTexture *texture) {
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

    bool res = loadDDS(data.data(), fsize, texture);
    return res;
}

bool loadDDS(uint8_t *data, size_t dataSize, sTexture *texture) {
    if (dataSize < 4) {
        loggerEx(eLogLevel::ERROR, "Not enough data to read DDS ident. Possibly truncated file\n");
        return false;
    }
    uint32_t ident = *(uint32_t *) data;
    data += 4;
    if (ident != 542327876) { // Not a DDS ident
        return false;
    }
    dataSize -= 4;
    if (dataSize < 124) {
        loggerEx(eLogLevel::ERROR, "Not enough data to read DDS header. Possibly truncated file\n");
        return false;
    }
    sDDSHeader *header = (sDDSHeader *) data;
    data += sizeof(sDDSHeader);
    dataSize -= 124;
    texture->m_width = header->m_width;
    texture->m_height = header->m_height;
    setTextureFormatInfo(header, texture);
    if (texture->m_pixelFormat == ePixelFormat::INVALID) {
        return false;
    }
    int64_t pixelDataSize = calculateTextureSize(texture->m_width, texture->m_height, texture->m_pixelFormat);
    texture->m_rawPixelData.resize(pixelDataSize);
    if (header->m_flags & eDDSPixelFormatFlags::FOURCC &&
        header->m_pixelFormat.m_fourCC == MAKE_FOURCC('D', 'X', '1', '0')) {
        if (dataSize < 20) {
            loggerEx(eLogLevel::ERROR, "Not enough data to DX10 header. Possibly truncated file\n");
            return false;
        }
        data += sizeof(sDX10Header);
        dataSize -= 20;
    }
    if (dataSize < calculateTextureSize(header->m_width, header->m_height, texture->m_pixelFormat)) {
        loggerEx(eLogLevel::ERROR, "Not enough data for pixel data. Possibly truncated file\n");
        return false;
    }
    if ((header->m_pixelFormat.m_flags & eDDSPixelFormatFlags::FOURCC) == 0 &&
        texture->m_pixelFormat >= ePixelFormat::RGBA8888 && texture->m_pixelFormat <= ePixelFormat::RA88) {
        uint32_t rMask = header->m_pixelFormat.m_rBitMask;
        uint32_t gMask = header->m_pixelFormat.m_gBitMask;
        uint32_t bMask = header->m_pixelFormat.m_bBitMask;
        uint32_t aMask = header->m_pixelFormat.m_aBitMask;
        uint8_t redShift = bitShift(rMask);
        uint8_t greenShift = bitShift(gMask);
        uint8_t blueShift = bitShift(bMask);
        uint8_t alphaShift = bitShift(aMask);
        auto *pData = texture->m_rawPixelData.data();
        switch (texture->m_pixelFormat) {
            case ePixelFormat::RGBA8888: {
                for (int i = 0; i < texture->m_rawPixelData.size(); i += 4) {
                    uint32_t pixel = *(uint32_t *) &data[i];
                    pData[i + 0] = (pixel & rMask) >> redShift;
                    pData[i + 1] = (pixel & gMask) >> greenShift;
                    pData[i + 2] = (pixel & bMask) >> blueShift;
                    pData[i + 3] = (pixel & aMask) >> alphaShift;

                }
                break;
            }
            case ePixelFormat::RGB888: {
                for (int i = 0; i < texture->m_rawPixelData.size(); i += 3) {
                    uint32_t pixel = *(uint32_t *) &data[i];
                    pData[i + 0] = (pixel & rMask) >> redShift;
                    pData[i + 1] = (pixel & gMask) >> greenShift;
                    pData[i + 2] = (pixel & bMask) >> blueShift;
                }
                break;
            }
            case ePixelFormat::RG88: {
                for (int i = 0; i < texture->m_rawPixelData.size(); i += 2) {
                    uint32_t pixel = *(uint32_t *) &data[i];
                    pData[i + 0] = (pixel & rMask) >> redShift;
                    pData[i + 1] = (pixel & gMask) >> greenShift;
                }
                break;
            }
            case ePixelFormat::RA88: {
                for (int i = 0; i < texture->m_rawPixelData.size(); i += 2) {
                    uint32_t pixel = *(uint32_t *) &data[i];
                    pData[i + 0] = (pixel & rMask) >> redShift;
                    pData[i + 1] = (pixel & aMask) >> alphaShift;
                }
                break;
            }
        }
    } else {

        memcpy(texture->m_rawPixelData.data(), data, pixelDataSize);
    }
    loggerEx(eLogLevel::INFO, std::format("Loaded DDS {}x{} {}\n", texture->m_width, texture->m_height,
                                          getPixelFormatName(texture->m_pixelFormat)));
    return true;
}


void setTextureFormatInfo(sDDSHeader *header, sTexture *texture) {
    eDDSPixelFormatFlags flags = header->m_pixelFormat.m_flags;
    if (flags & eDDSPixelFormatFlags::FOURCC) {
        if (
                (flags & eDDSPixelFormatFlags::ALPHA) != 0 ||
                (flags & eDDSPixelFormatFlags::ALPHAPIXELS) != 0 ||
                (flags & eDDSPixelFormatFlags::LUMINANCE) != 0 ||
                (flags & eDDSPixelFormatFlags::RGB) != 0 ||
                (flags & eDDSPixelFormatFlags::YUV) != 0
                ) {
            texture->m_pixelFormat = ePixelFormat::INVALID;
            return;
        }
        switch (header->m_pixelFormat.m_fourCC) {
            case MAKE_FOURCC('D', 'X', 'T', '1'): {
                texture->m_pixelFormat = ePixelFormat::BC1;
                return;
            }
            case MAKE_FOURCC('D', 'X', 'T', '2'):
            case MAKE_FOURCC('D', 'X', 'T', '3'): {
                texture->m_pixelFormat = ePixelFormat::BC2;
                return;
            }
            case MAKE_FOURCC('D', 'X', 'T', '4'):
            case MAKE_FOURCC('D', 'X', 'T', '5'): {
                texture->m_pixelFormat = ePixelFormat::BC3;
                return;
            }
            case MAKE_FOURCC('B', 'C', '4', 'U'):
            case MAKE_FOURCC('B', 'C', '4', 'S'):
            case MAKE_FOURCC('A', 'T', 'I', '1'): {
                texture->m_pixelFormat = ePixelFormat::BC4;
                return;
            }
            case MAKE_FOURCC('A', 'T', 'I', '2'):
            case MAKE_FOURCC('B', 'C', '5', 'U'):
            case MAKE_FOURCC('B', 'C', '5', 'S'): {
                texture->m_pixelFormat = ePixelFormat::BC5;
                return;
            }
            case MAKE_FOURCC('D', 'X', '1', '0'): {
                sDX10Header *dx10Header = (sDX10Header *) (((uint8_t *) header) + sizeof(sDDSHeader));
                switch (dx10Header->m_dxgiFormat) {
                    case DX10_BC1_TYPELESS:
                    case DX10_BC1_UNORM:
                    case DX10_BC1_UNORM_SRGB:
                        texture->m_pixelFormat = ePixelFormat::BC1;
                        break;
                    case DX10_BC2_TYPELESS:
                    case DX10_BC2_UNORM:
                    case DX10_BC2_UNORM_SRGB:
                        texture->m_pixelFormat = ePixelFormat::BC2;
                        break;
                    case DX10_BC3_TYPELESS:
                    case DX10_BC3_UNORM:
                    case DX10_BC3_UNORM_SRGB:
                        texture->m_pixelFormat = ePixelFormat::BC3;
                        break;
                    case DX10_BC4_TYPELESS:
                    case DX10_BC4_UNORM:
                    case DX10_BC4_SNORM:
                        texture->m_pixelFormat = ePixelFormat::BC4;
                        break;
                    case DX10_BC5_UNORM:
                    case DX10_BC5_TYPELESS:
                    case DX10_BC5_SNORM:
                        texture->m_pixelFormat = ePixelFormat::BC5;
                        break;
                    case DX10_BC6H_TYPELESS:
                    case DX10_BC6H_UF16:
                    case DX10_BC6H_SF16:
                        texture->m_pixelFormat = ePixelFormat::BC6;
                        break;
                    case DX10_BC7_TYPELESS:
                    case DX10_BC7_UNORM:
                    case DX10_BC7_UNORM_SRGB:
                        texture->m_pixelFormat = ePixelFormat::BC7;
                        break;
                    default:
                        loggerEx(eLogLevel::ERROR,
                                 std::format("Unsupported DXGI format: {}\n", (uint16_t) dx10Header->m_dxgiFormat));
                        texture->m_pixelFormat = ePixelFormat::INVALID;
                        break;
                }
                break;
            }
            default:
                texture->m_pixelFormat = ePixelFormat::INVALID;
        }
    } else if (flags & eDDSPixelFormatFlags::RGB) {
        switch (header->m_pixelFormat.m_rGBBitCount) {
            case 8:
                texture->m_pixelFormat = ePixelFormat::R8;
                break;
            case 16:
                texture->m_pixelFormat = ePixelFormat::RG88;
                break;
            case 24:
                texture->m_pixelFormat = ePixelFormat::RGB888;
                break;
            case 32:
                texture->m_pixelFormat = ePixelFormat::RGBA8888;
                break;
        }
    } else if (flags & eDDSPixelFormatFlags::LUMINANCE) {
        switch (header->m_pixelFormat.m_rGBBitCount) {
            case 8:
                texture->m_pixelFormat = ePixelFormat::R8;
                break;
            case 16:
                if (flags & eDDSPixelFormatFlags::ALPHAPIXELS)
                    texture->m_pixelFormat = ePixelFormat::RA88;
                else
                    texture->m_pixelFormat = ePixelFormat::RG88;
                break;
            default:
                texture->m_pixelFormat = ePixelFormat::INVALID;
                break;
        }
    }
}

int bitShift(uint32_t mask) {
    if (mask == 0) return 0;
    int shift = 0;
    while ((mask & 1) == 0) {
        mask >>= 1;
        ++shift;
    }
    return shift;
}

