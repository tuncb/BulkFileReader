cmake_minimum_required(VERSION 3.1.2)
project(tester CXX)

include(../../conan.cmake)
conan_cmake_run(BASIC_SETUP CMAKE_TARGETS CONAN_CMAKE_MULTI
  REQUIRES
  "boost/1.66.0@conan/stable"
)

set(SOURCE 
  ./source/main.cpp
)

set(BULKFILEREADER_LIBRARY 
  ../bulkfilereader/include/bulkfilereader/bulkfilereader.h
)

configure_file(./source/char_text.txt char_text.txt COPYONLY)

add_executable(bulkfilereader_tester ${SOURCE} ${BULKFILEREADER_LIBRARY})
target_include_directories(bulkfilereader_tester PRIVATE "../bulkfilereader/include/")
conan_target_link_libraries(bulkfilereader_tester)

source_group("bulkfilereader" FILES ${BULKFILEREADER_LIBRARY})