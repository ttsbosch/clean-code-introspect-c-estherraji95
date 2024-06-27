#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include "CsvToXmlTradeConverter.h"

// Define your fixture class
class CsvToXmlTradeConverterTest : public ::testing::Test {
protected:
    // Optional: SetUp will be called before each test
    void SetUp() override {
        // Initialize resources if needed
    }

    // Optional: TearDown will be called after each test
    void TearDown() override {
        // Clean up resources if needed
    }
};

TEST_F(CsvToXmlTradeConverterTest, HandleMemoryAllocationFailure) {
    testing::internal::CaptureStderr();
    HandleMemoryAllocationFailure();
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Memory allocation failed.\n");
}
