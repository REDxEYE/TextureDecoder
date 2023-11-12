#pragma once

#include <cstdio>
#include <filesystem>

// Function to open a file with a Unicode path
FILE *openFile(const std::filesystem::path &path, const char *mode);

void fileWriteFunc(void *context, void *data, int size);

void memoryWriteFunc(void* context, void* data, int size);