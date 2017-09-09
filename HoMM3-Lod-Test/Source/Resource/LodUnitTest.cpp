#include "gtest/gtest.h"
#include "Resource/Lod.hpp"

class LodUnitTest : public ::testing::Test
{
public:
    std::vector<byte> bytes_;

    virtual void SetUp()
    {
        std::ifstream ifs("txt_content.lod", std::ios::binary);
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

TEST_F(LodUnitTest, LodIsProperlyLoadedFromFile)
{
    HoMM3::Resource::Lod lod("txt_content.lod");
    
    ASSERT_EQ(true, lod.IsLoaded());
    ASSERT_EQ((uint) 0x444F4C, lod.Header().key);
    ASSERT_EQ((uint) 500, lod.Header().type);
    ASSERT_EQ((uint) 74, lod.Header().nb);
    ASSERT_EQ((uint) 74, lod.EntriesHeaders().size());

    ASSERT_STREQ("AdvEvent.txt", (const char*) lod.EntriesHeaders()[0].get()->name);
    ASSERT_EQ((uint) 2, lod.EntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1084872, lod.EntriesHeaders()[0].get()->offset);
    ASSERT_EQ((uint) 25344, lod.EntriesHeaders()[0].get()->size);
    ASSERT_EQ((uint) 8059, lod.EntriesHeaders()[0].get()->zsize);

    ASSERT_STREQ("XtraInfo.txt", (const char*) lod.EntriesHeaders()[73].get()->name);
    ASSERT_EQ((uint) 2, lod.EntriesHeaders()[73].get()->type);
    ASSERT_EQ((uint) 1193109, lod.EntriesHeaders()[73].get()->offset);
    ASSERT_EQ((uint) 1273, lod.EntriesHeaders()[73].get()->size);
    ASSERT_EQ((uint) 357, lod.EntriesHeaders()[73].get()->zsize);
}

TEST_F(LodUnitTest, LodIsNotLoadedFromMissingFile)
{
    HoMM3::Resource::Lod lod("missing.lod");
    
    ASSERT_EQ(false, lod.IsLoaded());
}
    
TEST_F(LodUnitTest, LodIsProperlyLoadedFromVector)
{
    HoMM3::Resource::Lod lod(this->bytes_);
    
    ASSERT_EQ(true, lod.IsLoaded());
    ASSERT_EQ((uint) 0x444F4C, lod.Header().key);
    ASSERT_EQ((uint) 500, lod.Header().type);
    ASSERT_EQ((uint) 74, lod.Header().nb);
    ASSERT_EQ((uint) 74, lod.EntriesHeaders().size());
    
    ASSERT_STREQ("AdvEvent.txt", (const char*) lod.EntriesHeaders()[0].get()->name);
    ASSERT_EQ((uint) 2, lod.EntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1084872, lod.EntriesHeaders()[0].get()->offset);
    ASSERT_EQ((uint) 25344, lod.EntriesHeaders()[0].get()->size);
    ASSERT_EQ((uint) 8059, lod.EntriesHeaders()[0].get()->zsize);
    
    ASSERT_STREQ("XtraInfo.txt", (const char*) lod.EntriesHeaders()[73].get()->name);
    ASSERT_EQ((uint) 2, lod.EntriesHeaders()[73].get()->type);
    ASSERT_EQ((uint) 1193109, lod.EntriesHeaders()[73].get()->offset);
    ASSERT_EQ((uint) 1273, lod.EntriesHeaders()[73].get()->size);
    ASSERT_EQ((uint) 357, lod.EntriesHeaders()[73].get()->zsize);
}

TEST_F(LodUnitTest, LodIsNotLoadedFromEmptyVector)
{
    HoMM3::Resource::Lod lod((std::vector<byte>()));
    
    ASSERT_EQ(false, lod.IsLoaded());
}
