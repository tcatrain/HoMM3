#pragma once

#include <vector>

#include "Types.hpp"

namespace HoMM3
{
    namespace Compression
    {
        class ACompressionHelper
        {
        public:
            /// <summary>Method used to inflate a deflated byte vector</summary>
            /// <param name="in_bytes">The input deflated byte vector</param>
            /// <returns>The output inflated byte vector</returns>
            virtual std::vector<byte> Inflate(const std::vector<byte>& in_bytes) = 0;
            
            /// <summary>Method used to deflate an inflated byte vector</summary>
            /// <param name="in_bytes">The input inflated byte vector</param>
            /// <returns>The output deflated byte vector</returns>
            virtual std::vector<byte> Deflate(const std::vector<byte>& in_bytes) = 0;
        };
    }
}
