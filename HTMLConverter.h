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
   void outputToFile(const string& filepath);

   void convertLine(string& s);
   void convertHeaders(string& s);

   int hasHeader(const string& line);
   
private:

   string markdownContent;
   string htmlOutput;
   string filepath;

};