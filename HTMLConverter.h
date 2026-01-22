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
   void handleBoldAndItalics(string& line);
   void handleBold(string& line);
   void handleItalics(string& line);
   void handleHeader(string& line);
};