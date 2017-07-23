#include "gtest/gtest.h"
#include "Compression/PackedRLEHelper.hpp"

class PackedRLEHelperUnitTest : public ::testing::Test
{
public:
    HoMM3::Compression::PackedRLEHelper rlecompressor;
    
    virtual void SetUp()
    {
        this->rlecompressor.ChunkSize(32);
        this->rlecompressor.UnpackedSize(32);
        this->rlecompressor.UnpackedChunkSize(32);
    }
    
    virtual void TearDown()
    {
    }
};

TEST_F(PackedRLEHelperUnitTest, Sequence32Times0IsCorrectlyDeflated)
{
    std::vector<byte> raw;
    for (uint i = 0; i < 32; ++i)
    {
        raw.push_back(0);
    }
    std::vector<byte> deflated = this->rlecompressor.Deflate(raw);
    ASSERT_EQ((uint) 3, deflated.size());
    ASSERT_EQ((usint) 2, *reinterpret_cast<usint*>(deflated.data()));
    ASSERT_EQ((byte) 31, deflated[2]);
}

TEST_F(PackedRLEHelperUnitTest, Sequence64Times0IsCorrectlyDeflated)
{
    std::vector<byte> raw;
    for (uint i = 0; i < 64; ++i)
    {
        raw.push_back(0);
    }
    std::vector<byte> deflated = this->rlecompressor.Deflate(raw);
    ASSERT_EQ((uint) 6, deflated.size());
    ASSERT_EQ((usint) 4, *reinterpret_cast<usint*>(deflated.data()));
    ASSERT_EQ((usint) 5, *reinterpret_cast<usint*>(deflated.data() + sizeof(usint)));
    ASSERT_EQ((byte) 31, deflated[4]);
    ASSERT_EQ((byte) 31, deflated[5]);
}

TEST_F(PackedRLEHelperUnitTest, Sequence0To31IsCorrectlyDeflated)
{
    std::vector<byte> raw;
    for (uint i = 0; i < 32; ++i)
    {
        raw.push_back((byte) i);
    }
    std::vector<byte> deflated = this->rlecompressor.Deflate(raw);
    ASSERT_EQ((uint) 35, deflated.size());
    ASSERT_EQ((usint) 2, *reinterpret_cast<usint*>(deflated.data()));
    ASSERT_EQ(0b00000000, deflated[2]);
    ASSERT_EQ(0b00100000, deflated[3]);
    ASSERT_EQ(0b01000000, deflated[4]);
    ASSERT_EQ(0b01100000, deflated[5]);
    ASSERT_EQ(0b10000000, deflated[6]);
    ASSERT_EQ(0b10100000, deflated[7]);
    ASSERT_EQ(0b11000000, deflated[8]);
    ASSERT_EQ(0b11111000, deflated[9]);
    for (uint i = 10; i < 32; ++i)
    {
        ASSERT_EQ(i - 3, deflated[i]);
    }
}

TEST_F(PackedRLEHelperUnitTest, Sequence32Times0IsCorrectlyInflated)
{
    std::vector<byte> raw;
    raw.push_back(2);
    raw.push_back(0);
    raw.push_back(31);

    std::vector<byte> inflated = this->rlecompressor.Inflate(raw);
    ASSERT_EQ((uint) 32, inflated.size());
    for (byte i : inflated)
    {
        ASSERT_EQ(0, i);
    }
}

TEST_F(PackedRLEHelperUnitTest, Sequence0To31IsCorrectlyInflated)
{
    std::vector<byte> raw;
    raw.push_back(2);
    raw.push_back(0);

    for (uint i = 0; i < 7; ++i)
    {
        raw.push_back((byte) (i << 5));
    }
    raw.push_back((byte) 0b11111000);
    for (uint i = 7; i < 32; ++i)
    {
        raw.push_back(i);
    }
    std::vector<byte> inflated = this->rlecompressor.Inflate(raw);
    ASSERT_EQ((uint) 32, inflated.size());
    for (uint i = 0; i < 32; ++i)
    {
        ASSERT_EQ(i, inflated[i]);
    }
}