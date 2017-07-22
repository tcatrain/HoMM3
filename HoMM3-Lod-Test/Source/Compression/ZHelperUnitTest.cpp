#include "gtest/gtest.h"
#include "Compression/ZHelper.hpp"

class ZHelperUnitTest : public ::testing::Test
{
public:
    HoMM3::Compression::ZHelper zcompressor;

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(ZHelperUnitTest, StringIsCorrectlyDeflated)
{
    std::string str("00000000000000000000");
    std::vector<byte> raw(str.begin(), str.end());
    std::vector<byte> deflated = this->zcompressor.Deflate(raw);
    
    ASSERT_EQ((uint) 20, str.size());
    ASSERT_EQ((uint) 20, raw.size());
    ASSERT_EQ((uint) 11, deflated.size());
}

TEST_F(ZHelperUnitTest, StringIsCorrectlyInflated)
{
    byte arr[11] = {120, 156, 51, 48, 192, 4, 0, 39, 116, 3, 193};
    std::string str(arr, arr + sizeof(arr));
    std::vector<byte> raw(str.begin(), str.end());
    std::vector<byte> inflated = this->zcompressor.Inflate(raw);
    
    ASSERT_EQ((uint) 11, str.size());
    ASSERT_EQ((uint) 11, raw.size());
    ASSERT_EQ((uint) 20, inflated.size());
}


