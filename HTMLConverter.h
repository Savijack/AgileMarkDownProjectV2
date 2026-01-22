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
   void outputToFile();
   string convertItalics(const string& line); 

private:

   vector<string> lines;
   vector<string> output;
   string filepath;

};