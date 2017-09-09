#include "gtest/gtest.h"
#include "Resource/Lod.hpp"

class LodUnitTest : public ::testing::Test
{
public:
    HoMM3::Resource::Lod* filelod_;
    HoMM3::Resource::Lod* veclod_;

    virtual void SetUp()
    {
        std::ifstream ifs("txt_content.lod", std::ios::binary);
        ifs.seekg(0, std::ios::end);
        uint length = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        std::vector<byte> bytes(length);
        ifs.read(reinterpret_cast<char*>(&bytes[0]), length);
        
        this->filelod_ = new HoMM3::Resource::Lod("txt_content.lod");
        this->veclod_ = new HoMM3::Resource::Lod(bytes);
    }

    virtual void TearDown()
    {
        delete this->filelod_;
        delete this->veclod_;
    }
};

TEST_F(LodUnitTest, LodIsProperlyLoadedFromFile)
{
    ASSERT_EQ((uint) 0x444F4C, this->filelod_->Header().key);
    ASSERT_EQ((uint) 500, this->filelod_->Header().type);
    ASSERT_EQ((uint) 74, this->filelod_->Header().nb);
    ASSERT_EQ((uint) 74, this->filelod_->EntriesHeaders().size());

    ASSERT_STREQ("AdvEvent.txt", (const char*) this->filelod_->EntriesHeaders()[0].get()->name);
    ASSERT_EQ((uint) 2, this->filelod_->EntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1084872, this->filelod_->EntriesHeaders()[0].get()->offset);
    ASSERT_EQ((uint) 25344, this->filelod_->EntriesHeaders()[0].get()->size);
    ASSERT_EQ((uint) 8059, this->filelod_->EntriesHeaders()[0].get()->zsize);

    ASSERT_STREQ("XtraInfo.txt", (const char*) this->filelod_->EntriesHeaders()[73].get()->name);
    ASSERT_EQ((uint) 2, this->filelod_->EntriesHeaders()[73].get()->type);
    ASSERT_EQ((uint) 1193109, this->filelod_->EntriesHeaders()[73].get()->offset);
    ASSERT_EQ((uint) 1273, this->filelod_->EntriesHeaders()[73].get()->size);
    ASSERT_EQ((uint) 357, this->filelod_->EntriesHeaders()[73].get()->zsize);
}

TEST_F(LodUnitTest, LodIsProperlyLoadedFromVector)
{
    ASSERT_EQ((uint) 0x444F4C, this->veclod_->Header().key);
    ASSERT_EQ((uint) 500, this->veclod_->Header().type);
    ASSERT_EQ((uint) 74, this->veclod_->Header().nb);
    ASSERT_EQ((uint) 74, this->veclod_->EntriesHeaders().size());
    
    ASSERT_STREQ("AdvEvent.txt", (const char*) this->veclod_->EntriesHeaders()[0].get()->name);
    ASSERT_EQ((uint) 2, this->veclod_->EntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1084872, this->veclod_->EntriesHeaders()[0].get()->offset);
    ASSERT_EQ((uint) 25344, this->veclod_->EntriesHeaders()[0].get()->size);
    ASSERT_EQ((uint) 8059, this->veclod_->EntriesHeaders()[0].get()->zsize);
    
    ASSERT_STREQ("XtraInfo.txt", (const char*) this->veclod_->EntriesHeaders()[73].get()->name);
    ASSERT_EQ((uint) 2, this->veclod_->EntriesHeaders()[73].get()->type);
    ASSERT_EQ((uint) 1193109, this->veclod_->EntriesHeaders()[73].get()->offset);
    ASSERT_EQ((uint) 1273, this->veclod_->EntriesHeaders()[73].get()->size);
    ASSERT_EQ((uint) 357, this->veclod_->EntriesHeaders()[73].get()->zsize);
}
