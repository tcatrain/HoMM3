#include "Compression/RLEHelper.hpp"

namespace HoMM3
{
    namespace Compression
    {
        /// <summary>Method used to inflate a deflated byte vector</summary>
        /// <param name="in_bytes">The input deflated byte vector</param>
        /// <returns>The output inflated byte vector</returns>
        const std::vector<byte> Inflate(const std::vector<byte> &in_bytes)
        {
            std::vector<byte> out_bytes;
            for (uint i = 0; i < in_bytes.size(); ++i)
            {
                out_bytes.push_back(in_bytes[i]);
            }
            return out_bytes;
        }
        
        /// <summary>Method used to deflate an inflated byte vector</summary>
        /// <param name="in_bytes">The input inflated byte vector</param>
        /// <returns>The output deflated byte vector</returns>
        const std::vector<byte> Deflate(const std::vector<byte> &in_bytes)
        {
            std::vector<byte> out_bytes;
            for (uint i = 0; i < in_bytes.size(); ++i)
            {
                out_bytes.push_back(in_bytes[i]);
            }
            return out_bytes;

        }
    }
}
