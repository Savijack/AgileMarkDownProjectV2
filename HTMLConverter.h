#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
using namespace std;

class HTMLConverter {

public:

   HTMLConverter(const string& filepath);
   void convert(const string& outputFilepath);
   void outputToFile(const string& filepath);

   void separateCodeBlocks(string& s);
   void restoreCodeBlocks(string& s);
   void processCodeblock(string& cb);
   void handleProgramOutput(string& cb);
   
   void convertLine(string& s);
   void convertHeaders(string& s);
   void convertLists(string& text);
   void convertTables(string& text);
   void convertParagraphs(string& line);
   void convertBoldAndItalics(string& line);
   void convertImagesAndLinks(string&line); 

   int hasHeader(const string& line);
   
   vector<string> codeblocks;

private:

   string markdownContent;
   string htmlOutput;
   string filepath;

};