#pragma once
#include <vector>
#include "Compression/ACompressionHelper.hpp"
#include "Types.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Class ZHelper</summary>
        class HOMM3_LOD_LIB ZHelper : ACompressionHelper
        {
        public:
            /// Size increase ratio of the buffer to inflate in
            static const float INCREASE_RATIO;
            
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
