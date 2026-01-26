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
    // this should be one of the first things called to make things easier for the other functions to parse
    separateCodeBlocks(markdownContent);

    convertLine(markdownContent);
    convertHeaders(markdownContent);
    convertBold(markdownContent);
    convertItalics(markdownContent);

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


    cout << "HTML file 'output.html' generated successfully." << endl;
}


void HTMLConverter::convertBold(string& line)
{
    bool inBold = false;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == '*' && line[i+1] == '*') // looking for **
        {
            if (inBold)
            {
                line.replace(i, 2, "</b>"); // handles the ending **
                inBold = false;
            }
            else 
            {
                line.replace(i, 2, "<b>");  // handles the starting **
                inBold = true; 
            }
        }
    }
    std::cout << "HTML file 'output.html' generated successfully." << std::endl;
}
//--
void HTMLConverter::convertLine(string& s) {
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
void HTMLConverter::separateCodeBlocks(string& s) {
    // for a visualization of what this pattern matches, see: regexr.com/8jdpk
    static const regex re(R"(```(?:.|\n)*?```)");
    smatch m; // smatch is an object that stores matches from regex_search()
    size_t count = 0;

    while (regex_search(s, m, re)) {
         // stash each codeblock in a vector<string> that's a member of HTMLConverter class
        codeblocks.push_back(m[0].str());

        // inserts a string that looks like <{codeblock:i}> in place of the actual codeblock. i is the index in the codeblocks vector.
        string placeholder = "<{codeblock:" + to_string(count++) + "}>";
        s.replace(m.position(0), m.length(0), placeholder);
    }
}

void HTMLConverter::convertItalics(string& line)
{
    string retVal = "";
    bool inItalics = false;

    //going through the string, checking each char for '*', and replacing with proper format when found
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == '*' && line[i+1] != '*') {
            if (inItalics) {
                retVal += "</em>"; 
            }
            else {
                retVal += "<em>";
            }
            inItalics = !inItalics; 
        }
        else {
            retVal += line[i]; 
        }
    }

    line = retVal;
}