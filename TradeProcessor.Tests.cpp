#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include "CsvToXmlTradeConverter.h"

TEST_F(CsvToXmlTradeConverterTest, HandleMemoryAllocationFailure) {
    testing::internal::CaptureStderr();
    HandleMemoryAllocationFailure();
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Memory allocation failed.\n");
}
