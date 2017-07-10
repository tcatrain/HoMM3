#pragma once
#include <vector>
#include <memory>
#include "Types.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Class Def</summary>
        class Def
        {
            
            #pragma pack(push, 1)
            /// <summary>
            /// Structure def_clridx (def color index)
            /// 0x00 red index
            /// 0x01 green index
            /// 0x02 blue index
            /// </summary>
            struct def_clridx
            {
                byte r;
                byte g;
                byte b;
            };
            #pragma pack(pop)
            
            #pragma pack(push, 1)
            /// <summary>
            /// Structure def_h (def header)
            /// 0x00 type the file in :
            ///     0x40 spell animation
            ///     0x42 creature in combat
            ///     0x43 map object
            ///     0x44 hero on the map
            ///     0x45 terrain texture
            ///     0x46 cursor
            ///     0x47 town buildings/game interface btns
            ///     0x49 hero in combat
            /// 0x04 width of the final image
            /// 0x08 height of the final image
            /// 0x0C number of sequences
            /// 0x10 sequence's rgb palette
            /// </summary>
            struct def_h
            {
                /// Size of the rgb palette of the DEF header
                static int const DEFH_PAL_SIZE = 256;
                
                /// Type of the DEF file
                uint type;
                /// Width of the final decoded image
                uint width;
                /// Height of the final decoded image
                uint height;
                /// Number of sequences in the DEF file
                uint nb;
                /// The RGB palette of the DEF sequence
                def_clridx palt[DEFH_PAL_SIZE];
            };
            #pragma pack(pop)
            
            #pragma pack(push, 1)
            /// <summary>
            /// 0x00 type of the sequence
            /// 0x04 length of the sequence
            /// 0x08 first unknown section
            /// 0x0C second unknown section
            /// 0x10
            /// </summary>
            struct def_seqh
            {
                
            };
            #pragma pack(pop)
            
        private:
            def_h header_;
            std::vector<std::unique_ptr<def_seqh>> sequences_headers_;
        };
    }
}
