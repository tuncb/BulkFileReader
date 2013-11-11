#pragma once
#ifndef OZP_BULKFILEREADER
#define OZP_BULKFILEREADER

#include <memory>
#include <string>
#include <fstream>

#include <sys\stat.h>

namespace ozp {

namespace detail {
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
}

template <typename T>
std::unique_ptr<T[]> bulk_read_file(const T* filename,
                                    bool does_end_with_null = true) {
    std::unique_ptr<T[]> input;

    struct __stat64 fileStat;
    int err = _stat64(filename, &fileStat);
    __int64 file_size = fileStat.st_size;
    std::basic_ifstream<T, std::char_traits<T> > file(
        filename, std::ios::in | std::ios::binary | std::ios::ate);

    if (does_end_with_null) {
        input.reset(new T[static_cast<size_t>(file_size)]);
        file.seekg(0, std::ios::beg);
        file.read(input.get(), file_size);
        input[file_size] = ozp::detail::provide_null<T>();
    } else {
        input.reset(new T[static_cast<size_t>(file_size + 1)]);
        file.seekg(0, std::ios::beg);
        file.read(input.get(), file_size);
    }

    file.close();
    return input;
}
}
#endif
