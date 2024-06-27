#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_TRADE_RECORDS 1024
#define MAX_LINE_LENGTH 1024
#define INITIAL_CAPACITY 10

typedef struct {
    char Source_Currency[4];
    char Destination_Currency[4];
    int tradeAmount;
    double tradePrice;
} Trade_Record;

void HandleMemoryAllocationFailure() {
    fprintf(stderr, "Memory allocation failed.\n");
}

char** AllocateInitialMemory() {
    char** lines = (char**)malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!lines) {
        HandleMemoryAllocationFailure();
    }
    return lines;
}

char** ReallocateMemory(char** lines, int* capacity) {
    *capacity *= 2;
    lines = (char**)realloc(lines, (*capacity) * sizeof(char*));
    if (!lines) {
        HandleMemoryAllocationFailure();
    }
    return lines;
}

char* CopyLine(const char* line) {
    char* copiedLine = (char*)malloc((strlen(line) + 1) * sizeof(char));
    if (copiedLine) {
        strcpy(copiedLine, line);
    }
    return copiedLine;
}

void ReadLinesFromStream(FILE* stream, char*** lines, int* count, int* capacity) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stream)) {
        if (*count >= *capacity) {
            *lines = ReallocateMemory(*lines, capacity);
            if (!*lines) return;
        }
        (*lines)[*count] = CopyLine(line);
        if (!(*lines)[*count]) {
            HandleMemoryAllocationFailure();
            return;
        }
        (*count)++;
    }
}

char** ReadTradeDataFromCsv(FILE* stream, int* numLines) {
    int capacity = INITIAL_CAPACITY;
    int count = 0;
    char** lines = AllocateInitialMemory();
    if (!lines) {
        return NULL;
    }

    ReadLinesFromStream(stream, &lines, &count, &capacity);

    *numLines = count;
    return lines;
}

void HandleTradeDataMemoryAllocationFailure() {
    fprintf(stderr, "Memory allocation failed for trade data.\n");
}

Trade_Record* AllocateTradeData(int numLines) {
    Trade_Record* tradeData = (Trade_Record*)malloc(numLines * sizeof(Trade_Record));
    if (!tradeData) {
        HandleTradeDataMemoryAllocationFailure();
    }
    return tradeData;
}

int ParseCsvLine(char* line, char* fields[], int maxFields) {
    int fieldCount = 0;
    char* token = strtok(line, ",");
    while (token != NULL && fieldCount < maxFields) {
        fields[fieldCount++] = token;
        token = strtok(NULL, ",");
    }
    return fieldCount;
}

void ParseCurrencies(const char* field, char* sourceCurrency, char* destinationCurrency) {
    strncpy(sourceCurrency, field, MAX_CURRENCY_LENGTH);
    sourceCurrency[MAX_CURRENCY_LENGTH] = '\0';
    strncpy(destinationCurrency, field + MAX_CURRENCY_LENGTH, MAX_CURRENCY_LENGTH);
    destinationCurrency[MAX_CURRENCY_LENGTH] = '\0';
}

void MapFieldsToTradeRecord(Trade_Record* record, char* fields[]) {
    ParseCurrencies(fields[0], record->Source_Currency, record->Destination_Currency);
    record->tradeAmount = atoi(fields[1]);
    record->tradePrice = atof(fields[2]);
}

void HandleInvalidCsvFormat(int lineIndex) {
    fprintf(stderr, "Invalid CSV format on line %d\n", lineIndex + 1);
}

Trade_Record* mapCsvLineDataToTradeData(char** lineInFile, int numLines) {
    Trade_Record* tradeData = AllocateTradeData(numLines);
    if (!tradeData) {
        return NULL;
    }

    char* fields[3];
    for (int index = 0; index < numLines; index++) {
        int fieldCount = ParseCsvLine(lineInFile[index], fields, 3);
        if (fieldCount != 3) {
            HandleInvalidCsvFormat(index);
            continue;
        }
        MapFieldsToTradeRecord(&tradeData[index], fields);
    }

    return tradeData;
}

void ValidateCurrency(const char* currency, const char* fieldName, int lineIndex) {
    if (strlen(currency) != 3) {
        fprintf(stderr, "WARN: %s currency on line %d malformed.\n", fieldName, lineIndex + 1);
    }
}

void ValidateTradeAmount(int tradeAmount, int lineIndex) {
    if (tradeAmount <= 0) {
        fprintf(stderr, "WARN: Trade amount on line %d not a valid integer.\n", lineIndex + 1);
    }
}

void ValidateTradePrice(double tradePrice, int lineIndex) {
    if (tradePrice <= 0) {
        fprintf(stderr, "WARN: Trade price on line %d not a valid decimal.\n", lineIndex + 1);
    }
}

void ValidateTradeRecord(Trade_Record* record, int lineIndex) {
    ValidateCurrency(record->Source_Currency, "Source", lineIndex);
    ValidateCurrency(record->Destination_Currency, "Destination", lineIndex);
    ValidateTradeAmount(record->tradeAmount, lineIndex);
    ValidateTradePrice(record->tradePrice, lineIndex);
}

void validateTradeData(Trade_Record* records, int numLines) {
    for (int i = 0; i < numLines; i++) {
        ValidateTradeRecord(&records[i], i);
    }
}

void WriteXML(Trade_Record* records, int numLines) {
    FILE* outFile = fopen("output.xml", "w");
    if (!outFile) {
        fprintf(stderr, "Could not open output.xml for writing.\n");
        return;
    }

    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < numLines; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", records[i].Source_Currency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", records[i].Destination_Currency);
        fprintf(outFile, "\t\t<Amount>%d</Amount>\n", records[i].tradeAmount);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", records[i].tradePrice);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>\n");
    fclose(outFile);
}

void FreeLines(char** lines, int numLines) {
    for (int i = 0; i < numLines; i++) {
        free(lines[i]);
    }
    free(lines);
}

void ConvertDatafromCsvtoXML(FILE* stream) {
    int numLines;
    char** lines = ReadTradeDataFromCsv(stream, &numLines);
    if (!lines) {
        fprintf(stderr, "Failed to read CSV data.\n");
        return;
    }

    Trade_Record* records = mapCsvLineDataToTradeData(lines, numLines);
    if (!records) {
        fprintf(stderr, "Failed to map CSV data to trade data.\n");
        FreeLines(lines, numLines);
        return;
    }

    validateTradeData(records, numLines);
    WriteXML(records, numLines);

    FreeLines(lines, numLines);
    free(records);
}

int main() {
    FILE* csvFile = fopen("../trades.txt", "r");
    if (!csvFile) {
        fprintf(stderr, "Could not open trades.txt: %s\n", strerror(errno));
        return 1;
    }

    ConvertDatafromCsvtoXML(csvFile);
    fclose(csvFile);
    return 0;
}
