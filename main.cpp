#include <iostream>
#include <string>
#include <fstream>
#include <dlfcn.h>

class Data {
    std::string content;
public:
    bool Content(const std::string& inputFile) {
        if (!loadTextFromFile(inputFile)) {
            printf("error\n");
         } else {
            printf("good\n");
        }
        return true;
    }

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

    bool saveTextToFile(const std::string& filePath) const {
        std::ofstream outFile(filePath);
        if (!outFile) {
            return false;
        }

        outFile << content;
        outFile.close();
        return true;
    }

    std::string& getContent() {
        return content;
    }

    void setContent(const std::string& newContent) {
        content = newContent;
    }
    /////////
};

class CaesarCipher {
public:
    bool Point(std::string& content, char operation, int shift) {
        bool success;

        if (operation == 'e' || operation == 'E') {
            success = encrypt(content, shift);
            if (success) {
                std::cout << "Encrypted content: " << content << std::endl;
            }
        } else {
            success = decrypt(content, shift);
            if (success) {
                std::cout << "Decrypted content: " << content << std::endl;
            }
        }
        return success;
    }

    bool encrypt(std::string& content, int shift) {
        void* handle = dlopen("./libcaesar_cipher.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Error: %s\n", dlerror());
            return false;
        }

        void (*caesar_cipher)(char*, int);
        caesar_cipher = (void(*)(char*, int))dlsym(handle, "caesar_cipher");
        if (!caesar_cipher) {
            fprintf(stderr, "Error: %s\n", dlerror());
            dlclose(handle);
            return false;
        }

        caesar_cipher(&content[0], shift);
        dlclose(handle);
        return true;
    }

    bool decrypt(std::string& content, int shift) {
        void* handle = dlopen("./libcaesar_decrypt.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Error: %s\n", dlerror());
            return false;
        }

        void (*caesar_cipher_decryption)(char*, int);
        caesar_cipher_decryption = (void(*)(char*, int))dlsym(handle, "caesar_cipher_decryption");
        if (!caesar_cipher_decryption) {
            fprintf(stderr, "Error: %s\n", dlerror());
            dlclose(handle);
            return false;
        }

        caesar_cipher_decryption(&content[0], shift);
        dlclose(handle);
        return true;
    }
};

class TextEditor {
public:
    void text(std::string& operation, std::string& inputFile, std::string& outputFile, int& shift) {

        std::cout << "e/d: ";
        std::cin >> operation;

        if (operation != "E" && operation != "D" && operation != "d" && operation != "e") {
            printf("no\n");
            return;
        }

        std::cout << "path to file: ";
        std::cin >> inputFile;

        std::cout << "path to output file: ";
        std::cin >> outputFile;

        std::cout << "shift: ";
        std::cin >> shift;
    }
};

int main() {
    TextEditor editor;

    std::string operation;

    std::string inputFile;
    std::string outputFile;

    int shift;

    editor.text(operation, inputFile, outputFile, shift);

    Data data;
    data.Content(inputFile);

    CaesarCipher cipher;
    if (!cipher.Point(data.getContent(), operation[0], shift)) {
        std::cerr << "Failed to process content" << std::endl;
        return 1;
    }

    if (!data.saveTextToFile(outputFile)) {
        std::cerr << "Failed to save content to file" << std::endl;
        return 1;
    }

    return 0;
}




//     /home/anastasiia/Documents/abc1.txt
