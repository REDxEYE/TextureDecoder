#include <cstdio>
#include "dds/loadDDS.h"

bool loadDDS(const char *filename, sTexture *texture) {
    FILE *file;
#ifdef WIN32
    if (fopen_s(&file, filename, "rb") != 0) {
        return false;
    }
#else
    file = fopen(filename, "rb");
    if (file == nullptr) {
        return false;
    }
#endif
    fseek(file, 0, 2);
    size_t fsize = ftell(file);
    fseek(file, 0, 0);
    uint8_t *data = new uint8_t[fsize];
    if (fread(data, 1, fsize, file) != fsize) {
        delete[] data;
        return false;
    }
    bool res = loadDDS(data, fsize, texture);
    delete[] data;
    return res;
}


ePixelFormat texturePixelFormat(sDDSHeader *header) {
    if (header->m_pixelFormat.m_flags & eDDSPixelFormatFlags::FOURCC) {
        switch (header->m_pixelFormat.m_fourCC) {
            case 'DXT1': {
                return ePixelFormat::BC1;
            }
            case 'DXT2':
            case 'DXT3': {
                return ePixelFormat::BC2;
            }
            case 'DXT4':
            case 'DXT5': {

                return ePixelFormat::BC3;
            }
            case 'DX10': {
                throw std::exception("Unsupported");
                sDX10Header *dx10Header = (sDX10Header *) (((uint8_t *) header) + sizeof(sDDSHeader));
                /*switch (dx10Header->m_dxgiFormat) {

                    case DX10_UNKNOWN:
                        break;
                    case DX10_R32G32B32A32_TYPELESS:
                        break;
                    case DX10_R32G32B32A32_FLOAT:
                        break;
                    case DX10_R32G32B32A32_UINT:
                        break;
                    case DX10_R32G32B32A32_SINT:
                        break;
                    case DX10_R32G32B32_TYPELESS:
                        break;
                    case DX10_R32G32B32_FLOAT:
                        break;
                    case DX10_R32G32B32_UINT:
                        break;
                    case DX10_R32G32B32_SINT:
                        break;
                    case DX10_R16G16B16A16_TYPELESS:
                        break;
                    case DX10_R16G16B16A16_FLOAT:
                        break;
                    case DX10_R16G16B16A16_UNORM:
                        break;
                    case DX10_R16G16B16A16_UINT:
                        break;
                    case DX10_R16G16B16A16_SNORM:
                        break;
                    case DX10_R16G16B16A16_SINT:
                        break;
                    case DX10_R32G32_TYPELESS:
                        break;
                    case DX10_R32G32_FLOAT:
                        break;
                    case DX10_R32G32_UINT:
                        break;
                    case DX10_R32G32_SINT:
                        break;
                    case DX10_R32G8X24_TYPELESS:
                        break;
                    case DX10_D32_FLOAT_S8X24_UINT:
                        break;
                    case DX10_R32_FLOAT_X8X24_TYPELESS:
                        break;
                    case DX10_X32_TYPELESS_G8X24_UINT:
                        break;
                    case DX10_R10G10B10A2_TYPELESS:
                        break;
                    case DX10_R10G10B10A2_UNORM:
                        break;
                    case DX10_R10G10B10A2_UINT:
                        break;
                    case DX10_R11G11B10_FLOAT:
                        break;
                    case DX10_R8G8B8A8_TYPELESS:
                        break;
                    case DX10_R8G8B8A8_UNORM:
                        break;
                    case DX10_R8G8B8A8_UNORM_SRGB:
                        break;
                    case DX10_R8G8B8A8_UINT:
                        break;
                    case DX10_R8G8B8A8_SNORM:
                        break;
                    case DX10_R8G8B8A8_SINT:
                        break;
                    case DX10_R16G16_TYPELESS:
                        break;
                    case DX10_R16G16_FLOAT:
                        break;
                    case DX10_R16G16_UNORM:
                        break;
                    case DX10_R16G16_UINT:
                        break;
                    case DX10_R16G16_SNORM:
                        break;
                    case DX10_R16G16_SINT:
                        break;
                    case DX10_R32_TYPELESS:
                        break;
                    case DX10_D32_FLOAT:
                        break;
                    case DX10_R32_FLOAT:
                        break;
                    case DX10_R32_UINT:
                        break;
                    case DX10_R32_SINT:
                        break;
                    case DX10_R24G8_TYPELESS:
                        break;
                    case DX10_D24_UNORM_S8_UINT:
                        break;
                    case DX10_R24_UNORM_X8_TYPELESS:
                        break;
                    case DX10_X24_TYPELESS_G8_UINT:
                        break;
                    case DX10_R8G8_TYPELESS:
                        break;
                    case DX10_R8G8_UNORM:
                        break;
                    case DX10_R8G8_UINT:
                        break;
                    case DX10_R8G8_SNORM:
                        break;
                    case DX10_R8G8_SINT:
                        break;
                    case DX10_R16_TYPELESS:
                        break;
                    case DX10_R16_FLOAT:
                        break;
                    case DX10_D16_UNORM:
                        break;
                    case DX10_R16_UNORM:
                        break;
                    case DX10_R16_UINT:
                        break;
                    case DX10_R16_SNORM:
                        break;
                    case DX10_R16_SINT:
                        break;
                    case DX10_R8_TYPELESS:
                        break;
                    case DX10_R8_UNORM:
                        break;
                    case DX10_R8_UINT:
                        break;
                    case DX10_R8_SNORM:
                        break;
                    case DX10_R8_SINT:
                        break;
                    case DX10_A8_UNORM:
                        break;
                    case DX10_R1_UNORM:
                        break;
                    case DX10_R9G9B9E5_SHAREDEXP:
                        break;
                    case DX10_R8G8_B8G8_UNORM:
                        break;
                    case DX10_G8R8_G8B8_UNORM:
                        break;
                    case DX10_BC1_TYPELESS:
                        break;
                    case DX10_BC1_UNORM:
                        break;
                    case DX10_BC1_UNORM_SRGB:
                        break;
                    case DX10_BC2_TYPELESS:
                        break;
                    case DX10_BC2_UNORM:
                        break;
                    case DX10_BC2_UNORM_SRGB:
                        break;
                    case DX10_BC3_TYPELESS:
                        break;
                    case DX10_BC3_UNORM:
                        break;
                    case DX10_BC3_UNORM_SRGB:
                        break;
                    case DX10_BC4_TYPELESS:
                        break;
                    case DX10_BC4_UNORM:
                        break;
                    case DX10_BC4_SNORM:
                        break;
                    case DX10_BC5_TYPELESS:
                        break;
                    case DX10_BC5_UNORM:
                        break;
                    case DX10_BC5_SNORM:
                        break;
                    case DX10_B5G6R5_UNORM:
                        break;
                    case DX10_B5G5R5A1_UNORM:
                        break;
                    case DX10_B8G8R8A8_UNORM:
                        break;
                    case DX10_B8G8R8X8_UNORM:
                        break;
                    case DX10_R10G10B10_XR_BIAS_A2_UNORM:
                        break;
                    case DX10_B8G8R8A8_TYPELESS:
                        break;
                    case DX10_B8G8R8A8_UNORM_SRGB:
                        break;
                    case DX10_B8G8R8X8_TYPELESS:
                        break;
                    case DX10_B8G8R8X8_UNORM_SRGB:
                        break;
                    case DX10_BC6H_TYPELESS:
                        break;
                    case DX10_BC6H_UF16:
                        break;
                    case DX10_BC6H_SF16:
                        break;
                    case DX10_BC7_TYPELESS:
                        break;
                    case DX10_BC7_UNORM:
                        break;
                    case DX10_BC7_UNORM_SRGB:
                        break;
                    case DX10_AYUV:
                        break;
                    case DX10_Y410:
                        break;
                    case DX10_Y416:
                        break;
                    case DX10_NV12:
                        break;
                    case DX10_P010:
                        break;
                    case DX10_P016:
                        break;
                    case DX10_420_OPAQUE:
                        break;
                    case DX10_YUY2:
                        break;
                    case DX10_Y210:
                        break;
                    case DX10_Y216:
                        break;
                    case DX10_NV11:
                        break;
                    case DX10_AI44:
                        break;
                    case DX10_IA44:
                        break;
                    case DX10_P8:
                        break;
                    case DX10_A8P8:
                        break;
                    case DX10_B4G4R4A4_UNORM:
                        break;
                    case DX10_P208:
                        break;
                    case DX10_V208:
                        break;
                    case DX10_V408:
                        break;
                    case DX10_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
                        break;
                    case DX10_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
                        break;
                }*/
            }
            default:
                return ePixelFormat::INVALID;
        }
    } else {
        switch (header->m_pixelFormat.m_rGBBitCount) {
            case 8:
                return ePixelFormat::R8;
            case 16:
                return ePixelFormat::RG88;
            case 24:
                return ePixelFormat::RGB888;
            case 32:
                return ePixelFormat::RGBA8888;
        }
    }
}


bool loadDDS(uint8_t *data, size_t dataSize, sTexture *texture) {
    uint32_t ident = *(uint32_t *) data;
    data += 4;
    if (ident != 542327876) { // Not a DDS ident
        return false;
    }
    sDDSHeader *header = (sDDSHeader *) data;
        data += sizeof(sDDSHeader);
    texture->m_width = header->m_width;
    texture->m_height = header->m_height;
    texture->m_pixelFormat = texturePixelFormat(header);
    if(texture->m_pixelFormat==ePixelFormat::INVALID){
        return false;
    }
    int64_t pixelDataSize = calculateTextureSize(texture->m_width, texture->m_height, texture->m_pixelFormat);
    texture->m_rawPixelData.resize(pixelDataSize);
    if (header->m_flags & eDDSPixelFormatFlags::FOURCC && header->m_pixelFormat.m_fourCC == 'DX10') {
        data += sizeof(sDX10Header);
    }
    memcpy(texture->m_rawPixelData.data(), data, pixelDataSize);
    return true;
}
