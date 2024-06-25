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

Trade_Record* mapCsvLineDataToTradeData(char **lineInFile, int numLines) {
    Trade_Record *tradeData;
    tradeData = (Trade_Record *)malloc(numLines * sizeof(Trade_Record));
    if (!tradeData) {
        fprintf(stderr, "Memory allocation failed for trade data.\n");
        return NULL;
    }

    char *fields[3];
    int fieldCount;
    for (int index = 0; index < numLines; index++) {
        fieldCount = 0;
        char *token = strtok(lineInFile[index], ",");
        while (token != NULL && fieldCount < 3) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }
        if (fieldCount != 3) {
            fprintf(stderr, "Invalid CSV format on line %d\n", index + 1);
            continue;
        }

        strncpy(tradeData[index].Source_Currency, fields[0], 3);
        tradeData[index].Source_Currency[3] = '\0';
        strncpy(tradeData[index].Destination_Currency, fields[0] + 3, 3);
        tradeData[index].Destination_Currency[3] = '\0';
        tradeData[index].tradeAmount = atoi(fields[1]);
        tradeData[index].tradePrice = atof(fields[2]);
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
    FILE *csvFile = fopen("trades.txt", "r");
    if (!csvFile) {
        fprintf(stderr, "Could not open trade_data.csv: %s\n", strerror(errno));
        return 1;
    }

    ConvertDatafromCsvtoXML(csvFile);
    fclose(csvFile);
    return 0;
}
