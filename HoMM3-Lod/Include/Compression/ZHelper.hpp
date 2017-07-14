#pragma once
#include <vector>
#include "Types.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Class ZHelper</summary>
        class HOMM3_LOD_LIB ZHelper
        {
        public:
            /// Size increase ratio of the buffer to inflate in
            static const float INCREASE_RATIO;
            
            /// <summary>Method used to inflate a deflated byte vector</summary>
            /// <param name="in_bytes">The input deflated byte vector</param>
            /// <returns>The output inflated byte vector</returns>
            static const std::vector<byte> Inflate(const std::vector<byte>&);

            /// <summary>Method used to deflate an inflated byte vector</summary>
            /// <param name="in_bytes">The input inflated byte vector</param>
            /// <returns>The output deflated byte vector</returns>
            static const std::vector<byte> Deflate(const std::vector<byte>&);
        };
    }
}
