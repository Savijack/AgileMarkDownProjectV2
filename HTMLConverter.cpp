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

    convertTables(markdownContent);
    convertLists(markdownContent);
    convertLine(markdownContent);
    convertHeaders(markdownContent);
    convertBold(markdownContent);
    convertItalics(markdownContent);
    convertImages(markdownContent);
    convertLinks(markdownContent);
    convertParagraphs(markdownContent);

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
    figure.codeblock.program-output pre.codeblock {
        padding: 0.75em 1em;
    }

    figure.codeblock.program-output pre.codeblock > code {
        padding: 0;
    }

    figure.codeblock.program-output pre.codeblock {
        line-height: 1.25;
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
    
    if (cb.find("program-output") != string::npos) {
        handleProgramOutput(cb);
        return;
    }
    
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
                    if (n >= 1 and static_cast<size_t>(n) <= lines.size()) {
                        highlight[n - 1] = true;
                    }
                }
            }
        }
    }

    string code_html;
    code_html += "<pre class=\"codeblock\"><code>\n";

    for (size_t i = 0; i < lines.size(); ++i) {
        string classes = "line";
        if (highlight[i]) {
            classes += " highlight";
        }

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
    } 
    else {
        cb = code_html;
    }
}

void HTMLConverter::convertLists(string& text)
{
    string result;
    string line;
    stringstream ss(text);
    
    vector<string> listStack;  // stack to track open list tags
    vector<int> indentStack;   // stack to track actual indents

    // since markdown is very flexible with indentation, this tracks what the text is using to avoid guessing
    int indentUnit = 0;
    
    // regex patterns for list items
    static const regex unorderedPattern(R"(^(\s*)([-*+]) (.+)$)");
    static const regex orderedPattern(R"(^(\s*)(\d+)\. (.+)$)");
    
    while (getline(ss, line))
    {
        smatch match;
        bool isUnorderedItem = false;
        bool isOrderedItem = false;
        int spaceCount = 0;
        size_t contentStart = 0;
        
        // check for unordered list marker
        if (regex_match(line, match, unorderedPattern))
        {
            isUnorderedItem = true;
            spaceCount = match[1].length();
            contentStart = match.position(3);
        }
        // check for ordered list marker
        else if (regex_match(line, match, orderedPattern))
        {
            isOrderedItem = true;
            spaceCount = match[1].length();
            contentStart = match.position(3);
        }
        else
        {
            // count leading spaces for non-list lines
            while (spaceCount < (int)line.length() && line[spaceCount] == ' ')
            {
                spaceCount++;
            }
        }
        
        // is list ordered or unordered
        string itemListType = "";
        if (isUnorderedItem)
        {
            itemListType = "ul";
        }
        else if (isOrderedItem)
        {
            itemListType = "ol";
        }
        
        if (itemListType != "")
        {
            // get indent unit from first line
            if (indentUnit == 0 && !indentStack.empty() && spaceCount > indentStack.back())
            {
                indentUnit = spaceCount - indentStack.back();
            }
            
            // close lists that are at deeper indent levels than current
            while (!indentStack.empty() && indentStack.back() > spaceCount)
            {
                indentStack.pop_back();
                string closingTag = listStack.back();
                listStack.pop_back();
                for (int i = 0; i < (int)indentStack.size(); i++)
                {
                    result += "  ";
                }
                result += "</" + closingTag + ">\n";
            }
            
            // check if we need to open a new list
            if (indentStack.empty() || spaceCount > indentStack.back())
            {
                // opening a nested list
                for (int i = 0; i < (int)indentStack.size(); i++)
                {
                    result += "  ";
                }
                result += "<" + itemListType + ">\n";
                listStack.push_back(itemListType);
                indentStack.push_back(spaceCount);
            }
            else if (spaceCount == indentStack.back() && !listStack.empty() && listStack.back() != itemListType)
            {
                // list type changed at same level
                for (int i = 0; i < (int)indentStack.size() - 1; i++)
                {
                    result += "  ";
                }
                result += "</" + listStack.back() + ">\n";
                listStack.pop_back();
                indentStack.pop_back();
                
                for (int i = 0; i < (int)indentStack.size(); i++)
                {
                    result += "  ";
                }
                result += "<" + itemListType + ">\n";
                listStack.push_back(itemListType);
                indentStack.push_back(spaceCount);
            }
            
            // add the list item
            for (int i = 0; i < (int)indentStack.size(); i++)
            {
                result += "  ";
            }
            result += "<li>" + line.substr(contentStart) + "</li>\n";
        }
        else
        {
            // not a list item - close any open lists
            while (!indentStack.empty())
            {
                indentStack.pop_back();
                string closingTag = listStack.back();
                listStack.pop_back();
                for (int i = 0; i < (int)indentStack.size(); i++)
                {
                    result += "  ";
                }
                result += "</" + closingTag + ">\n";
            }
            result += line + "\n";
        }
    }
    
    // close any remaining open lists
    while (!indentStack.empty())
    {
        indentStack.pop_back();
        string closingTag = listStack.back();
        listStack.pop_back();
        for (int i = 0; i < (int)indentStack.size(); i++)
        {
            result += "  ";
        }
        result += "</" + closingTag + ">\n";
    }
    
    text = result;
}

void HTMLConverter::handleProgramOutput(string& cb) {
    // find first newline (end of ```program-output line)
    size_t first_newline = cb.find('\n');
    if (first_newline == string::npos) {
        cb.clear();
        return;
    }

    // strip opening ``` line
    cb = cb.substr(first_newline + 1);

    // strip closing ```
    size_t closing_backticks = cb.rfind("```");
    if (closing_backticks != string::npos) {
        cb.erase(closing_backticks);
    }

    // return HTML conversion through parameter
    cb =
        "<figure class=\"codeblock program-output\">\n"
        "<figcaption class=\"codeblock__title\">Program Output</figcaption>\n"
        "<pre class=\"codeblock\"><code>\n"
        + htmlEscape(cb) +
        "\n</code></pre>\n"
        "</figure>\n";
}

void HTMLConverter::convertTables(string& text)
{
    string result;
    string line;
    stringstream ss(text);
    
    // regex pattern for table block start: :::table cols=[...][...][...]
    static const regex tableStartPattern(R"(^(\s*):::table\s+cols=(.+)$)");
    // regex pattern for table row: [item1][item2][item3]
    static const regex tableRowPattern(R"(^(\s*)\[(.+)\](\[.+\])*$)");
    // regex pattern for table block end: :::
    static const regex tableEndPattern(R"(^(\s*):::$)");
    
    while (getline(ss, line))
    {
        smatch match;
        bool isTableStart = false;
        bool isTableEnd = false;
        bool isTableRow = false;
        
        // check for table start
        if (regex_match(line, match, tableStartPattern))
        {
            isTableStart = true;
        }
        // check for table end
        else if (regex_match(line, match, tableEndPattern))
        {
            isTableEnd = true;
        }
        // check for table row
        else if (regex_match(line, match, tableRowPattern))
        {
            isTableRow = true;
        }
        
        if (isTableStart)
        {
            // get column headers
            string colsStr = match[2].str();
            vector<string> columns;
            
            static const regex colPattern(R"(\[([^\]]+)\])");
            smatch colMatch;
            string::const_iterator searchStart(colsStr.cbegin());
            
            while (regex_search(searchStart, colsStr.cend(), colMatch, colPattern))
            {
                columns.push_back(colMatch[1].str());
                searchStart = colMatch.suffix().first;
            }
            
            // start table
            result += "<table>\n";
            result += "  <thead>\n";
            result += "    <tr>\n";
            
            for (const auto& col : columns)
            {
                result += "      <th>" + col + "</th>\n";
            }
            
            result += "    </tr>\n";
            result += "  </thead>\n";
            result += "  <tbody>\n";
        }
        else if (isTableEnd)
        {
            // close table
            result += "  </tbody>\n";
            result += "</table>\n";
        }
        else if (isTableRow)
        {
            // get row items
            vector<string> items;
            static const regex itemPattern(R"(\[([^\]]+)\])");
            smatch itemMatch;
            string::const_iterator searchStart(line.cbegin());
            
            while (regex_search(searchStart, line.cend(), itemMatch, itemPattern))
            {
                items.push_back(itemMatch[1].str());
                searchStart = itemMatch.suffix().first;
            }
            
            // add row
            result += "    <tr>\n";
            for (const auto& item : items)
            {
                result += "      <td>" + item + "</td>\n";
            }
            result += "    </tr>\n";
        }
        else
        {
            // non-table line
            result += line + "\n";
        }
    }
    
    text = result;
}

void HTMLConverter::convertParagraphs(string& line)
{
    string retVal = "";
    size_t start = 0;

    while (start <= line.size() - 1)
    {
        size_t temp = line.find("\n\n", start);
        if (temp == string::npos)
        {
            temp = line.size();
        }
        string paragraph = line.substr(start, temp - start);
        if (!paragraph.empty())
        {
            retVal += "<p>" + paragraph + "</p>";
        }
        start = temp + 2;
    }
    line = retVal;
}