#include "HTMLConverter.h"

HTMLConverter::HTMLConverter(const string &filepath)
{
   ifstream file(filepath);
   string line;

   if (!file.is_open()) {
       cerr << "Error: Could not open the file." << endl;
       exit(0);
   }

   while (getline(file, line)) {
       lines.push_back(line);
   }

   file.close();
   for (size_t i = 0; i < lines.size(); i++)
   {
       cout << lines[i] << endl;
   }
}

void HTMLConverter::convert(const string& outputFilepath)
{
    for (auto& line : lines) {
        if (hasBoldAndItalics(line)) {
            handleBoldAndItalics(line);
        }
        if (hasBold(line)) {
            handleBold(line);
        }
        if (hasItalics(line)) {
            handleItalics(line);
        }
        if (hasHeader(line)) {
            handleHeader(line);
        }
    }

    ofstream outputFile;
    // Specify the filename with the .html extension
    outputFile.open(outputFilepath);

    if(outputFile.is_open())
    {
        cout << "file opened" << endl;
    }
    else
    {
        cout << "file didn't open" << endl;
    }

    // Write HTML content as strings
    outputFile << "<!DOCTYPE html>\n";
    outputFile << "<html>\n";
    outputFile << "<body>\n";

    for(size_t i = 0; i < output.size(); i++)
    {
        outputFile << output[i];
    }

    outputFile << "</body>\n";
    outputFile << "</html>\n";
    // Close the file
    outputFile.close();

    std::cout << "HTML file 'output.html' generated successfully." << std::endl;
}
bool HTMLConverter::hasBoldAndItalics(const string& line) {
    static const regex pattern(R"(\*{3}.+?\*{3})");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasBold(const string& line) {
    // assumes boldAndItalics are already handled
    static const regex pattern(R"(\*{2}.*\*{2})");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasItalics(const string& line) {
    // assumes bold is already handled
    static const regex pattern(R"(\*(?!\*)([^*]+)\*)");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasLine(const string& line) {
    static const regex pattern(R"(^\s*---\s*$)");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasLink(const string& line) {
    static const regex pattern(R"((^|[^!])\[[^\]]+\]\([^)]+\))");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasImage(const string& line) {
    static const regex pattern(R"(!\[[^\]]+\]\([^)]+\))");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasCodeBlock(const string& line) {
    static const regex pattern(R"(`{3})");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasList(const string& line) {
    static const regex dash_pattern(R"(^\s{0,3}-\s+)");
    static const regex number_pattern(R"(^\s{0,3}\d+\.\s+)");
    return regex_search(line, dash_pattern) or regex_search(line, number_pattern);
}
bool HTMLConverter::hasCodeHighlight(const string& line) {
    static const regex pattern(R"(\[<[^>]+>\])");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasTable(const string& line) {
    static const regex pattern(R"(^:::)");
    return regex_search(line, pattern);
}
bool HTMLConverter::hasFilename(const string& line) {
    static const regex pattern(R"(```\s{1}file=\".*\")");
    return regex_search(line, pattern);
}
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

void HTMLConverter::handleBoldAndItalics(string& line) {
    static const regex pattern(R"(\*{3}(.*)\*{3})");
    line = regex_replace(line, pattern, "<b><em>$1</em></b>");
}
void HTMLConverter::handleBold(string& line) {
    static const regex pattern(R"(\*\*(.*?)\*\*)");
    line = regex_replace(line, pattern, "<b>$1</b>");
}
void HTMLConverter::handleItalics(string& line) {
    static const regex pattern(R"(\*(.*)\*)");
    line = regex_replace(line, pattern, "<em>$1</em>");
}
void HTMLConverter::handleHeader(string& line) {
    int num_headers = hasHeader(line);
    if (num_headers < 1) return;
    static const regex pattern(R"(^#+\s+(.*))");

    line = "<h" + to_string(num_headers) + ">"
        + regex_replace(line, pattern, "$1")
        + "</h" + to_string(num_headers) + ">";
}