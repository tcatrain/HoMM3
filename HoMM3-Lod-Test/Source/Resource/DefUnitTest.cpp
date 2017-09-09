#include "gtest/gtest.h"
#include "Resource/Def.hpp"
#include <fstream>
#include <iostream>

class DefUnitTest : public ::testing::Test
{
public:
    std::vector<byte> bytes_;

    virtual void SetUp()
    {
        std::ifstream ifs("sprite.def", std::ios::binary);
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

TEST_F(DefUnitTest, DefIsProperlyLoadedFromFile)
{
    HoMM3::Resource::Def def("sprite.def");
    
    ASSERT_EQ(true, def.IsLoaded());
    ASSERT_EQ((uint) 68, def.Header().type);
    ASSERT_EQ((uint) 96, def.Header().width);
    ASSERT_EQ((uint) 64, def.Header().height);
    ASSERT_EQ((uint) 768, sizeof(def.Header().palette));
    ASSERT_EQ((uint) 10, def.Header().nb);
    ASSERT_EQ((uint) 10, def.EntriesHeaders().size());

    ASSERT_EQ((uint) 0, def.EntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1, def.EntriesHeaders()[0].get()->nb);
    ASSERT_EQ((uint) 1, def.EntriesHeaders()[0].get()->seq_frames.size());
    ASSERT_STREQ("ab01_37.pcx", (const char*) def.EntriesHeaders()[0].get()->seq_frames[0].get()->name);
    ASSERT_EQ((uint) 71694, def.EntriesHeaders()[0].get()->seq_frames[0].get()->offset);
}

TEST_F(DefUnitTest, DefIsNotLoadedFromMissingFile)
{
    HoMM3::Resource::Def def("missing.def");
    
    ASSERT_EQ(false, def.IsLoaded());
}

TEST_F(DefUnitTest, DefIsProperlyLoadedFromVector)
{
    HoMM3::Resource::Def def(this->bytes_);
    
    ASSERT_EQ(true, def.IsLoaded());
    ASSERT_EQ((uint) 68, def.Header().type);
    ASSERT_EQ((uint) 96, def.Header().width);
    ASSERT_EQ((uint) 64, def.Header().height);
    ASSERT_EQ((uint) 768, sizeof(def.Header().palette));
    ASSERT_EQ((uint) 10, def.Header().nb);
    ASSERT_EQ((uint) 10, def.EntriesHeaders().size());
    
    ASSERT_EQ((uint) 0, def.EntriesHeaders()[0].get()->type);
    ASSERT_EQ((uint) 1, def.EntriesHeaders()[0].get()->nb);
    ASSERT_EQ((uint) 1, def.EntriesHeaders()[0].get()->seq_frames.size());
    ASSERT_STREQ("ab01_37.pcx", (const char*) def.EntriesHeaders()[0].get()->seq_frames[0].get()->name);
    ASSERT_EQ((uint) 71694, def.EntriesHeaders()[0].get()->seq_frames[0].get()->offset);
}

TEST_F(DefUnitTest, DefIsNotLoadedFromEmptyVector)
{
    HoMM3::Resource::Def def((std::vector<byte>()));
    
    ASSERT_EQ(false, def.IsLoaded());
}
