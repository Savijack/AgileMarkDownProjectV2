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

TEST_CASE("Bold Conversion")
{
	HTMLConverter *test = new HTMLConverter("./test_documents/1.md");
	
	SECTION("Single Bold Word")
	{
		string input = "Hello **World**!";
		test->convertBold(input);
		REQUIRE(input == "Hello <b>World</b>!");
	}

	SECTION("Multiple Bold Words")
	{
		string input = "Hello **World I am** here!";
		test->convertBold(input);
		REQUIRE(input == "Hello <b>World I am</b> here!");
	}

	SECTION("Separated Bold Words")
	{
		string input = "Hello **World** I am **here!**";
		test->convertBold(input);
		REQUIRE(input == "Hello <b>World</b> I am <b>here!</b>");
	}

	SECTION("Touching Bold Words")
	{
		string input = "Hello **World** **I** am **here!**";
		test->convertBold(input);
		REQUIRE(input == "Hello <b>World</b> <b>I</b> am <b>here!</b>");
	}
	delete test;
}