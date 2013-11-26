BulkFileReader
==============

C++ library for reading text files into null terminated character arrays.
* Supports both char and wchar_t
* Reads files in a single go.
* Throws std::runtime_error in case of errors.

Installation
-----------
Header only -> 
~~~~cpp
#include <BulkFileReader/BulkFileReader.h>
~~~~

Function definition
----------
~~~~cpp
namespace ozp {
  template <typename T> std::unique_ptr<T[]> bulk_read_file(const T* filename)
}
~~~~

Example
----
~~~~cpp
auto text = ozp::bulk_read_file(L"wchar_text.txt");
~~~~
