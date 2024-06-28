
#include "gtest/gtest.h"
#include "Converters.h"
TEST(ConverttoInt, ValidInput) {
    int resultValue;
    const char* validInput = "123";
    ASSERT_EQ(ConverttoInt(validInput, &resultValue), 1);
    ASSERT_EQ(resultValue, 123);
}
 
TEST(ConverttoInt, InvalidInput) {
    int resultValue;
    const char* invalidInput = "abc";
    ASSERT_EQ(ConverttoInt(invalidInput, &resultValue), 0);
    // resultValue should remain unchanged if conversion fails
}
 
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
