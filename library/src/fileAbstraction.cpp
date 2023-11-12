#include "fileAbstraction.h"


FILE *openFile(const std::filesystem::path &path, const char *mode) {
    FILE *file = nullptr;
#if defined(_WIN32)
    // On Windows, use _wfopen for Unicode paths
    errno_t err = _wfopen_s(&file, path.wstring().c_str(), std::wstring(mode, mode + strlen(mode)).c_str());
    if (err) {
        return nullptr;
    }
#else
    // On Linux, use fopen with UTF-8 paths
    file = fopen(path.string().c_str(), mode);
#endif
    return file;
}

void fileWriteFunc(void *context, void *data, int size) {
    FILE *file = static_cast<FILE *>(context);
    fwrite(data, 1, size, file);
}

void memoryWriteFunc(void *context, void *data, int size) {
    std::vector<uint8_t>* buffer = static_cast<std::vector<uint8_t>*>(context);
    uint8_t* byteData = static_cast<uint8_t*>(data);

    // Append the data to the buffer
    buffer->insert(buffer->end(), byteData, byteData + size);
}
