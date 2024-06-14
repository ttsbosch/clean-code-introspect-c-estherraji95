#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <CsvToXmlTradeConverter.h>
#include <Converters.h>
#include <TradeRecord.h>

#define LOTSIZE 100
void readTradeDataFromCSV()
{
    while (fgets(line, sizeof(line), stream)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        mapCsvLineDatatoTradeData()
        ValidateTradedata()
        ExtractFields()
    }
}

void mapCsvLineDatatoTradeData()
{
    while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }
}
void ValidateTradedata()
{
    
 
        if (fieldCount != 3) {
            logtoFile(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }
 
        if (strlen(fields[0]) != 6) {
            logtoFile(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }
 
        int trade_amount;
        if (!TryConverttoInt(fields[1], &trade_amount)) {
            logtoFile(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }
 
        double trade_price;
        if (!TryConverttoDouble(fields[2], &trade_price)) {
            logtoFile(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }
    
 }
 
Void ExtractFields(){
        strncpy(Records[objectCount].SourceCurrency, fields[0], 3);
        strncpy(Records[objectCount].DestinationCurrency, fields[0] + 3, 3);
        Records[objectCount].Lots = trade_amount / LOTSIZE;
        Records[objectCount].Price = trade_price;
        objectCount++;
        lineCount++;
    }
 
 void WriteTradestoXML{
 
    FILE* outFile = fopen("output.xml", "w");
    logtoFile(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        logtoFile(outFile, "\t<TradeRecord>\n");
        logtoFile(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", Records[i].SourceCurrency);
        logtoFile(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", Records[i].DestinationCurrency);
        logtoFile(outFile, "\t\t<Lots>%d</Lots>\n", Records[i].Lots);
        logtoFile(outFile, "\t\t<Price>%f</Price>\n", Records[i].Price);
        logtoFile(outFile, "\t</TradeRecord>\n");
    }
    logtoFile(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}

void logtoFile(message){
    fprintf(message);
}

void ConvertFromCSVToXML(FILE* stream) {
    char line[1024];
    TradeRecords Records[1024];
    int lineCount = 0;
    int objectCount = 0;
    readTradeDataFromCSV()
    WriteTradestoXML()
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
