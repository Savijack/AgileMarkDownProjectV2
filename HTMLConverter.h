#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class HTMLConverter {

public:

   HTMLConverter(const string& filepath);
   void readMD(const string& filepath);
   void convert(const string& outputFilepath);
   void outputToFile(const string& filepath);
   void convertBold(string& line);


private:

   string markdownContent;
   string htmlOutput;
   string filepath;

};