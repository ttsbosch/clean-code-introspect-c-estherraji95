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

