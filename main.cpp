#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Text {
private:
    std::string content;

public:
    bool loadTextFromFile(const std::string& filePath) {
        std::ifstream inFile(filePath);
        if (!inFile) {
            return false;
        }

        std::string line;
        while (std::getline(inFile, line)) {
            content += line + "\n";
        }

        inFile.close();
        return true;
    }

    const std::string& getContent() const {
        return content;
    }
};

class TextEditor {
public:
    void text() {
        char operation;  // e/d
        std::string inputFile;
        std::string outputFile;
        int shift = 1;

        std::cout << " e/d";
        std::cin >> operation;

        if (operation != 'E' && operation != 'D' && operation != 'd' && operation != 'e') {
            printf("no\n");
            return;
        }

        std::cout << " path to file ";
        std::cin >> inputFile;

        Text text;
        if (!text.loadTextFromFile(inputFile)){
            printf("error\n");

        }
        else printf("good\n");


        const std::string& content = text.getContent();
        std::cout << "Content :" << content << std::endl;
    }
};

int main() {
    TextEditor editor;
    editor.text();
    return 0;
}


//  /home/anastasiia/Documents/abc.txt