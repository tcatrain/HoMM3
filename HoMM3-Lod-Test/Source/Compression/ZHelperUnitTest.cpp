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

TEST_F(ZHelperUnitTest, StringIsCorrectlyCompressed)
{
    std::string str("00000000000000000000");
    std::vector<byte> raw(str.begin(), str.end());
    std::vector<byte> deflated = this->zcompressor.Deflate(raw);
    std::vector<byte> inflated = this->zcompressor.Inflate(deflated);
    ASSERT_EQ((uint) 20, str.size());
    ASSERT_EQ((uint) 20, raw.size());
    ASSERT_EQ((uint) 11, deflated.size());
    ASSERT_EQ((uint) 20, inflated.size());
}
