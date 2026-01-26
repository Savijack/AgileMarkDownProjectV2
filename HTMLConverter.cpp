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
    convertImages(markdownContent);
    convertLinks(markdownContent);

    for (auto& cb : codeblocks) {
        processCodeblock(cb);
    }
    restoreCodeBlocks(markdownContent);
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

void HTMLConverter::convertImages(string& line)
{
    string retVal = ""; 

    //go through string
    for(size_t i=0; i<line.length(); i++)
    {
        if(line[i] == '!' && i+1 < line.length() && line[i+1] == '[') //checking for image formatting
        {
            //finding the indexes of the alt/description text
            size_t alt = i+2; 
            size_t endAlt = line.find(']',alt);

            if(line[endAlt+1] == '(' && endAlt != string::npos) //checking for start of url and making sure end of alt was found
            {
                //finding the indexes of the image URL
                size_t url = endAlt+2; 
                size_t endURL = line.find(')',url); 

                if(endURL != string::npos) //making sure end of url found
                {
                    //creating strings for alt and url texts
                    string altText = line.substr(alt, endAlt-alt); 
                    string urlText = line.substr(url, endURL-url); 

                    //update retVal
                    retVal += "<img src=\"" + urlText + "\" alt=\"" + altText + "\">";

                    //get past the image text
                    i = endURL; 
                    continue; 
                }
            }
        }
        retVal += line[i]; 
    }
    line = retVal; 
}
void HTMLConverter::convertLinks(string& line)
{
    string retVal = ""; 

    //go through string
    for(size_t i=0; i<line.length(); i++)
    {
        if(i+1 < line.length() && line[i] == '[') //checking for link formatting
        {
            //finding the indexes of the alt/description text
            size_t text = i+1;
            size_t endText = line.find(']',text);

            if(line[endText+1] == '(' && endText != string::npos) //checking for start of url and making sure end of text was found
            {
                //finding the indexes of the link text
                size_t url = endText+2; 
                size_t endURL = line.find(')',url); 

                if(endURL != string::npos) //making sure end of url found
                {
                    //creating strings for alt and url texts
                    string altText = line.substr(text, endText-text); 
                    string urlText = line.substr(url, endURL-url); 

                    //update retVal
                    retVal += "<a href=\"" + urlText + "\">" + altText + "</a>";

                    //get past the link text
                    i = endURL; 
                    continue; 
                }
            }
        }
        retVal += line[i]; 
    }
    line = retVal; 
}
//--
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
//--
// this function effectively reverses HTMLConverter::separateCodeBlocks()
void HTMLConverter::restoreCodeBlocks(string& s) {
    // for a visualization/explanation of this pattern, see: regexr.com/8jell
    static const regex re(R"(<\{codeblock:(\d+)\}>)");
    smatch m; // smatch is an object that stores matches from regex_search()

    while (regex_search(s, m, re)) {
        size_t i = stoul(m[1].str()); // stoul() is string to long unsigned, to use as indexes in codeblocks vector
        s.replace(m.position(0), m.length(0), codeblocks[i]);
    }
}
//--
void HTMLConverter::processCodeblock(string& cb) {
    string html_str;

    // grab first line for metadata e.g. file="", highlights="", etc
    size_t first_newline = cb.find('\n');
    if (first_newline == string::npos) {
        cb.clear(); // bad codeblock
        return;
    }
    string header = cb.substr(3, first_newline - 3); // header minus ```

    // remove opening ```
    cb = cb.substr(first_newline + 1);

    // remove closing ```
    size_t closing_backticks = cb.rfind("```");
    if (closing_backticks != string::npos) {
        cb.erase(closing_backticks);
    }

    // for visualization/explanation of this pattern, see: regexr.com/8jelu
    static const regex file_pattern(R"(file=\"(.*?)\")");
    smatch m; // smatch is an object that stores matches from regex_search()

    if (regex_search(header, m, file_pattern)) { // if it has a filename specifier
        html_str =
            "<figure class=\"codeblock\">\n"
            "  <figcaption class=\"codeblock__title\">" + m[1].str() + "</figcaption>\n"
            "  <pre><code>\n" +
            cb +
            "\n  </code></pre>\n"
            "</figure>";
    }
    else { // no filename specifier
        html_str =
            "<pre class=\"codeblock\"><code>\n" +
            cb +
            "\n</code></pre>";
    }
    cb = html_str;
}