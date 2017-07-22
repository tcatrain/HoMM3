#include <fstream>
#include <iostream>

#include "Compression/PackedRLEHelper.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Method used to unpack the chunk starting at a provided address</summary>
        /// <param name="chunk_addr">The address of the chunk to process</param>
        /// <returns>The vector containing chunk's bytes</returns>
        void PackedRLEHelper::UnpackChunk_(const byte* chunk_addr, std::vector<byte>& out_bytes)
        {
            uint rowlength = 0, length;
            
            this->nb_read_ = 0;
            do
            {
                length = this->UnpackNext_(chunk_addr);
                /// Appends the buffer to the end of chunk bytes
                out_bytes.insert(out_bytes.end(), this->buffer_, this->buffer_ + length);
                rowlength += length;
            } while(rowlength < this->unpked_minsize_);
        }
        
        /// <summary>Method used to unpack the next sequence for the current chunk</summary>
        /// <param name="chunk_addr">The address of the chunk to process</param>
        /// <returns>The length of the unpacked segment</returns>
        uint PackedRLEHelper::UnpackNext_(const byte* chunk_addr)
        {
            /// Reads the next byte from the sequence to read
            byte current = *(chunk_addr + this->nb_read_);
            /// The length of the sequence is defined in the last 5 bits of the byte
            uint length = (current & (uint) 0b11111) + 1;
            /// The key of the sequence is defined in the first 3 bits if the byte
            uint key = current >> 5;
            this->buffer_ = new byte[length];
            
            /// If the key is bin(111), the sequence must be read as is from the inpyt
            if (key == 0b111)
            {
                for (uint j = 0; j < length; ++j)
                {
                    /// Reads the next $length bytes as the output sequence
                    this->buffer_[j] = *(chunk_addr + this->nb_read_ + j);
                }
                this->nb_read_ += length;
            }
            else
            {
                /// If the key is not bin(111), then the sequence is a repetition of $length $key
                std::fill_n(this->buffer_, length, key);
            }
            this->nb_read_++;
            return length;
        }
        
        uint PackedRLEHelper::PackNext_(const byte* next_addr)
        {
            
            return 0;
        }
        
        /// <summary>Sets the size of a byte chunk</summary>
        /// <param name="cnk_size">The chunk size</param>
        void PackedRLEHelper::ChunkSize(uint cnk_size)
        {
            this->chunk_size_ = cnk_size;
        }
        
        /// <summary>Sets the number of chunk to read</summary>
        /// <param name="cnk_nb">The number of chunk</param>
        void PackedRLEHelper::UnpackedSize(uint unpked_size)
        {
            this->unpked_size_ = unpked_size;
        }
        
        /// <summary>Sets the minimal size of an unpacked chunk</summary>
        /// <param name="unpked_minsize">The minimal unpacked chunk size</param>
        void PackedRLEHelper::UnpackedChunkSize(uint unpked_minsize)
        {
            this->unpked_minsize_ = unpked_minsize;
        }
        
        /// <summary>Method used to deflate an inflated byte vector</summary>
        /// <param name="in_bytes">The input inflated byte vector</param>
        /// <returns>The output deflated byte vector</returns>
        std::vector<byte>& PackedRLEHelper::Deflate(const std::vector<byte> &in_bytes)
        {
            std::logic_error("Not yet implemented");
            return *(new std::vector<byte>(in_bytes));
        }
        
        /// <summary>Method used to inflate a deflated byte vector</summary>
        /// <param name="in_bytes">The input deflated byte vector</param>
        /// <returns>The output inflated byte vector</returns>
        std::vector<byte>& PackedRLEHelper::Inflate(const std::vector<byte> &in_bytes)
        {
            std::vector<byte>* out_bytes = new std::vector<byte>();
            for (uint i = 0; i < in_bytes.size(); ++i)
            {
                out_bytes->push_back(in_bytes[i]);
            }
            return *out_bytes;
        }
    }
}
