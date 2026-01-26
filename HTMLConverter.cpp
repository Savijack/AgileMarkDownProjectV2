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
    outputFile << "<html>\n<head>\n";
    outputFile << "<style>\n";
    outputFile << 
    R"(
    figure.codeblock {
        margin: 1.5em 0;
        border: 1px solid #444;
        border-radius: 10px;
        overflow: hidden;
        background: #1e1e1e;
    }

    figure.codeblock > figcaption.codeblock__title {
        background: #2a2a2a;
        color: #ddd;
        font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
        font-size: 0.9em;
        padding: 0.5em 0.75em;
        text-align: center;
        border-bottom: 1px solid #444;
    }

    pre.codeblock {
        margin: 0;
        padding: 0.75em 0;
        overflow-x: auto;
        counter-reset: line;
    }

    pre.codeblock > code {
        display: block;
        font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
        font-size: 0.95em;
        line-height: 1;
        color: #eee;
    }

    pre.codeblock .line {
        display: block;
        position: relative;
        padding: 0 1em 0 4.25em;
        white-space: pre;
    }

    pre.codeblock .line::before {
        counter-increment: line;
        content: counter(line);
        position: absolute;
        left: 0;
        width: 3.25em;
        padding-right: 0.75em;
        text-align: right;
        color: #888;
        user-select: none;
    }

    pre.codeblock .line.highlight {
        background: rgba(255, 255, 0, 0.12);
    }

    pre.codeblock .line.highlight::before {
        color: #ffd76a;
    }

    pre.codeblock ::selection {
        background: rgba(120, 160, 255, 0.25);
    }
    )";
    outputFile << "</style>\n</head>\n<body>\n";
    outputFile << htmlOutput;
    outputFile << "</body></html>";
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
// this is a helper for processCodeblock
// it exists so I can put code in html safely
static string htmlEscape(const string& s) {
    string out;
    out.reserve(s.size());
    for (char c : s) {
        switch (c) {
            case '&': out += "&amp;";  break;
            case '<': out += "&lt;";   break;
            case '>': out += "&gt;";   break;
            case '"': out += "&quot;"; break;
            default:  out += c;        break;
        }
    }
    return out;
}

void HTMLConverter::processCodeblock(string& cb) {
    // grab first line for metadata e.g. file="", highlight=""
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

    // split into lines
    vector<string> lines;
    string line;
    stringstream ss(cb);
    while (getline(ss, line)) {
        lines.push_back(line);
    }

    // highlight flags (one per line)
    vector<bool> highlight(lines.size(), false);

    // handles [< ... >] whole-line syntax ----
    for (size_t i = 0; i < lines.size(); ++i) {
        string& raw = lines[i];
        if (raw.size() >= 4 and
            raw.rfind("[<", 0) == 0 and
            raw.substr(raw.size() - 2) == ">]") {

            highlight[i] = true;
            raw = raw.substr(2, raw.size() - 4); // strip [< and >]
        }
    }

    // handles highlight="3-5,8" header syntax
    static const regex highlight_pattern(R"(highlight=\"(.*?)\")");
    smatch hm;
    if (regex_search(header, hm, highlight_pattern)) {
        string spec = hm[1].str();
        string token;
        stringstream hs(spec);

        while (getline(hs, token, ',')) {
            size_t dash = token.find('-');
            if (dash == string::npos) {
                int n = stoi(token);
                if (n >= 1 and (size_t)n <= lines.size()) {
                    highlight[n - 1] = true;
                }
            } 
            else {
                int a = stoi(token.substr(0, dash));
                int b = stoi(token.substr(dash + 1));
                if (a > b) {
                    swap(a, b);
                }
                for (int n = a; n <= b; ++n) {
                    if (n >= 1 and static_cast<size_t>(n) <= lines.size())
                        highlight[n - 1] = true;
                }
            }
        }
    }

    string code_html;
    code_html += "<pre class=\"codeblock\"><code>\n";

    for (size_t i = 0; i < lines.size(); ++i) {
        string classes = "line";
        if (highlight[i]) classes += " highlight";

        code_html += "<span class=\"" + classes + "\">"
                   + htmlEscape(lines[i]) +
                   "</span>\n";
    }

    code_html += "</code></pre>\n";

    // filename handler
    static const regex file_pattern(R"(file=\"(.*?)\")");
    smatch m;
    if (regex_search(header, m, file_pattern)) {
        cb =
            "<figure class=\"codeblock\">\n"
            "<figcaption class=\"codeblock__title\">" + htmlEscape(m[1].str()) + "</figcaption>\n" +
            code_html +
            "</figure>\n";
    } else {
        cb = code_html;
    }
}

