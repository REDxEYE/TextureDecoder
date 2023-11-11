#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
#include "textureDecoder.h"
#include "dds/loadDDS.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }
    char *inputName = argv[1];
    sTexture texture;
    if (!loadDDS(inputName, &texture)) {
        return -1;
    }
    fs::path outputName(inputName);
    outputName.replace_extension(".hdr");

    sTexture cTexture;
    cTexture.m_height = texture.m_height;
    cTexture.m_width = texture.m_width;
    cTexture.m_pixelFormat = ePixelFormat::RGB32F;

    if (convertTexture(&texture, &cTexture))
        stbi_write_hdr(outputName.string().c_str(), (int)texture.m_width, (int)texture.m_height, 3,
                       (float*)cTexture.m_rawPixelData.data());
    else
        return -1;
}