#include "gtest/gtest.h"
#include "Resource/Def.hpp"

class DefUnitTest : public ::testing::Test
{
public:
    HoMM3::Resource::Def* def_;

    virtual void SetUp()
    {
        this->def_ = new HoMM3::Resource::Def("sprite.def");
        this->def_->Load();
    }

    virtual void TearDown()
    {
        delete this->def_;
    }
};

TEST_F(DefUnitTest, DefIsProperlyLoaded)
{
    ASSERT_EQ((uint) 68, this->def_->GetHeader().type);
    ASSERT_EQ((uint) 96, this->def_->GetHeader().width);
    ASSERT_EQ((uint) 64, this->def_->GetHeader().height);
    ASSERT_EQ((uint) 768, sizeof(this->def_->GetHeader().palette));
    ASSERT_EQ((uint) 10, this->def_->GetHeader().nb);
    ASSERT_EQ((uint) 10, this->def_->GetEntriesHeaders().size());

    ASSERT_EQ((uint) 0, this->def_->GetEntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1, this->def_->GetEntriesHeaders()[0].get()->nb);
    ASSERT_EQ((uint) 1, this->def_->GetEntriesHeaders()[0].get()->seq_frames.size());
    ASSERT_STREQ("ab01_37.pcx", (const char*) this->def_->GetEntriesHeaders()[0].get()->seq_frames[0].get()->name);
    ASSERT_EQ((uint) 71694, this->def_->GetEntriesHeaders()[0].get()->seq_frames[0].get()->offset);
}