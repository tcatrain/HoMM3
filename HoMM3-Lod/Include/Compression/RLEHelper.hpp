#pragma once
#include <vector>
#include "Types.hpp"

namespace HoMM3
{
    namespace  Compression
    {
        class HOMM3_LOD_LIB RLEHelper
        {
        public:
            /// <summary>Method used to inflate a deflated byte vector</summary>
            /// <param name="in_bytes">The input deflated byte vector</param>
            /// <returns>The output inflated byte vector</returns>
            static const std::vector<byte> Inflate(const std::vector<byte>&);
            
            ///s <summary>Method used to deflate an inflated byte vector</summary>
            /// <param name="in_bytes">The input inflated byte vector</param>
            /// <returns>The output deflated byte vector</returns>
            static const std::vector<byte> Deflate(const std::vector<byte>&);
        };
    }
}
