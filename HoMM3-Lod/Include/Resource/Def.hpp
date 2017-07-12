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
            /// <summary>
            /// 0x00 name of the sequence entry
            /// 0x0C offset of the sequence entry
            /// </summary>
            struct def_seqeh
            {
                /// Size of the name of a sequence entry
                static uint const DEF_SEQEH_NAME_SIZE = 13;
            
                byte name[DEF_SEQEH_NAME_SIZE];
                uint offset;
            };
            
            /// Size of sequence section directly mappable from the resource
            static uint const DEF_SEQH_FIXED_SIZE = sizeof(uint) * 4;
            
            /// Type of the sequence
            uint type;
            /// Number of frames in the sequence
            uint nb;
            /// First unknown memory segment
            uint unkwn1;
            /// Second unknown memory segment
            uint unkwn2;
            /// The list of entries headers in the sequence
            std::vector<std::unique_ptr<def_seqeh>> seq_entries_headers;
        };
        #pragma pack(pop)
        
        /// <summary>Class Def</summary>
        class Def : public AResource<def_h, def_seqh>
        {
        private:
            /// <summary>Method used to dump the content of the Def object</summary>
    		/// <param name="os">The output stream where to write the dump</param>
            virtual void Dump_(std::ostream&) const override;
            
            /// <summary>Method used to load the entries headers of the DEF file</summary>
            virtual void LoadEntriesHeaders_() override;
            
            /// <summary>Reads the next frame name for the sequence</summary>
            /// <param name="sequence_entry_header">Current sequence entry header to get the name for</param>
            /// <param name="current">Current iteration of name extraction</param>
            /// <param name="outof">Total expected iteration count</param>
            void ReadNextName_(def_seqh::def_seqeh&, uint, uint);
            
            /// <summary>Reads the next frame offset for the sequence</summary>
            /// <param name="sequence_entry_header">Current sequence entry header to get the offset for</param>
            /// <param name="current">Current iteration of offset extraction</param>
            /// <param name="outof">Total expected iteration count</param>
            void ReadNextOffset_(def_seqh::def_seqeh&, uint, uint);
            
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
