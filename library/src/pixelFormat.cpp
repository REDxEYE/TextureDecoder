#include "pixelFormat.h"

const char *getPixelFormatName(ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        case INVALID:
            return "INVALID";
        case RGBA32:
            return "RGBA32";
        case RGB32:
            return "RGB32";
        case RG32:
            return "RG32";
        case R32:
            return "R32";
        case RGBA16:
            return "RGBA16";
        case RGB16:
            return "RGB16";
        case RG16:
            return "RG16";
        case R16:
            return "R16";
        case RGBA32F:
            return "RGBA32F";
        case RGB32F:
            return "RGB32F";
        case RG32F:
            return "RG32F";
        case R32F:
            return "R32F";
        case RGBA16F:
            return "RGBA16F";
        case RGB16F:
            return "RGB16F";
        case RG16F:
            return "RG16F";
        case R16F:
            return "R16F";
        case RGBA8888:
            return "RGBA8888";
        case BGRA8888:
            return "BGRA8888";
        case RGB888:
            return "RGB888";
        case RG88:
            return "RG88";
        case RA88:
            return "RA88";
        case R8:
            return "R8";
        case RGB565:
            return "RGB565";
        case RGBA5551:
            return "RGBA5551";
        case RGBA1010102:
            return "RGBA1010102";
        case BC1:
            return "BC1";
        case BC1a:
            return "BC1a";
        case BC2:
            return "BC2";
        case BC3:
            return "BC3";
        case BC4:
            return "BC4";
        case BC5:
            return "BC5";
        case BC6:
            return "BC6";
        case BC7:
            return "BC7";
    }
}

int getChannelCount(ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        case RGBA32:
        case RGBA32F:
        case RGBA8888:
        case BGRA8888:
        case RGBA16:
        case RGBA16F:
        case RGBA5551:
        case RGBA1010102:
        case BC1a:
        case BC2:
        case BC3:
        case BC7:
            return 4;

        case RGB32:
        case RGB32F:
        case RGB888:
        case RGB16:
        case RGB16F:
        case RGB565:
        case BC1:
        case BC6:
            return 3;

        case RG32:
        case RG32F:
        case RG88:
        case RG16:
        case RG16F:
        case RA88:
        case BC4:
        case BC5:
            return 2;

        case R32:
        case R32F:
        case R8:
        case R16:
        case R16F:
            return 1;

        case INVALID:
        default:
            return 0; // Invalid or unknown format
    }
}

bool isCompressedPixelFormat(ePixelFormat pixelFormat) {
    if (pixelFormat >= ePixelFormat::BC1)
        return true;
    return false;
}

ePixelFormat getUncompressedPixelFormatVariant(ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        default:
            return pixelFormat;
        case BC1:
        case BC1a:
        case BC2:
        case BC3:
        case BC7:
            return RGBA8888;
        case BC4:
            return R8;
        case BC5:
            return RG88;
        case BC6:
            return RGB16F;
    }
}

int getPixelFormatPixelSize(ePixelFormat pixelFormat) {
    switch (pixelFormat) {
        default:
        case INVALID:
            return 0;
        case RGBA32:
            return 16;
        case RGB32:
            return 12;
        case RG32:
            return 8;
        case R32:
            return 4;
        case RGBA16:
            return 8;
        case RGB16:
            return 6;
        case RG16:
            return 4;
        case R16:
            return 2;
        case RGBA32F:
            return 16;
        case RGB32F:
            return 12;
        case RG32F:
            return 8;
        case R32F:
            return 4;
        case RGBA16F:
            return 8;
        case RGB16F:
            return 6;
        case RG16F:
            return 4;
        case R16F:
            return 2;
        case RGBA8888:
            return 4;
        case BGRA8888:
            return 4;
        case RGB888:
            return 3;
        case RG88:
            return 2;
        case RA88:
            return 2;
        case R8:
            return 1;
        case RGB565:
            return 2;
        case RGBA5551:
            return 2;
        case RGBA1010102:
            return 4;
    }
}
