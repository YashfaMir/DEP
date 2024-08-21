#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>  // For stringstream

// Helper function to convert an integer to a string
std::string to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

// Helper function to convert a string to an integer
int stoi(const std::string &str) {
    std::stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

// Function to read the contents of a file into a string
void readFile(const std::string &filename, std::string &content) {
    std::ifstream file(filename.c_str());  // Use .c_str() to convert std::string to const char*
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line;
            if (!file.eof()) { // To avoid adding a newline at the end of the file
                content += "\n";
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
}

// Function to write a string to a file
void writeFile(const std::string &filename, const std::string &content) {
    std::ofstream file(filename.c_str());  // Use .c_str() to convert std::string to const char*
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }
}

// Function to compress a string using Run-Length Encoding
std::string compress(const std::string &data) {
    std::string compressed;
    int n = data.size();
    
    for (int i = 0; i < n; i++) {
        int count = 1;
        while (i < n - 1 && data[i] == data[i + 1]) {
            count++;
            i++;
        }
        compressed += data[i];
        compressed += to_string(count);  // Use custom to_string function
    }
    
    return compressed;
}

// Function to decompress a string using Run-Length Encoding
std::string decompress(const std::string &data) {
    std::string decompressed;
    int n = data.size();
    
    for (int i = 0; i < n; i++) {
        char c = data[i];
        i++;
        std::string countStr;
        while (i < n && std::isdigit(data[i])) {
            countStr += data[i];
            i++;
        }
        i--;
        int count = stoi(countStr);  // Use custom stoi function
        decompressed.append(count, c);
    }
    
    return decompressed;
}

int main() {
    std::string content;
    readFile("input.txt", content); // Read content from "input.txt"
    
    if (content.empty()) {
        std::cerr << "The file is empty." << std::endl;
        return 1;
    }
    
    std::string compressedData = compress(content); // Compress the content
    writeFile("compressed.txt", compressedData); // Write compressed content to "compressed.txt"
    
    std::string decompressedData = decompress(compressedData); // Decompress the content
    writeFile("decompressed.txt", decompressedData); // Write decompressed content to "decompressed.txt"
    
    std::cout << "Compression and decompression complete." << std::endl;
    return 0;
}

