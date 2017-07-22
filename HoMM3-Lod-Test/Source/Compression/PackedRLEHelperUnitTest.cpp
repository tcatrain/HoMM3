#include "gtest/gtest.h"
#include "Compression/PackedRLEHelper.hpp"

class PackedRLEHelperUnitTest : public ::testing::Test
{
public:
    HoMM3::Compression::PackedRLEHelper rlecompressor;
    
    virtual void SetUp()
    {
    }
    
    virtual void TearDown()
    {
    }
};

TEST_F(PackedRLEHelperUnitTest, StringIsCorrectlyCompressed)
{
    std::string str("00000000000000000000");
    std::vector<byte> raw(str.begin(), str.end());
    std::vector<byte> deflated = this->rlecompressor.Deflate(raw);
    std::vector<byte> inflated = this->rlecompressor.Inflate(deflated);
}
