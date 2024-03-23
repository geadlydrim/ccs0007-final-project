#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void deleteLinesAfterMatch(const std::string& filename, const std::string& match) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool matchFound = false;

    // Read the file line by line
    while (std::getline(inputFile, line)) {
        // Check if the current line contains the match
        if (line.find(match) != std::string::npos) {
            matchFound = true;
            // Remove the current line and the following 3 lines
            for (int i = 0; i < 4 && std::getline(inputFile, line); ++i) {
                // Do nothing, effectively skipping the next lines
            }
        } else {
            // Add the current line to the vector of lines
            lines.push_back(line);
        }
    }

    inputFile.close();

    // Rewrite the file with the modified lines
    std::ofstream outputFile(filename);
    if (!outputFile) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }

    // Rewrite the non-deleted lines to the file
    for (const std::string& l : lines) {
        outputFile << l << std::endl;
    }

    outputFile.close();
}

int main() {
    std::string filename = "example.txt"; // Replace with your file name
    std::string match = "d"; // Replace with the text you want to match

    deleteLinesAfterMatch(filename, match);

    std::cout << "Lines after deletion:" << std::endl;
    std::ifstream checkFile(filename);
    if (checkFile) {
        std::string line;
        while (std::getline(checkFile, line)) {
            std::cout << line << std::endl;
        }
        checkFile.close();
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

    return 0;
}
