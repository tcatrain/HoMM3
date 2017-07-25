#include "gtest/gtest.h"
#include "Resource/Pcx.hpp"

class PcxUnitTest : public ::testing::Test
{
public:
    HoMM3::Resource::Pcx* pcx_;

    virtual void SetUp()
    {
        this->pcx_ = new HoMM3::Resource::Pcx("ab01_01.pcx");
    }

    virtual void TearDown()
    {
        delete this->pcx_;
    }
};

TEST_F(PcxUnitTest, PcxIsProperlyLoaded)
{
    ASSERT_EQ((uint) 1692, this->pcx_->Header().size);
    ASSERT_EQ((uint) 3, this->pcx_->Header().type);
    ASSERT_EQ((uint) 96, this->pcx_->Header().flwidth);
    ASSERT_EQ((uint) 64, this->pcx_->Header().flheight);
    ASSERT_EQ((uint) 96, this->pcx_->Header().fmwidth);
    ASSERT_EQ((uint) 55, this->pcx_->Header().fmheight);
    ASSERT_EQ((uint) 0, this->pcx_->Header().xmargin);
    ASSERT_EQ((uint) 5, this->pcx_->Header().ymargin);
}
