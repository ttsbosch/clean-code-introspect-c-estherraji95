#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<CsvToXmlTradeConverter.h>

void ConvertFromCSVToXML(FILE* stream) {
    char line[1024];
    TradeRecords Records[1024];
    int lineCount = 0;
    int objectCount = 0;
 
    while (fgets(line, sizeof(line), stream)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }
 
        if (fieldCount != 3) {
            fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }
 
        if (strlen(fields[0]) != 6) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }
 
        int trade_amount;
        if (!TryConverttoInt(fields[1], &trade_amount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }
 
        double trade_price;
        if (!TryConverttoDouble(fields[2], &trade_price)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }
 
        strncpy(Records[objectCount].SourceCurrency, fields[0], 3);
        strncpy(Records[objectCount].DestibationCurrency, fields[0] + 3, 3);
        Records[objectCount].Lots = trade_amount / LotSize;
        Records[objectCount].Price = trade_price;
        objectCount++;
        lineCount++;
    }
 
    FILE* outFile = fopen("output.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", Records[i].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", Records[i].DestibationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", Records[i].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", Records[i].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}


int main() {
    std::ifstream inputFile("trades.txt");
    if (!inputFile) {
        std::cerr << "Error opening file\n";
        return 1;
    }
    ConvertFromCSVToXML(inputFile);
    return 0;
}
}
