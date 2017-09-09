#include "gtest/gtest.h"
#include "Resource/Pcx.hpp"

class PcxUnitTest : public ::testing::Test
{
public:
    std::vector<byte> bytes_;
    
    virtual void SetUp()
    {
        std::ifstream ifs("ab01_01.pcx", std::ios::binary);
        ifs.seekg(0, std::ios::end);
        uint length = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        this->bytes_.resize(length);
        ifs.read(reinterpret_cast<char*>(&this->bytes_[0]), length);
    }

    virtual void TearDown()
    {
    }
};

TEST_F(PcxUnitTest, PcxIsProperlyLoadedFromFile)
{
    HoMM3::Resource::Pcx pcx("ab01_01.pcx");
    
    ASSERT_EQ(true, pcx.IsLoaded());
    ASSERT_EQ((uint) 1692, pcx.Header().size);
    ASSERT_EQ((uint) 3, pcx.Header().type);
    ASSERT_EQ((uint) 96, pcx.Header().flwidth);
    ASSERT_EQ((uint) 64, pcx.Header().flheight);
    ASSERT_EQ((uint) 96, pcx.Header().fmwidth);
    ASSERT_EQ((uint) 55, pcx.Header().fmheight);
    ASSERT_EQ((uint) 0, pcx.Header().xmargin);
    ASSERT_EQ((uint) 5, pcx.Header().ymargin);
}

TEST_F(PcxUnitTest, PcxIsNotLoadedFromMissingFile)
{
    HoMM3::Resource::Pcx pcx("missing.pcx");
    
    ASSERT_EQ(false, pcx.IsLoaded());
}


TEST_F(PcxUnitTest, PcxIsProperlyLoadedFromVector)
{
    HoMM3::Resource::Pcx pcx(this->bytes_);
    
    ASSERT_EQ(true, pcx.IsLoaded());
    ASSERT_EQ((uint) 1692, pcx.Header().size);
    ASSERT_EQ((uint) 3, pcx.Header().type);
    ASSERT_EQ((uint) 96, pcx.Header().flwidth);
    ASSERT_EQ((uint) 64, pcx.Header().flheight);
    ASSERT_EQ((uint) 96, pcx.Header().fmwidth);
    ASSERT_EQ((uint) 55, pcx.Header().fmheight);
    ASSERT_EQ((uint) 0, pcx.Header().xmargin);
    ASSERT_EQ((uint) 5, pcx.Header().ymargin);
}

TEST_F(PcxUnitTest, PcxIsNotLoadedFromEmptyVector)
{
    HoMM3::Resource::Pcx pcx((std::vector<byte>()));
    
    ASSERT_EQ(false, pcx.IsLoaded());
}
