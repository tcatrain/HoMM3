#pragma once

#include <vector>

#include "Compression/ACompressionHelper.hpp"
#include "Types.hpp"

namespace HoMM3
{
    namespace Compression
    {
        class HOMM3_LOD_LIB PackedRLEHelper : ACompressionHelper
        {
        private:
            /// The size of a byte chunk
            uint chunk_size_;
            /// The size of the unpacked data
            uint unpked_size_;
            /// The minimal size to consider a chunk to be unpacked
            uint unpked_minsize_;
            /// The number of byte read into the current chunk
            uint nb_read_;
            /// The buffer to store unpacked bytes
            byte* buffer_;
            
            /// <summary>Method used to unpack the chunk starting at a provided address</summary>
            /// <param name="chunk_addr">The address of the chunk to process</param>
            /// <returns>The vector containing chunk's bytes</returns>
            void UnpackChunk_(const byte*, std::vector<byte>&);
            
            /// <summary>Method used to unpack the next sequence for the current chunk</summary>
            /// <param name="chunk_addr">The address of the chunk to process</param>
            /// <returns>The length of the unpacked segment</returns>
            uint UnpackNext_(const byte*);
            
        public:
            /// <summary>Sets the size of a byte chunk</summary>
            /// <param name="cnk_size">The chunk size</param>
            void ChunkSize(uint);
            
            /// <summary>Sets the total size to unpack</summary>
            /// <param name="cnk_nb">The size to unpack</param>
            void UnpackedSize(uint);
            
            /// <summary>Sets the minimal size of an unpacked chunk</summary>
            /// <param name="unpked_minsize">The minimal unpacked chunk size</param>
            void UnpackedChunkSize(uint);
            
            /// <summary>Method used to inflate a deflated byte vector</summary>
            /// <param name="in_bytes">The input deflated byte vector</param>
            /// <returns>The output inflated byte vector</returns>
            std::vector<byte>& Inflate(const std::vector<byte>&) override;
            
            /// <summary>Method used to deflate an inflated byte vector</summary>
            /// <param name="in_bytes">The input inflated byte vector</param>
            /// <returns>The output deflated byte vector</returns>
            std::vector<byte>& Deflate(const std::vector<byte>&) override;
        };
    }
}
