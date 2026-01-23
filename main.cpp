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
TEST_CASE("convertLine testcases")
{
	HTMLConverter * test = new HTMLConverter("./test_documents/1.md");
    {
        string s = "---";
        test->convertLine(s);
        REQUIRE(s == "<hr>");
    }
    {
        string s = "   ---   ";
        test->convertLine(s);
        REQUIRE(s == "<hr>");
    }
    {
        string s = "text --- text";
        test->convertLine(s);
        REQUIRE(s == "text --- text");
    }
    {
        string s = "----";
        test->convertLine(s);
        REQUIRE(s == "----");
    }
    {
        string s = "---\n";
        test->convertLine(s);
        REQUIRE(s == "<hr>");
    }
    delete test;
}
TEST_CASE("convertHeaders: single h1 header")
{
    HTMLConverter test("./test_documents/1.md");

    string text = "# Title\n";
    test.convertHeaders(text);

    REQUIRE(text == "<h1>Title</h1>\n");
}

TEST_CASE("convertHeaders: multiple header levels")
{
    HTMLConverter test("./test_documents/1.md");

    string text =
        "# One\n"
        "## Two\n"
        "### Three\n";

    test.convertHeaders(text);

    REQUIRE(text ==
        "<h1>One</h1>\n"
        "<h2>Two</h2>\n"
        "<h3>Three</h3>\n"
    );
}

TEST_CASE("convertHeaders: no headers")
{
    HTMLConverter test("./test_documents/1.md");

    string text =
        "Paragraph\n"
        "Not a header\n";

    test.convertHeaders(text);

    REQUIRE(text ==
        "Paragraph\n"
        "Not a header\n"
    );
}

TEST_CASE("convertHeaders: headers mixed with text")
{
    HTMLConverter test("./test_documents/1.md");

    string text =
        "# Header\n"
        "Text\n"
        "## Subheader\n";

    test.convertHeaders(text);

    REQUIRE(text ==
        "<h1>Header</h1>\n"
        "Text\n"
        "<h2>Subheader</h2>\n"
    );
}

TEST_CASE("convertHeaders: header followed by blank line")
{
    HTMLConverter test("./test_documents/1.md");

    string text =
        "# Header\n"
        "\n"
        "Text\n";

    test.convertHeaders(text);

    REQUIRE(text ==
        "<h1>Header</h1>\n"
        "\n"
        "Text\n"
    );
}
