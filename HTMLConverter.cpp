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


    cout << "HTML file 'output.html' generated successfully." << endl;
}


void HTMLConverter::convertBold(string& line)
{
    bool inBold = false;
    for (int i = 0; i < line.size(); i++)
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
}