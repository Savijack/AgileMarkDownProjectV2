#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

class HTMLConverter {

public:

   HTMLConverter(const string& filepath);
   void convert(const string& outputFilepath);
   void outputToFile(const string& filepath);

   void separateCodeBlocks(string& s);
   void restoreCodeBlocks(string& s);
   void processCodeblock(string& cb);

   void convertBold(string& line);
   void convertLine(string& s);
   void convertHeaders(string& s);
   void convertItalics(string& line); 
   void convertImages(string& line);
   void convertLinks(string& line);

   int hasHeader(const string& line);
   
   vector<string> codeblocks;

private:

   string markdownContent;
   string htmlOutput;
   string filepath;
   

};