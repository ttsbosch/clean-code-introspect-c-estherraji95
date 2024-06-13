char** SplitString(const char* input_string, char delimiter) {
    int count = 0;
    const char* ptr = input_string;
    while (*ptr != '\0') {
        if (*ptr++ == delimiter) {
            count++;
        }
    }
 
    char** tokens = (char**)malloc(sizeof(char*) * (count + 2));
    int i = 0;
    ptr = input_string;
    char* token = (char*)malloc(strlen(input_string) + 1);
    int j = 0;
    while (*ptr != '\0') {
        if (*ptr == delimiter) {
            token[j] = '\0';
            tokens[i++] = strdup(token);
            j = 0;
        } else {
            token[j++] = *ptr;
        }
        ptr++;
    }
    token[j] = '\0';
    tokens[i++] = strdup(token);
    tokens[i] = NULL;
    free(token);
    return tokens;
}
