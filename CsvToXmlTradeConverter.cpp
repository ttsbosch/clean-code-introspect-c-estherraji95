#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "TradeRecord.h"
#include "StringProcessor.h"

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

void ReadLinesFromStream(FILE* stream, char** lines, int* count, int* capacity) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), stream)) {
        if (*count >= *capacity) {
            lines = ReallocateMemory(lines, capacity);
            if (!lines) return;
        }
        lines[*count] = CopyLine(line);
        if (!lines[*count]) {
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

    ReadLinesFromStream(stream, lines, &count, &capacity);

    *numLines = count;
    return lines;
}

char **ReadTradeDataFromCsv(FILE *stream, int *numLines) {
    char **lines = NULL;
    char line[MAX_LINE_LENGTH];
    int capacity = INITIAL_CAPACITY;
    int count = 0;

    lines = (char **)malloc(capacity * sizeof(char *));
    if (!lines) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), stream)) {
        if (count >= capacity) {
            capacity *= 2;
            lines = (char **)realloc(lines, capacity * sizeof(char *));
            if (!lines) {
                fprintf(stderr, "Memory reallocation failed.\n");
                return NULL;
            }
        }
        lines[count] = (char *)malloc((strlen(line) + 1) * sizeof(char));
        strcpy(lines[count], line);
        count++;
    }

    *numLines = count;
    return lines;
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

void validateTradeData(Trade_Record *records, int numLines) {
    for (int i = 0; i < numLines; i++) {
        if (strlen(records[i].Source_Currency) != 3 || strlen(records[i].Destination_Currency) != 3) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed.\n", i + 1);
        }
        if (records[i].tradeAmount <= 0) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer.\n", i + 1);
        }
        if (records[i].tradePrice <= 0) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal.\n", i + 1);
        }
    }
}

void WriteXML(Trade_Record *records, int numLines) {
    FILE *outFile = fopen("output.xml", "w");
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
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
}

void ConvertDatafromCsvtoXML(FILE *stream) {
    int numLines;
    char **lines = ReadTradeDataFromCsv(stream, &numLines);
    if (!lines) {
        fprintf(stderr, "Failed to read CSV data.\n");
        return;
    }

    Trade_Record *records = mapCsvLineDataToTradeData(lines, numLines);
    if (!records) {
        fprintf(stderr, "Failed to map CSV data to trade data.\n");
        for (int i = 0; i < numLines; i++) {
            free(lines[i]);
        }
        free(lines);
        return;
    }

    validateTradeData(records, numLines);
    WriteXML(records, numLines);

    for (int i = 0; i < numLines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(records);
}

int main() {
    FILE *csvFile = fopen("../trades.txt", "r");
    if (!csvFile) {
        fprintf(stderr, "Could not open trades.txt: %s\n", strerror(errno));
        return 1;
    }

    ConvertDatafromCsvtoXML(csvFile);
    fclose(csvFile);
    return 0;
}
