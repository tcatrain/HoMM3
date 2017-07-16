#include <fstream>
#include <iostream>
#include "Compression/PackedRLEHelper.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Sets the size of a byte chunk</summary>
        /// <param name="cnk_size">The chunk size</param>
        void PackedRLEHelper::SetChunkSize(uint cnk_size)
        {
            this->chunk_size_ = cnk_size;
        }
        
        /// <summary>Sets the number of chunk to read</summary>
        /// <param name="cnk_nb">The number of chunk</param>
        void PackedRLEHelper::SetUnpackedSize(uint unpked_size)
        {
            this->unpked_size_ = unpked_size;
        }
        
        /// <summary>Sets the minimal size of an unpacked chunk</summary>
        /// <param name="unpked_minsize">The minimal unpacked chunk size</param>
        void PackedRLEHelper::SetUnpackedChunkSize(uint unpked_minsize)
        {
            this->unpked_minsize_ = unpked_minsize;
        }
        
        /// <summary>Method used to inflate a deflated byte vector</summary>
        /// <param name="in_bytes">The input deflated byte vector</param>
        /// <returns>The output inflated byte vector</returns>
        const std::vector<byte> PackedRLEHelper::Inflate(const std::vector<byte> &in_bytes)
        {
            std::vector<byte> out_bytes;
            for (uint i = 0; i < in_bytes.size(); ++i)
            {
                out_bytes.push_back(in_bytes[i]);
            }
            return out_bytes;
        }
        
        /// <summary>Method used to deflate an inflated byte vector</summary>
        /// <param name="in_bytes">The input inflated byte vector</param>
        /// <returns>The output deflated byte vector</returns>
        const std::vector<byte> PackedRLEHelper::Deflate(const std::vector<byte> &in_bytes)
        {
            std::vector<byte> out_bytes, chunk_bytes;
            
            for (uint i = 0; i < this->unpked_size_ / this->chunk_size_; ++i)
            {
                chunk_bytes = this->DoChunk_(in_bytes, *reinterpret_cast<const usint*>(&in_bytes[i * sizeof(usint) + 32]));
                out_bytes.insert(out_bytes.cend(), chunk_bytes.begin(), chunk_bytes.end());
                
            }
            return out_bytes;
        }
        
        const std::vector<byte> PackedRLEHelper::DoChunk_(const std::vector<byte>& in_bytes, usint lnoffset)
        {
            std::vector<byte> chunk_bytes;
            uint rowlength = 0, offset = 0;
            byte *buf;
            do
            {
                buf = this->DoRLEUnpacking_(in_bytes, lnoffset, offset, *reinterpret_cast<const byte*>(&in_bytes[lnoffset + 32 + offset]));
                chunk_bytes.insert(chunk_bytes.cend(), buf, buf + sizeof(buf));
                rowlength += sizeof(buf);
                ++offset;
            } while(rowlength < 32);
            return chunk_bytes;
        }
        
        byte* PackedRLEHelper::DoRLEUnpacking_(const std::vector<byte>& in_bytes, uint lnoffset, uint& offset, byte current)
        {
            uint length = (current & 0x1F) + 1;
            byte* buffer = new byte[length];
            
            if ((current & 0xE0) == 0xE0)
            {
                for (uint j = 0; j < length; ++j)
                {
                    buffer[j] = *reinterpret_cast<const byte*>(&in_bytes[lnoffset + 32 + ++offset]);
                }
            }
            else
            {
                std::fill_n(buffer, length, current & 0xE0);
            }
            return buffer;
        }
    }
}
