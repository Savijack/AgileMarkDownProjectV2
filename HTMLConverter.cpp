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

void HTMLConverter::convertBold(string& line)
{
    string temp = line; // for iteration purposes
    bool inBold = false;
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == '*' && line[i+1] == '*')
        {
            if (inBold)
            {
                temp.replace(i, 2, "</b>"); // ending **
                inBold = false;
            }
            else
            {
                temp.replace(i, 2, "<b>");  // starting **
                inBold = true; 
            }
        }
        line = temp;
    }
}