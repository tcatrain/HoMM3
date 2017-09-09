#include "gtest/gtest.h"
#include "Resource/Pcx.hpp"

class PcxUnitTest : public ::testing::Test
{
public:
    HoMM3::Resource::Pcx* filepcx_;
    HoMM3::Resource::Pcx* vecpcx_;

    virtual void SetUp()
    {
        std::ifstream ifs("ab01_01.pcx", std::ios::binary);
        ifs.seekg(0, std::ios::end);
        uint length = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        std::vector<byte> bytes(length);
        ifs.read(reinterpret_cast<char*>(&bytes[0]), length);

        this->filepcx_ = new HoMM3::Resource::Pcx("ab01_01.pcx");
        this->vecpcx_ = new HoMM3::Resource::Pcx(bytes);
    }

    virtual void TearDown()
    {
        delete this->filepcx_;
        delete this->vecpcx_;
    }
};

TEST_F(PcxUnitTest, PcxIsProperlyLoadedFromFile)
{
    ASSERT_EQ((uint) 1692, this->filepcx_->Header().size);
    ASSERT_EQ((uint) 3, this->filepcx_->Header().type);
    ASSERT_EQ((uint) 96, this->filepcx_->Header().flwidth);
    ASSERT_EQ((uint) 64, this->filepcx_->Header().flheight);
    ASSERT_EQ((uint) 96, this->filepcx_->Header().fmwidth);
    ASSERT_EQ((uint) 55, this->filepcx_->Header().fmheight);
    ASSERT_EQ((uint) 0, this->filepcx_->Header().xmargin);
    ASSERT_EQ((uint) 5, this->filepcx_->Header().ymargin);
}

TEST_F(PcxUnitTest, PcxIsProperlyLoadedFromVector)
{
    ASSERT_EQ((uint) 1692, this->vecpcx_->Header().size);
    ASSERT_EQ((uint) 3, this->vecpcx_->Header().type);
    ASSERT_EQ((uint) 96, this->vecpcx_->Header().flwidth);
    ASSERT_EQ((uint) 64, this->vecpcx_->Header().flheight);
    ASSERT_EQ((uint) 96, this->vecpcx_->Header().fmwidth);
    ASSERT_EQ((uint) 55, this->vecpcx_->Header().fmheight);
    ASSERT_EQ((uint) 0, this->vecpcx_->Header().xmargin);
    ASSERT_EQ((uint) 5, this->vecpcx_->Header().ymargin);
}
