#include <iostream>
#include <string>
#include <fstream>
#include <dlfcn.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

class Data {

public:
    size_t  Size = 256;

    char* content = new char[Size];

    char* inputFile = new char[Size];
    char* outputFile = new char[Size];
    int shift;



    Data() {
        inputFile[0] = '\0';
        outputFile[0] = '\0';

        content[0] = '\0';

    }

    ~Data() {
        delete[] inputFile;
        delete[] outputFile;
        delete[] content;
    }

    bool Content(const char* inputFile) {
        if (!loadTextFromFile(inputFile)) {
            printf("error\n");
            return false;
        } else {
            printf("good\n");
        }
        return true;
    }


    bool loadTextFromFile(const char* inputFile) {
        std::ifstream inFile(inputFile);
        if (!inFile) {
            return false;
        }

        std::string line;
        size_t currentLength = 0;

        while (std::getline(inFile, line)) {
            size_t lineLength = line.length() + 1; // +1 для \n
            if (currentLength + lineLength >= Size) {
                Size *= 2;
                char* newContent = new char[Size];
                std::memcpy(newContent, content, currentLength);
                delete[] content;
                content = newContent;
            }


            std::strcpy(content + currentLength, line.c_str());
            currentLength += lineLength;
            content[currentLength - 1] = '\n';
        }

        inFile.close();
        return true;
    }

    bool saveTextToFile(const char* filePath, const char* content) const {
        int out_File = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_File == -1) {
            return false;
        }

        ssize_t bytesWritten = write(out_File, content, strlen(content));
        if (bytesWritten == -1) {
            close(out_File);
            return false;
        }

        if (close(out_File) == -1) {
            return false;
        }

        return true;
    }





    char* getContent() {
        return content;
    }

    void setContent(const char* newContent) {
        size_t newContentLength = std::strlen(newContent);
        if (newContentLength >= Size) {
            delete[] content;
            Size = newContentLength + 1;
            content = new char[Size];
        }
        std::strcpy(content, newContent);
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
    void text(char* operation, char* inputFile, char* outputFile, int& shift) {
        std::cout << "e/d: ";
        std::cin >> operation;

        if (strcmp(operation, "E") != 0 && strcmp(operation, "D") != 0 && strcmp(operation, "e") != 0 && strcmp(operation, "d") != 0) {
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
    Data data;
    TextEditor editor;

    int Size_operation = 10;
    char *operation = new char[Size_operation];

    editor.text(operation, data.inputFile, data.outputFile, data.shift);

    if (!data.Content(data.inputFile)) {
        std::cerr << "Failed to load content from file" << std::endl;
        delete[] operation;
        return 1;
    }

    CaesarCipher cipher;
    std::string contentStr = data.getContent();
    if (!cipher.Point(contentStr, operation[0], data.shift)) {
        std::cerr << "Failed to process content" << std::endl;
        delete[] operation;
        return 1;
    }

    data.setContent(contentStr.c_str());

    if (!data.saveTextToFile(data.outputFile, data.content)) {
        std::cerr << "Failed to save content to file" << std::endl;
        delete[] operation;
        return 1;
    }

    delete[] operation;
    return 0;
}



// /home/anastasiia/Documents/abc1256.txt








