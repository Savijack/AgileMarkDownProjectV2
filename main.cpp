#include <filesystem>
#include <iostream>
#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "HTMLConverter.h"

using namespace std;

int main(int argc, char *argv[]) {
    Catch::Session session;
    session.run(argc, argv);

    string inputPath;
    string outputPath;

    cout << "Enter input markdown filepath: ";
    if (!getline(cin, inputPath)) {
        return 1;
    }

    if (!filesystem::exists(inputPath)) {
        cerr << "Input file does not exist: " << inputPath << "\n";
        return 1;
    }

    cout << "Enter output html filepath: ";
    if (!getline(cin, outputPath)) {
        return 1;
    }

    HTMLConverter converter(inputPath);
    converter.convert(outputPath);
    cout << "Converted to " << outputPath << "\n";
    return 0;
}

//--
TEST_CASE("Check creating output file")  
{
	HTMLConverter *test = new HTMLConverter("./test_documents/4.md");
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

TEST_CASE("Bold and Italic Conversion")
{
	HTMLConverter *test = new HTMLConverter("./test_documents/1.md");
	
	SECTION("Single Bold Word")
	{
		string s = "Hello **World**!";
		test->convertBoldAndItalics(s);
		REQUIRE(s == "Hello <b>World</b>!");
	}
	SECTION("Separated Bold Words")
	{
		string s = "Hello **World** I am **here!**";
		test->convertBoldAndItalics(s);
		REQUIRE(s == "Hello <b>World</b> I am <b>here!</b>");
	}
	SECTION("single italic")
	{
		string s = "*hello*";
		test->convertBoldAndItalics(s);
		REQUIRE(s == "<em>hello</em>");
	}
	SECTION("separated italics")
	{
		string s = "*one* and *two*";
		test->convertBoldAndItalics(s);
		REQUIRE(s == "<em>one</em> and <em>two</em>");
	}
    SECTION("single bold and single italic")
    {
        string s = "*one* and **two**";
        test->convertBoldAndItalics(s);
        REQUIRE(s == "<em>one</em> and <b>two</b>");
    }
    SECTION("combined bold and italic word")
    {
        string s = "Oh ***hello*** there";
        test->convertBoldAndItalics(s);
        REQUIRE(s == ("Oh <b><em>hello</em></b> there"));
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

TEST_CASE("convert image and links function")
{
    HTMLConverter *test = new HTMLConverter("./test_documents/2.md");
	SECTION("simple image")
    {
        string s = "![bed](bed.png)";
        test->convertImagesAndLinks(s); 
        REQUIRE(s == "<img src=\"bed.png\" alt=\"bed\">");
    }
    SECTION("image in text")
    {
        string s = "one, two, ![image](image.jpg) three";
        test->convertImagesAndLinks(s); 
        REQUIRE(s == "one, two, <img src=\"image.jpg\" alt=\"image\"> three");
    }
    SECTION("two images")
    {
        string s = "![image1](image1.jpg) and ![image2](image2.jpg)";
        test->convertImagesAndLinks(s); 
        REQUIRE(s == "<img src=\"image1.jpg\" alt=\"image1\"> and <img src=\"image2.jpg\" alt=\"image2\">");
    }
    SECTION("simple link")
    {
        string s = "[link](link.com)";
        test->convertImagesAndLinks(s); 
        REQUIRE(s == "<a href=\"link.com\">link</a>");
    }
    SECTION("image in text")
    {
        string s = "one, two, [link](link.com) three";
        test->convertImagesAndLinks(s); 
        REQUIRE(s == "one, two, <a href=\"link.com\">link</a> three");
    }
    SECTION("one link and photo")
    {
        string s = "[link1](link1.com) and ![pic1](pic1.png)";
        test->convertImagesAndLinks(s); 
        REQUIRE(s == "<a href=\"link1.com\">link1</a> and <img src=\"pic1.png\" alt=\"pic1\">");
    }
	delete test; 
}

TEST_CASE("convert list function")
{
	HTMLConverter *test = new HTMLConverter("./test_documents/1.md");
	
	SECTION("Simple unordered list")
	{
		string s = "- Item 1\n- Item 2\n- Item 3\n";
		test->convertLists(s);
		REQUIRE(s == "<ul>\n  <li>Item 1</li>\n  <li>Item 2</li>\n  <li>Item 3</li>\n</ul>\n");
	}

	SECTION("Simple ordered list")
	{
		string s = "1. First\n2. Second\n3. Third\n";
		test->convertLists(s);
		REQUIRE(s == "<ol>\n  <li>First</li>\n  <li>Second</li>\n  <li>Third</li>\n</ol>\n");
	}

	SECTION("nested lists")
	{
		string s = "- Item 1\n  - Nested 1\n  - Nested 2\n- Item 2\n";
		test->convertLists(s);
		REQUIRE(s == "<ul>\n  <li>Item 1</li>\n  <ul>\n    <li>Nested 1</li>\n    <li>Nested 2</li>\n  </ul>\n  <li>Item 2</li>\n</ul>\n");
	}

	SECTION("list and text mixed")
	{
		string s = "1. Item 1\n2. Item 2\nRegular paragraph\n";
		test->convertLists(s);
		REQUIRE(s == "<ol>\n  <li>Item 1</li>\n  <li>Item 2</li>\n</ol>\nRegular paragraph\n");
	}

	SECTION("list and text mixed 2")
	{
		string s = "Some text\n- Item 1\n- Item 2\n";
		test->convertLists(s);
		REQUIRE(s == "Some text\n<ul>\n  <li>Item 1</li>\n  <li>Item 2</li>\n</ul>\n");
	}

	SECTION("Multiple separate lists")
	{
		string s = "- List 1 Item\nParagraph\n1. List 2 Item\n";
		test->convertLists(s);
		REQUIRE(s == "<ul>\n  <li>List 1 Item</li>\n</ul>\nParagraph\n<ol>\n  <li>List 2 Item</li>\n</ol>\n");
	}

	delete test;
}

TEST_CASE("convert table function")
{
	HTMLConverter *test = new HTMLConverter("./test_documents/4.md");
	
	SECTION("Table with three columns")
	{
		string s = ":::table cols=[Name][Age][City]\n[Alice][30][NYC]\n[Bob][25][LA]\n:::\n";
		test->convertTables(s);
		REQUIRE(s == "<table>\n  <thead>\n    <tr>\n      <th>Name</th>\n      <th>Age</th>\n      <th>City</th>\n    </tr>\n  </thead>\n  <tbody>\n    <tr>\n      <td>Alice</td>\n      <td>30</td>\n      <td>NYC</td>\n    </tr>\n    <tr>\n      <td>Bob</td>\n      <td>25</td>\n      <td>LA</td>\n    </tr>\n  </tbody>\n</table>\n");
	}

	SECTION("Table with two columns")
	{
		string s = ":::table cols=[metric][value]\n[build time][12s]\n[test time][48s]\n:::\n";
		test->convertTables(s);
		REQUIRE(s == "<table>\n  <thead>\n    <tr>\n      <th>metric</th>\n      <th>value</th>\n    </tr>\n  </thead>\n  <tbody>\n    <tr>\n      <td>build time</td>\n      <td>12s</td>\n    </tr>\n    <tr>\n      <td>test time</td>\n      <td>48s</td>\n    </tr>\n  </tbody>\n</table>\n");
	}

	SECTION("Table with single row")
	{
		string s = ":::table cols=[Header1][Header2]\n[Data1][Data2]\n:::\n";
		test->convertTables(s);
		REQUIRE(s == "<table>\n  <thead>\n    <tr>\n      <th>Header1</th>\n      <th>Header2</th>\n    </tr>\n  </thead>\n  <tbody>\n    <tr>\n      <td>Data1</td>\n      <td>Data2</td>\n    </tr>\n  </tbody>\n</table>\n");
	}

	SECTION("Table with text")
	{
		string s = "Some text before\n:::table cols=[A][B]\n[1][2]\n:::\nSome text after\n";
		test->convertTables(s);
		REQUIRE(s == "Some text before\n<table>\n  <thead>\n    <tr>\n      <th>A</th>\n      <th>B</th>\n    </tr>\n  </thead>\n  <tbody>\n    <tr>\n      <td>1</td>\n      <td>2</td>\n    </tr>\n  </tbody>\n</table>\nSome text after\n");
	}

	delete test;
}

TEST_CASE("convert paragraph function")
{
    HTMLConverter *test = new HTMLConverter("./test_documents/2.md");
    SECTION("one paragraph")
    {
        string s = "this is a paragraph.\n\n";
        test->convertParagraphs(s);
        REQUIRE(s == "<p>this is a paragraph.</p>");
    }
    SECTION("two paragraphs")
    {
        string s = "this is paragraph one.\n\nthis is paragraph two.";
        test->convertParagraphs(s);
        REQUIRE(s == "<p>this is paragraph one.</p><p>this is paragraph two.</p>");
    }
    SECTION("many paragraphs")
    {
        string s = "this is paragraph one.\n\nthis is paragraph two.\n\nthis is paragraph three.\n\nthis is paragraph four.";
        test->convertParagraphs(s);
        REQUIRE(s == "<p>this is paragraph one.</p><p>this is paragraph two.</p><p>this is paragraph three.</p><p>this is paragraph four.</p>");
    }
    SECTION("ignores when starts with two new lines")
    {
        string s = "\n\nthis is paragraph one.\n\nthis is paragraph two.";
        test->convertParagraphs(s);
        REQUIRE(s == "<p>this is paragraph one.</p><p>this is paragraph two.</p>");
    }
    delete test;
}