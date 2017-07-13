#pragma once
#include <streambuf>
#include <vector>
#include <zlib.h>
#include "Types.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Class ZHelper</summary>
        class HOMM3_LOD_LIB ZHelper
        {
        private:

        public:
            /// Size increase ratio of the buffer to inflate in
            static const float INCREASE_RATIO;
            
            /// <summary>Method used to inflate a deflated byte vector</summary>
            /// <param name="in_bytes">The input deflated byte vector</param>
            /// <returns>The output inflated byte vector</returns>
            static std::vector<byte> const Inflate(std::vector<byte> const&);

            /// <summary>Method used to deflate an inflated byte vector</summary>
            /// <param name="in_bytes">The input inflated byte vector</param>
            /// <returns>The output deflated byte vector</returns>
            static std::vector<byte> const Deflate(std::vector<byte> const&);
        };
    }
}
