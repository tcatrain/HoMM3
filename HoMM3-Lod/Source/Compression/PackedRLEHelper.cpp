#include <fstream>
#include <iostream>

#include "Compression/PackedRLEHelper.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Method used to unpack the chunk starting at a provided address</summary>
        /// <param name="chunk_addr">The address of the chunk to process</param>
        /// <param name="out_bytes">The output byte vector</param>
        void PackedRLEHelper::UnpackChunk_(const byte *chunk_addr, std::vector<byte>& out_bytes)
        {
            uint rowlength = 0, length;
            
            this->nb_read_ = 0;
            do
            {
                length = this->UnpackNext_(chunk_addr);
                /// Appends the buffer to the end of chunk bytes
                out_bytes.insert(out_bytes.end(), this->buffer_.begin(), this->buffer_.end());
                rowlength += length;
                this->buffer_.clear();
            } while(rowlength < this->unpked_minsize_);
        }
        
        /// <summary>Method used to unpack the next sequence for the current chunk</summary>
        /// <param name="chunk_addr">The address of the chunk to process</param>
        /// <returns>The length of the unpacked segment</returns>
        uint PackedRLEHelper::UnpackNext_(const byte *chunk_addr)
        {
            /// Reads the next byte from the sequence to read
            byte current = *(chunk_addr + this->nb_read_);
            /// The length of the sequence is defined in the last 5 bits of the byte
            uint length = (current & (uint) 0b11111) + 1;
            /// The key of the sequence is defined in the first 3 bits if the byte
            uint key = current >> 5;

            /// If the key is bin(111), the sequence must be read as is from the input
            if (key == 0b111)
            {
                for (uint i = 0; i < length; ++i)
                {
                    /// Reads the next $length bytes as the output sequence
                    this->buffer_.push_back(*(chunk_addr + this->nb_read_ + i + 1));
                }
                this->nb_read_ += length;
            }
            else
            {
                /// If the key is not bin(111), then the sequence is a repetition of $length $key
                for (uint i = 0; i < length; ++i)
                {
                    this->buffer_.push_back(key);
                }
            }
            this->nb_read_++;
            return (length);
        }

        /// <summary>Method used to pack the chunk starting at a provided address</summary>
        /// <param name="chunk_addr">The address of the chunk to process</param>
        /// <param name="out_bytes">The output byte vector</param>
        void PackedRLEHelper::PackChunk_(const byte* chunk_addr, std::vector<byte>& out_bytes)
        {
            byte current = *chunk_addr;

            /// Keep in mind the RLE byte actually count as 1 in the repetition count
            this->nb_read_ = RLE_NBREAD_WITH_KEY(current);
            for (uint i = 1; i < this->chunk_size_; ++i)
            {
                current = *(chunk_addr + i);
                /// Checks whether an RLE segment was started or not
                if (this->nb_read_ > 0)
                {
                    this->KeepOnRLE_(current, *(chunk_addr + i - 1), out_bytes);
                }
                else
                {
                    this->KeepOnNonRLE_(current, out_bytes);
                }
            }
            /// Write the last buffer or pack to the output
            this->FinalizePackedChunk_(current, out_bytes);
        }

        /// <summary>Method used to continue packing an RLE segment in the chunk</summary>
        /// <param name="current">The current byte in the chunk to process</param>
        /// <param name="previous">The previous byte in the chunk to process</param>
        /// <param name="out_bytes">The output byte vector</param>
        void PackedRLEHelper::KeepOnRLE_(const byte& current, const byte& previous, std::vector<byte>& out_bytes)
        {
            if (current == previous)
            {
                /// If the current byte is the same as the previous just increase the number of repetition
                ++this->nb_read_;
            }
            else
            {
                /// If not, it means the RLE is over for this segment
                out_bytes.push_back(RLE_ONE_BYTE(previous, this->nb_read_));
                /// Then reinit the number of repetitions
                this->nb_read_ = RLE_NBREAD_WITH_KEY(current);
                if (current >= 0b111)
                {
                    /// Adds current to the buffer if the key is greater than the max key
                    this->buffer_.push_back(current);
                }
            }
        }

        /// <summary>Method used to continue packing a non RLE segment in the chunk</summary>
        /// <param name="current">The current byte in the chunk to process</param>
        /// <param name="out_bytes">The output byte vector</param>
        void PackedRLEHelper::KeepOnNonRLE_(const byte& current, std::vector<byte>& out_bytes)
        {
            if (current >= 0b111 && this->buffer_.size() < this->chunk_size_)
            {
                /// If current is greater than the max key and buffer is not full, just adds current to the buffer
                this->buffer_.push_back(current);
            }
            else
            {
                /// If one of the previous conditions are not met, the non RLE is over for this segment
                /// First add the raw data indicator to the output bytes
                out_bytes.push_back(RLE_ONE_BYTE(0b111, this->buffer_.size()));
                /// Then copy the content of the buffer to the output bytes and clear the buffer
                out_bytes.insert(out_bytes.end(), this->buffer_.begin(), this->buffer_.end());
                this->buffer_.clear();
                /// Then reinit the number of repetitions
                this->nb_read_ = RLE_NBREAD_WITH_KEY(current);
                if (current >= 0b111)
                {
                    /// Adds current to the buffer if the key is greater than the max key
                    this->buffer_.push_back(current);
                }
            }
        }

        /// <summary>Method used to finalize the chunk packing</summary>
        /// <param name="current">The current byte in the chunk to process</param>
        /// <param name="out_bytes">The output byte vector</param>
        void PackedRLEHelper::FinalizePackedChunk_(const byte& current, std::vector<byte>& out_bytes)
        {
            if (this->nb_read_ > 0)
            {
                /// If RLE was started add current to output bytes
                out_bytes.push_back(RLE_ONE_BYTE(current, this->nb_read_));
            }
            else
            {
                /// If non RLE was started, first add the raw data indicator to the output bytes
                out_bytes.push_back(RLE_ONE_BYTE(7, this->buffer_.size()));
                /// Then copy the content of the buffer to the output bytes and clear the buffer
                out_bytes.insert(out_bytes.end(), this->buffer_.begin(), this->buffer_.end());
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
        std::vector<byte> PackedRLEHelper::Deflate(const std::vector<byte> &in_bytes)
        {
            this->unpked_size_ = (uint) in_bytes.size();
            uint chunk_nb = this->unpked_size_ / this->chunk_size_;
            uint offset;

            std::vector<byte> *out_bytes = new std::vector<byte>();
            for (uint i = 0; i < chunk_nb; ++i)
            {
                /// The chunk offset should be the total size of the headers + the current size of the output bytes - the size of already written headers
                offset = (uint) (out_bytes->size() +  chunk_nb * sizeof(usint) - i * sizeof(usint));
                /// Adds the chunk header to the out_bytes, just after previously added headers
                out_bytes->insert(out_bytes->begin() + i * sizeof(usint), reinterpret_cast<byte *>(&offset), reinterpret_cast<byte *>(&offset) + sizeof(usint));
                /// Then pack the next chunk
                this->PackChunk_(in_bytes.data() + i * this->chunk_size_, *out_bytes);
            }
            return (*out_bytes);
        }
        
        /// <summary>Method used to inflate a deflated byte vector</summary>
        /// <param name="in_bytes">The input deflated byte vector</param>
        /// <returns>The output inflated byte vector</returns>
        std::vector<byte> PackedRLEHelper::Inflate(const std::vector<byte>& in_bytes)
        {
            std::vector<byte> out_bytes;
            uint chunk_nb = this->unpked_size_ / this->chunk_size_;
            usint chunk_offset;

            for (uint i = 0; i < chunk_nb; ++i)
            {
                /// Reads the next 2 bytes to get the offset of the next chunk within the sequence
                chunk_offset = *reinterpret_cast<const usint *>(&in_bytes[i * sizeof(usint)]);
                /// Then position the cursor to the byte sequence starting at $offset
                this->UnpackChunk_(in_bytes.data() + chunk_offset, out_bytes);
            }
            return (out_bytes);
        }
    }
}
