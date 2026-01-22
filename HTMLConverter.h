#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;

class HTMLConverter {

public:

   HTMLConverter(const string& filepath);
   void readMD(const string& filepath);
   void convert(const string& outputFilepath);
   void outputToFile();

private:

   vector<string> lines;
   vector<string> output;
   string filepath;

   // some of these are specific to C++ regex and might not work if compiled with other regex engines
   bool hasBoldAndItalics(const string& line); // matches ***text***
   bool hasBold(const string& line);           // matches **text**
   bool hasItalics(const string& line);        // matches *text*
   bool hasLine(const string& line);           // matches ---
   bool hasLink(const string& line);           // matches [YouTube](youtube.com)
   bool hasImage(const string& line);          // matches ![alt text](path/to/photo.png)
   bool hasCodeBlock(const string& line);      // matches ```
   bool hasList(const string& line);           // matches dashed or numbered lists (strings starting with - or a number + .)
   bool hasCodeHighlight(const string& line);  // matches [<code>]
   bool hasTable(const string& line);          // matches :::
   bool hasFilename(const string& line);       // matches ``` filename=""
   int hasHeader(const string& line);          // returns number of #'s at the start of line, followed by space

   // handlers
   void handleBoldAndItalics(string& line); // ***text*** --> <b><em>text</em></b>
   void handleBold(string& line);           // **text** --> <b>text</b>
   void handleItalics(string& line);        // *text* --> <em>text</em>
   void handleHeader(string& line);         // # Hello World --> <h1>Hello World</h1>
   void handleImage(string& line);          // ![alt text](path/to/photo.png) --> <img src="path/to/photo.png" alt="alt text">
   void handleLink(string& line);
};