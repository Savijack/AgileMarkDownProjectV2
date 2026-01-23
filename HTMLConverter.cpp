#include "HTMLConverter.h"

HTMLConverter::HTMLConverter(const string &filepath)
{
   // Opens file markdown file
   ifstream file(filepath);
   string line;

   if (!file.is_open()) {
       cerr << "Error: Could not open the file." << endl;
       exit(0);
   }
   
   // Reads markdown file line by line and adds it to string
   while (getline(file, line)) {
       markdownContent += line + "\n";
   }

   // Closes file after reading
   file.close();
   cout << markdownContent << endl;
}

void HTMLConverter::convert(const string& outputFilepath)
{
    


    outputToFile(outputFilepath);
}
//--
void HTMLConverter::outputToFile(const string& filepath) {
    ofstream outputFile;
    outputFile.open(filepath);

    if (outputFile.is_open()) {
        cout << "file opened" << endl;
    }
    else {
        cout << "file didn't open" << endl;
    }

    // Write HTML content to output file
    outputFile << "<!DOCTYPE html>\n";
    outputFile << "<html>\n";
    outputFile << "<body>\n";
    outputFile << htmlOutput;
    outputFile << "</body>\n";
    outputFile << "</html>\n";
    // Close the file
    outputFile.close();

    std::cout << "HTML file 'output.html' generated successfully." << std::endl;
}
//--
void HTMLConverter::convertLine(string& line) {
    static const regex pattern(R"(^\s*(---)\s*$)");
    line = regex_replace(line, pattern, "<hr>");
}