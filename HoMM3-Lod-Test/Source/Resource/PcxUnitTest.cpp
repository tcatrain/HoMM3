#include "gtest/gtest.h"
#include "Resource/Pcx.hpp"

class PcxUnitTest : public ::testing::Test
{
public:
    HoMM3::Resource::Pcx* pcx_;

    virtual void SetUp()
    {
        this->pcx_ = new HoMM3::Resource::Pcx("ab01_01.pcx");
        this->pcx_->Load();
    }

    virtual void TearDown()
    {
        delete this->pcx_;
    }
};

TEST_F(PcxUnitTest, PcxIsProperlyLoaded)
{
    ASSERT_EQ((uint) 1692, this->pcx_->GetHeader().size);
    ASSERT_EQ((uint) 3, this->pcx_->GetHeader().type);
    ASSERT_EQ((uint) 96, this->pcx_->GetHeader().flwidth);
    ASSERT_EQ((uint) 64, this->pcx_->GetHeader().flheight);
    ASSERT_EQ((uint) 96, this->pcx_->GetHeader().fmwidth);
    ASSERT_EQ((uint) 55, this->pcx_->GetHeader().fmheight);
    ASSERT_EQ((uint) 0, this->pcx_->GetHeader().xmargin);
    ASSERT_EQ((uint) 5, this->pcx_->GetHeader().ymargin);
}