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
            
            /// If the key is bin(111), the sequence must be read as is from the input
            if (key == 0b111)
            {
                for (uint j = 0; j < length; ++j)
                {
                    /// Reads the next $length bytes as the output sequence
                    this->buffer_[j] = *(chunk_addr + this->nb_read_ + j + 1);
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

        /// <summary>Method used to pack the chunk starting at a provided address</summary>
        /// <param name="chunk_addr">The address of the chunk to process</param>
        void PackedRLEHelper::PackChunk_(const byte* chunk_addr, std::vector<byte>& out_bytes)
        {
            byte current, previous = *chunk_addr;
            std::vector<byte> buffer;

            this->nb_read_ = previous < 7 ? 1 : 0;
            for (uint i = 1; i < this->chunk_size_; ++i)
            {
                current = *(chunk_addr + i);
                if (this->nb_read_ > 0)
                {
                    if (current == previous)
                    {
                        ++this->nb_read_;
                    }
                    else
                    {
                        out_bytes.push_back((previous << 5) | (this->nb_read_ - 1));
                        if (current < 7)
                        {
                            this->nb_read_ = 1;
                        }
                        else
                        {
                            this->nb_read_ = 0;
                            buffer.clear();
                            buffer.push_back(current);
                        }
                        previous = current;
                    }
                }
                else
                {
                    if (current < 7 || buffer.size() > 31)
                    {
                        out_bytes.push_back((7 << 5) | (buffer.size() - 1));
                        out_bytes.insert(out_bytes.end(), buffer.begin(), buffer.end());
                        if (current < 7)
                        {
                            this->nb_read_ = 1;
                        }
                        else
                        {
                            this->nb_read_ = 0;
                            buffer.clear();
                            buffer.push_back(current);
                        }
                        previous = current;
                    }
                    else
                    {
                        buffer.push_back(current);
                    }
                }
            }
            if (this->nb_read_ > 0)
            {
                out_bytes.push_back((current << 5) | (this->nb_read_ - 1));
            }
            else
            {
                out_bytes.push_back((7 << 5) | (buffer.size() - 1));
                out_bytes.insert(out_bytes.end(), buffer.begin(), buffer.end());
            }
        }
        
        /// <summary>Sets the size of a byte chunk</summary>
        /// <param name="cnk_size">The chunk size</param>
        void PackedRLEHelper::ChunkSize(uint cnk_size)
        {
            this->chunk_size_ = cnk_size;
        }
        
        /// <summary>Sets the number of chunk to read</summary>
        /// <param name="unpked_size">The size of unpacked datas</param>
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
            this->unpked_size_ = (uint) in_bytes.size();
            uint chunk_nb = this->unpked_size_ / this->chunk_size_;
            uint offset;

            std::vector<byte>* out_bytes = new std::vector<byte>();
            for (uint i = 0; i < chunk_nb; ++i)
            {
                offset = (uint) out_bytes->size() +  chunk_nb * sizeof(usint) - i * sizeof(usint);
                out_bytes->insert(out_bytes->begin() + i * sizeof(usint), reinterpret_cast<byte*>(&offset), reinterpret_cast<byte*>(&offset) + sizeof(usint));
                this->PackChunk_(in_bytes.data() + i * this->chunk_size_, *out_bytes);
            }
            return *out_bytes;
        }
        
        /// <summary>Method used to inflate a deflated byte vector</summary>
        /// <param name="in_bytes">The input deflated byte vector</param>
        /// <returns>The output inflated byte vector</returns>
        std::vector<byte>& PackedRLEHelper::Inflate(const std::vector<byte>& in_bytes)
        {
            std::vector<byte>* out_bytes = new std::vector<byte>();
            uint chunk_nb = this->unpked_size_ / this->chunk_size_;
            usint chunk_offset;

            for (uint i = 0; i < chunk_nb; ++i)
            {
                /// Reads the next 2 bytes to get the offset of the next chunk within the sequence
                chunk_offset = *reinterpret_cast<const usint*>(&in_bytes[i * sizeof(usint)]);
                /// Then position the cursor to the byte sequence starting at $offset
                this->UnpackChunk_(in_bytes.data() + chunk_offset, *out_bytes);
            }
            return *out_bytes;
        }
    }
}
