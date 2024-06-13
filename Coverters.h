int ConverttoInt(const char* input_string, int* value) {
    char* endptr;
    *value = strtol(input_string, &endptr, 10);
    if (endptr == input_string) {
        return 0;
    }
    return 1;
}
 

int ConverttoDouble(const char* input_string, double* value) {
    char* endptr;
    *value = strtod(input_string, &endptr);
    if (endptr == input_string) {
        return 0;
    }
    return 1;
}
