#pragma once
#ifndef OZP_BULKFILEREADER
#define OZP_BULKFILEREADER

#include <memory>
#include <string>
#include <fstream>
#include <exception>
#include <limits>

#include <sys\stat.h>

namespace ozp {

namespace detail_bfr {
template <typename T> struct NullChar {};
template <> struct NullChar<char> {
    char operator()() { return '\0'; }
};
template <> struct NullChar<wchar_t> {
    wchar_t operator()() { return L'\0'; }
};

template <typename T> T provide_null() {
    NullChar<T> null_provider;
    return null_provider();
}

__int64 inline get_filesize(const char* filename) {
    struct __stat64 fileStat;
    int err = _stat64(filename, &fileStat);
    if (err != 0)
        throw std::runtime_error("File not found!");
    return fileStat.st_size;
}

__int64 inline get_filesize(const wchar_t* filename) {
    struct __stat64 fileStat;
    int err = _wstat64(filename, &fileStat);
    if (err != 0)
        throw std::runtime_error("File not found!");
    return fileStat.st_size;
}

}

template <typename T>
std::unique_ptr<T[]> bulk_read_file(const T* filename,
                                    bool null_terminated = true) {
    std::unique_ptr<T[]> input;

    auto file_size = ozp::detail_bfr::get_filesize(filename);
    if (file_size * sizeof(T) > std::numeric_limits<size_t>::max())
        throw std::runtime_error("File too big to fit in memory!");

    std::basic_ifstream<T, std::char_traits<T> > file(
        filename, std::ios::in | std::ios::binary | std::ios::ate);

    if (null_terminated) {
        input.reset(new T[static_cast<size_t>(file_size + 1)]);
        file.seekg(0, std::ios::beg);
        file.read(input.get(), file_size);
        input[file_size] = ozp::detail_bfr::provide_null<T>();
    } else {
        input.reset(new T[static_cast<size_t>(file_size)]);
        file.seekg(0, std::ios::beg);
        file.read(input.get(), file_size);
    }

    file.close();
    return input;
}
}
#endif
