#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "TradeRecord.h"
#include "CsvToXmlTradeConverter.h"

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
