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
	HTMLConverter *test = new HTMLConverter("./test_documents/2.md");
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

TEST_CASE("separateCodeBlocks: single code block") {
    HTMLConverter test("./test_documents/1.md");
    string s =
        "Here is some text\n"
        "```int x = 5;```\n"
        "More text";

    test.separateCodeBlocks(s);

    REQUIRE(test.codeblocks.size() == 1);
    REQUIRE(test.codeblocks[0] == "```int x = 5;```");
    REQUIRE(s == "Here is some text\n<{codeblock:0}>\nMore text");
}
TEST_CASE("separateCodeBlocks: multiple code blocks") {
    HTMLConverter test("./test_documents/1.md");
    string s =
        "Start\n"
        "```code A```\n"
        "Middle\n"
        "```code B```\n"
        "End";

    test.separateCodeBlocks(s);

    REQUIRE(test.codeblocks.size() == 2);
    REQUIRE(test.codeblocks[0] == "```code A```");
    REQUIRE(test.codeblocks[1] == "```code B```");

    REQUIRE(s ==
        "Start\n"
        "<{codeblock:0}>\n"
        "Middle\n"
        "<{codeblock:1}>\n"
        "End");
}
TEST_CASE("separateCodeBlocks: no code blocks") {
    HTMLConverter test("./test_documents/1.md");
    string original = "Just plain text\nNo code here";
    string s = original;

    test.separateCodeBlocks(s);

    REQUIRE(test.codeblocks.empty());
    REQUIRE(s == original);
}

TEST_CASE("convert italic function")
{
	HTMLConverter *test = new HTMLConverter("./test_documents/2.md");
	SECTION("Test single italic")
	{
		string s = "*hello*";
		test->convertItalics(s);
		REQUIRE(s == "<em>hello</em>");
	}
	SECTION("Test multiple italics")
	{
		string s = "*one* and *two*";
		test->convertItalics(s);
		REQUIRE(s == "<em>one</em> and <em>two</em>");
	}
	delete test; 
}

TEST_CASE("convert image function")
{
    HTMLConverter *test = new HTMLConverter("./test_documents/2.md");
	SECTION("simple image")
    {
        string s = "![bed](bed.png)";
        test->convertImages(s); 
        REQUIRE(s == "<img src=\"bed.png\" alt=\"bed\">");
    }
    SECTION("image in text")
    {
        string s = "one, two, ![image](image.jpg) three";
        test->convertImages(s); 
        REQUIRE(s == "one, two, <img src=\"image.jpg\" alt=\"image\"> three");
    }
    SECTION("two images")
    {
        string s = "![image1](image1.jpg) and ![image2](image2.jpg)";
        test->convertImages(s); 
        REQUIRE(s == "<img src=\"image1.jpg\" alt=\"image1\"> and <img src=\"image2.jpg\" alt=\"image2\">");
    }
	delete test; 
}
// TEST_CASE("processCodeblock: file header -> figure+caption") {
//     HTMLConverter c("./test_documents/1.md");

//     string cb =
//         "``` file=\"main.cpp\"\n"
//         "int x = 10;\n"
//         "return x;\n"
//         "```";

//     c.processCodeblock(cb);

//     REQUIRE(cb.find("<figure class=\"codeblock\">") != string::npos);
//     REQUIRE(cb.find("<figcaption class=\"codeblock__title\">main.cpp</figcaption>") != string::npos);
//     REQUIRE(cb.find("  <pre><code>\n") != string::npos);

//     REQUIRE(cb.find("int x = 10;\nreturn x;\n") != string::npos);

//     REQUIRE(cb.find("</figure>") != string::npos);
//     REQUIRE(cb.find("```") == string::npos);
// }