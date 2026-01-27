# User Operations Guide

## Software Overview

Serene Pathfinder's Agile Markdown Converter allows the user to input a Markdown file and convert its syntax to HTML. All our program needs is the path of the Markdown file you wish to convert to HTML. 

## How to Use Our Program

1. Prepare a document in MarkDown form that you wish to transform into HTML.
2. Open main.cpp.
3. Place the filepath of this document into the HTMLConverter constructor in main.cpp.
4. Click the run button.

## Examples

Check the "test_documents" folder for example Markdown documents. Use Markdown syntax as described below:
- Headers (1-6 #'s)
- Paragraphs (\n \n)
- Italics (\*italics\*)
- Bold (\*\*bolded\*\*)
- Ordered Lists (\n_ text)
- Unordered Lists (\n1. text)
- Links ([link text]\(url))
- Images (![alt text]\(url))
- Single Lines of Code (`)
- Code Blocks (```)
- Horizontal Lines (---)
- Code with File Name (``` file = "main.cpp")
- Highlight Code ([<int x;>])
- Multiline Highlight (highlight = "3-5, 8")
- Create Table:
    :::table [col1][col2][col3]
             [text][text][text]
    :::