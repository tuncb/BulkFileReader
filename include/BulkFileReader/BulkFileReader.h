#pragma once
#ifndef OZP_BULKFILEREADER
#define OZP_BULKFILEREADER

#include <memory>
#include <string>
#include <fstream>
#include <exception>

#include <sys\stat.h>

namespace ozp {

namespace detail_bfr {

template <typename T> struct NullChar {};
template <> struct NullChar<char> {
    inline char operator()() { return '\0'; }
};
template <> struct NullChar<wchar_t> {
    inline wchar_t operator()() { return L'\0'; }
};

template <typename T> inline T provide_null() {
    NullChar<T> null_provider;
    return null_provider();
}

inline int get_filestat(const char* filename, struct __stat64& stat) {
    return _stat64(filename, &stat);
}

inline int get_filestat(const wchar_t* filename, struct __stat64& stat) {
    return _wstat64(filename, &stat);
}

template <typename T> inline __int64 get_filesize(const T* filename) {
    struct __stat64 stat;
    int err = get_filestat(filename, stat);
    if (err != 0)
        throw std::runtime_error("File not found!");
    return stat.st_size;
}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>   Bulk reads text files into char or wchar_t arrays. </summary>
///
/// <exception cref="std::runtime_error">   Thrown when file is not found, or cannot be opened, etc... </exception>
///
/// <typeparam name="typename T"> char or wchar_t </typeparam>
/// <param name="filename"> Filename of the file. </param>
///
/// <returns>   Array of null terminated characters; </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline std::unique_ptr<T[]> bulk_read_file(const T* filename) {
    std::unique_ptr<T[]> input;

    auto file_size = ozp::detail_bfr::get_filesize(filename);
    std::basic_ifstream<T, std::char_traits<T> > file(
        filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open())
        throw std::runtime_error("Could not open file!");

    input.reset(new T[static_cast<size_t>(file_size + 1)]);
    file.seekg(0, std::ios::beg);
    file.read(input.get(), file_size);
    input[static_cast<size_t>(file_size)] = ozp::detail_bfr::provide_null<T>();

    return input;
}
}
#endif
