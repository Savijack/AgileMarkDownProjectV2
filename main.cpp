#include <iostream>
#include <filesystem>
#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_MAIN   
#include "catch.hpp"
#include "HTMLConverter.h"

using namespace std;

//--
TEST_CASE("Check creating output file")  
{
	HTMLConverter *test = new HTMLConverter("./test_documents/1.md");
	SECTION("Test Document 1")
	{
		test->convert("output.html");
		REQUIRE(filesystem::exists("./output.html"));
	}

	SECTION("")
	{
		REQUIRE(10 == 10);
	}
	delete test;
}
TEST_CASE("convertLine testcases")
{
	HTMLConverter * test = new HTMLConverter("./test_documents/1.md");
    {
        std::string s = "---";
        test->convertLine(s);
        REQUIRE(s == "<hr>");
    }
    {
        std::string s = "   ---   ";
        test->convertLine(s);
        REQUIRE(s == "<hr>");
    }
    {
        std::string s = "text --- text";
        test->convertLine(s);
        REQUIRE(s == "text --- text");
    }
    {
        std::string s = "----";
        test->convertLine(s);
        REQUIRE(s == "----");
    }
    {
        std::string s = "---\n";
        test->convertLine(s);
        REQUIRE(s == "<hr>");
    }
}
