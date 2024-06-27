#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include "CsvToXmlTradeConverter.h" // Assuming this is the header file with your functions

class CsvToXmlTradeConverterTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(CsvToXmlTradeConverterTest, HandleMemoryAllocationFailure) {
    testing::internal::CaptureStderr();
    HandleMemoryAllocationFailure();
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Memory allocation failed.\n");
}

TEST_F(CsvToXmlTradeConverterTest, AllocateInitialMemory) {
    char** lines = AllocateInitialMemory();
    ASSERT_NE(lines, nullptr);
    free(lines);
}

TEST_F(CsvToXmlTradeConverterTest, ReallocateMemory) {
    char** lines = AllocateInitialMemory();
    int capacity = INITIAL_CAPACITY;
    lines = ReallocateMemory(lines, &capacity);
    ASSERT_NE(lines, nullptr);
    free(lines);
}

TEST_F(CsvToXmlTradeConverterTest, CopyLine) {
    const char* line = "test line";
    char* copiedLine = CopyLine(line);
    ASSERT_NE(copiedLine, nullptr);
    EXPECT_STREQ(copiedLine, line);
    free(copiedLine);
}

TEST_F(CsvToXmlTradeConverterTest, CheckAndReallocateBuffer) {
    char** lines = AllocateInitialMemory();
    int count = INITIAL_CAPACITY;
    int capacity = INITIAL_CAPACITY;
    ASSERT_TRUE(CheckAndReallocateBuffer(&lines, &count, &capacity));
    free(lines);
}
