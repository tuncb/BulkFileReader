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
template <typename T> struct NullChar {
    static_assert<false>,
        "BulkFileReader: only char and wchar_t are accepted types."
};
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

int get_filestat(const char* filename, struct __stat64& stat) {
    return _stat64(filename, &stat);
}

int get_filestat(const wchar_t* filename, struct __stat64& stat) {
    return _wstat64(filename, &stat);
}

template <typename T> __int64 inline get_filesize(const T* filename) {
    struct __stat64 stat;
    int err = get_filestat(filename, stat);
    if (err != 0)
        throw std::runtime_error("File not found!");
    return stat.st_size;
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

    if (!file.is_open())
        throw std::runtime_error("Could not open file!");

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

    return input;
}
}
#endif
