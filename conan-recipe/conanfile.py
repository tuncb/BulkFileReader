from conans import ConanFile

class BulkFileReaderConan(ConanFile):
    name = "bulkfilereader"
    version = "1.0.0"
    license = "https://www.apache.org/licenses/LICENSE-2.0"
    url = "https://github.com/tuncb/bulkfilereader"
    description = ("C++ library for reading text files into null terminated character arrays.")
    
    def package(self):
        self.copy("*", dst="bulkfilereader", src="../projects/bulkfilereader/include/bulkfilereader")
    def package_info(self):
        self.cpp_info.includedirs = ['.']
