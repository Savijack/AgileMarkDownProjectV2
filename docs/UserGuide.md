# User Operations Guide

## Software Overview

Serene Pathfinder's Agile Markdown Converter allows the user to input a Markdown (.md) file and convert its syntax to HTML. The syntax will then be placed in a new HTML file accessible by the user. All our program needs is the path of the Markdown file to convert to HTML. 

## What is Markdown

Markdown is a markup language that is designed to add formatting elements to a text document. Special characters and syntax are used to structure a document according to user specifications. The Agile Markdown Converter uses both traditional and custom Markdown syntax to achieve this. To format a text file with the Agile Markdown Converter, use the listed syntax below:

- Headers (1-6 #'s)
    + Ex: # Header Text

- Paragraphs (\n \n; these are newline characters created when pressing enter)
    + Ex: Here is a paragraph. \n \
    \n \
    Here is the next paragraph.
- Italics (\*italics\*)
    + Ex: \*italic word\* -> *italic word*
- Bold (\*\*bolded\*\*)
    + Ex: \*\*bolded word\*\* -> **bolded word**
- Ordered Lists (\n1. text)
    + Ex: \
    \n 1. this \
    \n 2. is an \
    \n 3. ordered list
- Unordered Lists (\n_ text; in this case the underscore indicates a space)
    + Ex:\
    \n - this\
    \n - is an\
    \n - unordered list
- Links ([link text]\(url))
    + Ex: [YouTube]\(https://www.youtube.com/)
- Images (![alt text]\(url); alt text is simply used to describe the image)
    + Ex: ![Picture of Professor]\(https://avatars.githubusercontent.com/u/934916?v=4)
- Single Lines of Code (`)
    + Ex: \`int x = 10;\`
- Horizontal Lines (---; leave empty lines in between to avoid creating a header)
    + Ex: These two blocks \n \
    \n\
    \--- \n\
    \n\
    Are separated by a horizontal line
- Code Blocks (\```)
    + Ex: \``` \\n
\
          int x = 10; \
          int y = 10; \
          int z = 10 * 10;
\
          \```
- Code with File Name (``` file = "main.cpp")
    + Ex:\
    \``` file="main.cpp"\
    int x;\
    x = 10;\
    \```
- Highlight Code (\[\<highlighted code>])
    + Ex:\
    \``` file="main.cpp"\
    [<int x;>]\
    x = 10;\
    \```
- Multiline Highlight (highlight = "line number-line number, line number")
    + Ex: \
    \``` file="main.cpp" highlight="1-3, 5"\
    int x = 10;\
    int y = 20;\
    int z = 30;\
    int a = xz;\
    int b = ay;\
    \```

- Create Table
    + Ex:\
    :::table tableName= [col1][col2][col3]\
    &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;[text][text][text]\
    :::

## How to Run the Application

1. Prepare a document in MarkDown (.md) form that you wish to transform into HTML.
2. Open main.cpp.
3. Open the run and debug tab.
4. Click the run button.
5. Place the input filepath of your Markdown document into the terminal and press enter.
6. Place the output filepath of the HTML file you wish to create and press enter.

## Examples

Check the "test_documents" folder for example Markdown documents. 