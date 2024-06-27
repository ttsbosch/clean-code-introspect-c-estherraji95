#include "TradeRecord.h"
#include "Converters.h"

void HandleMemoryAllocationFailure();
char** AllocateInitialMemory();
char** ReallocateMemory(char** lines, int* capacity);
char* CopyLine(const char* line);
bool CheckAndReallocateBuffer(char*** lines, int* count, int* capacity);
bool AddLineToBuffer(char*** lines, int* count, int* capacity, const char* line);
char** ReadTradeDataFromCsv(FILE* stream, int* numLines);
TradeRecord* AllocateTradeData(int numLines);
int ParseCsvLine(char* line, char* fields[], int maxFields);
void ParseCurrencies(const char* field, char* sourceCurrency, char* destinationCurrency);
void MapFieldsToTradeRecord(TradeRecord* record, char* fields[]);
TradeRecord* mapCsvLineDataToTradeData(char** lineInFile, int numLines);
void ValidateTradeRecord(const TradeRecord* record, int lineIndex);
void WriteXML(const TradeRecord* records, int numLines);
void ConvertDatafromCsvtoXML(FILE* stream);

