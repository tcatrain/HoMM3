#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Resource/AResource.hpp"
#include "Types.hpp"
namespace HoMM3
{
    namespace Resource
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
            static uint const DEFH_PAL_SIZE = 256;
            
            /// Type of the DEF file
            uint type;
            /// Width of the final decoded image
            uint width;
            /// Height of the final decoded image
            uint height;
            /// Number of sequences in the DEF file
            uint nb;
            /// The RGB palette of the DEF sequence
            def_clridx palette[DEFH_PAL_SIZE];
        };
        #pragma pack(pop)
        
        #pragma pack(push, 1)
        /// <summary>
        /// 0x00 type of the sequence
        /// 0x04 length of the sequence
        /// 0x08 first unknown section
        /// 0x0C second unknown section
        /// </summary>
        struct def_seqh
        {
            /// Type of the sequence
            uint type;
            /// Number of frames in the sequence
            uint nb;
            /// First unknown memory segment
            uint unkwn1;
            /// Second unknown memory segment
            uint unkwn2;
        };
        #pragma pack(pop)
        
        /// <summary>Class Def</summary>
        class Def : public AResource<def_h, def_seqh>
        {
        private:
            /// <summary>Method used to dump the content of the Def object</summary>
    		/// <param name="os">The output stream where to write the dump</param>
            void Dump_(std::ostream&) const override;
            
        public:
            /// <summary>
            /// Constructor of the class HoMM3::Resource::Def. Reads the byte vector
            /// and parses bytes to locate the sequences .
            /// </summary>
            /// <param name="bytes">Byte vector containing the DEF file to read</param>
            Def(const std::string&);
            
            /// <summary>Destructor if the class HoMM3::Resource::Def</summary>
            ~Def();
            
            /// <summary>Method used to read an sequence from the DEF file</summary>
            /// <param name="seqh">The sequence header structure to read</param>
            /// <returns>The byte vector containing the sequence</returns>
            const std::vector<byte> ReadEntry(const def_seqh&) override;
        };
    }
}
