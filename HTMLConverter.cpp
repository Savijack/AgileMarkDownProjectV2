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
   //cout << markdownContent << endl;
}

void HTMLConverter::convert(const string& outputFilepath)
{
    convertLine(markdownContent);
    convertHeaders(markdownContent);


    htmlOutput = markdownContent;
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
void HTMLConverter::convertLine(std::string& s) {
    static const regex pattern(R"((^|\n)\s*---\s*(?=\n|$))");
    s = regex_replace(s, pattern, "$1<hr>");
}
//--
// RETURNS:
// 1-6 depending on the number of #'s
// 0 if there's no #, or it's not in the correct position, or it's not followed by a space.
int HTMLConverter::hasHeader(const string& line) {
    size_t count = 0; // size_t instead of int to dodge compiler warning

    while (count < line.size() and line[count] == '#') {
        count++;
    }
 
    // if it has one or more #'s, followed by a space 
    if (count > 0 and line[count] == ' ') {
        return count;
    }
    return 0;
}
void HTMLConverter::convertHeaders(string& text) {
    string result;
    string line;
    stringstream ss(text);

    while (getline(ss, line)) {
        int n = hasHeader(line);
        if (n > 0) {
            result += "<h" + to_string(n) + ">" +
                      line.substr(n + 1) +
                      "</h" + to_string(n) + ">";
        } else {
            result += line;
        }
        result += '\n';
    }

    text = result;
}