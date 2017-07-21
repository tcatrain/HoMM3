#include "gtest/gtest.h"
#include "Resource/Lod.hpp"

class LodUnitTest : public ::testing::Test
{
public:
    HoMM3::Resource::Lod* lod_;

    virtual void SetUp()
    {
        this->lod_ = new HoMM3::Resource::Lod("txt_content.lod");
        this->lod_->Load();
    }

    virtual void TearDown()
    {
        delete this->lod_;
    }
};

TEST_F(LodUnitTest, LodIsProperlyLoaded)
{
    ASSERT_EQ((uint) 0x444F4C, this->lod_->GetHeader().key);
    ASSERT_EQ((uint) 500, this->lod_->GetHeader().type);
    ASSERT_EQ((uint) 74, this->lod_->GetHeader().nb);
    ASSERT_EQ((uint) 74, this->lod_->GetEntriesHeaders().size());

    ASSERT_STREQ("AdvEvent.txt", (const char*) this->lod_->GetEntriesHeaders()[0].get()->name);
    ASSERT_EQ((uint) 2, this->lod_->GetEntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1084872, this->lod_->GetEntriesHeaders()[0].get()->offset);
    ASSERT_EQ((uint) 25344, this->lod_->GetEntriesHeaders()[0].get()->size);
    ASSERT_EQ((uint) 8059, this->lod_->GetEntriesHeaders()[0].get()->zsize);

    ASSERT_STREQ("XtraInfo.txt", (const char*) this->lod_->GetEntriesHeaders()[73].get()->name);
    ASSERT_EQ((uint) 2, this->lod_->GetEntriesHeaders()[73].get()->type);
    ASSERT_EQ((uint) 1193109, this->lod_->GetEntriesHeaders()[73].get()->offset);
    ASSERT_EQ((uint) 1273, this->lod_->GetEntriesHeaders()[73].get()->size);
    ASSERT_EQ((uint) 357, this->lod_->GetEntriesHeaders()[73].get()->zsize);
}