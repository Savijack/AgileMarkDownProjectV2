# Serene Pathfinders MarkDown Converter

## Project Description

This program is designed to take in a MarkDown file and convert the text to HTML. The project will be able to convert the following traditional MarkDown syntax into HTML:

- Headers
- Paragraphs
- Italics
- Bold
- Lists
- Links
- Images
- Single Lines of Code
- Code Blocks
- Horizontal Lines

Beyond traditional Markdown symbols, the program will convert these custom syntax into HTML:

- Code with File Name
- Highlight Code
- Multiline Highlight
- Create Table

Once the syntax is converted to HTML, the string will be sent into a new HTML file that can be accessed by the user.

## Install Dependencies

A C++ compiler such as Microsoft Visual C++ Compiler is necessary to run this code.

## How To Run The Application

1. Prepare a document in MarkDown form that you wish to transform into HTML.
2. Place the filepath of this document into the HTMLConverter constructor in main.cpp.
3. Click the run button.

## How To Run Tests

1. Create a MarkDown file containing the MarkDown syntax that will be tested.
2. Use a separate .cpp file to perform tests.
3. Ensure test cases cover single and multiple instances of MarkDown syntax.
4. Test cases should have formatting similar to the following example:
```
TEST_CASE("Convert Italics Test Cases")
{
    HTMLConverter * test = new HTMLConverter("./test_documents/1.md");
    {
        string s = "*hello*";
        test->convertItalics(s);
        REQUIRE(s == "<em>hello</em>");
    }

    {
        string s = "this is *italic* text";
        test->convertItalics(s);
        REQUIRE(s == "this is <em>italic</em> text");
    }

    {
        string s = "*one* *two*";
        test->convertItalics(s);
        REQUIRE(s == "<em>one</em> <em>two</em>");
    }
}
```