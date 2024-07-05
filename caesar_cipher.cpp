#include <stdio.h>
#include <string.h>

void caesar_cipher(char* ask, int shift) {
    for (int i = 0; ask[i] != '\0'; i++) {
        char ch = ask[i];
        if (ch >= 'a' && ch <= 'z') {
            ch = (ch - 'a' + shift) % 26 + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = (ch - 'A' + shift) % 26 + 'A';
        }
        ask[i] = ch;
    }
}
