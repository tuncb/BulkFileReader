#pragma once
#ifndef OZP_BULKFILEREADER
#define OZP_BULKFILEREADER

#include <memory>
#include <string>
#include <fstream>

#include <sys\stat.h>

namespace ozp {
    std::unique_ptr<char[]> bulk_read_file(const std::string &filename) {
        std::unique_ptr<char[]> input;
        struct __stat64 fileStat;
        int err = _stat64(filename.c_str(), &fileStat);
        __int64 file_size = fileStat.st_size;
        std::ifstream file(filename.c_str(),
            std::ios::in | std::ios::binary | std::ios::ate);
        input.reset(new char[static_cast<size_t>(file_size) + 1]);
        file.seekg(0, std::ios::beg);
        file.read(input.get(), file_size);
        input[file_size] = '\0';
        file.close();
        return input;
    }
}

#endif
