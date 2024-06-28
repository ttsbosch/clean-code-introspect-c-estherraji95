#include <stdlib.h>
#include <string.h>

int CountDelimiters(const char* input_string, char delimiter) {
    int count = 0;
    const char* ptr = input_string;
    while (*ptr != '\0') {
        if (*ptr++ == delimiter) {
            count++;
        }
    }
    return count;
}

char** AllocateTokens(int count) {
    return (char**)malloc(sizeof(char*) * (count + 2));
}

void CopyToken(char* destination, const char* source, int length) {
    strncpy(destination, source, length);
    destination[length] = '\0';
}

char** SplitString(const char* input_string, char delimiter) {
    int delimiter_count = CountDelimiters(input_string, delimiter);
    char** tokens = AllocateTokens(delimiter_count);
    
    const char* ptr = input_string;
    int i = 0;
    const char* start = ptr;
    
    while (*ptr != '\0') {
        if (*ptr == delimiter) {
            int token_length = ptr - start;
            char* token = (char*)malloc(token_length + 1);
            CopyToken(token, start, token_length);
            tokens[i++] = token;
            start = ptr + 1;
        }
        ptr++;
    }
    
    // Handle the last token
    int token_length = ptr - start;
    char* token = (char*)malloc(token_length + 1);
    CopyToken(token, start, token_length);
    tokens[i++] = token;
    tokens[i] = NULL;
    
    return tokens;
}
