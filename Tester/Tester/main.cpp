#define BOOST_TEST_MODULE BulkFileReader
#include <boost/test/unit_test.hpp>

#include <BulkFileReader/BulkFileReader.h>

BOOST_AUTO_TEST_CASE(char_read) {
    auto text = ozp::bulk_read_file("char_text.txt");
    std::string str(text.get());
    BOOST_CHECK_EQUAL(str.size(), 1334);
}

BOOST_AUTO_TEST_CASE(wchar_read) {
    auto text = ozp::bulk_read_file(L"char_text.txt");
    std::wstring str(text.get());
    BOOST_CHECK_EQUAL(str.size(), 1334);
}