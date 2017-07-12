#pragma once
#include "Types.hpp"

namespace HoMM3
{
    namespace Resource
    {
        #pragma pack(push, 1)
        struct PcxHeader {
            uint size;
            uint type;
            uint width;
            uint height;
            uint fmwidth;
            uint fmheight;
            uint xmargin;
            uint ymargin;
        };
        #pragma pack(pop)
    }
}
