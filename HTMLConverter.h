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

   void convertLine(string& s);

private:

   string markdownContent;
   string htmlOutput;
   string filepath;

};