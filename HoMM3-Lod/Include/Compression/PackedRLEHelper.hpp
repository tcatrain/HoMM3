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
            
            const std::vector<byte> DoChunk_(const std::vector<byte>&, usint);
            byte* DoRLEUnpacking_(const std::vector<byte>& in_bytes, uint lnoffset, uint& offset, byte current);
        public:
            /// <summary>Sets the size of a byte chunk</summary>
            /// <param name="cnk_size">The chunk size</param>
            void SetChunkSize(uint);
            
            /// <summary>Sets the total size to unpack</summary>
            /// <param name="cnk_nb">The size to unpack</param>
            void SetUnpackedSize(uint);
            
            /// <summary>Sets the minimal size of an unpacked chunk</summary>
            /// <param name="unpked_minsize">The minimal unpacked chunk size</param>
            void SetUnpackedChunkSize(uint);
            
            /// <summary>Method used to inflate a deflated byte vector</summary>
            /// <param name="in_bytes">The input deflated byte vector</param>
            /// <returns>The output inflated byte vector</returns>
            const std::vector<byte> Inflate(const std::vector<byte>&) override;
            
            /// <summary>Method used to deflate an inflated byte vector</summary>
            /// <param name="in_bytes">The input inflated byte vector</param>
            /// <returns>The output deflated byte vector</returns>
            const std::vector<byte> Deflate(const std::vector<byte>&) override;
        };
    }
}
