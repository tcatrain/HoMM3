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
        /// Structure LodHeader
        /// 0x00 key of the file
        /// 0x04 type of the file
        /// 0x08 number of file entries
        /// 0x0C unknown byte array
        /// </summary>
        struct LodHeader
        {
            /// Size of an unknown portion of the LOD header
            static int const LODH_UKWN_SIZE = 80;
        
            /// Key of the LOD file
            uint key;
            /// Type of the LOD file
            uint type;
            /// Number of included file
            uint nb;
            /// Unknown portion of the LOD file (useless ?)
            byte ukwn[LODH_UKWN_SIZE];
        };
        #pragma pack(pop)
        
        #pragma pack(push, 1)
        /// <summary>
        /// Structure LodEntryHeader
        /// 0x00 name of the entry
        /// 0x10 offset of the entry into the LOD file
        /// 0x14 size of the decompressed file
        /// 0x18 type of the file
        /// 0x1C size of the compressed file
        /// </summary>
        struct LodEntryHeader
        {
            /// Size of an entry name in a LOD file
            static int const LODEH_NAME_SIZE = 16;
        
            /// Name if the entry
            unsigned char name[LODEH_NAME_SIZE];
            /// Offset of the entry into the LOD file
            uint offset;
            /// Size of the entry when compressed
            uint size;
            /// Type of entry
            uint type;
            /// Size of then entry when uncompressed
            uint zsize;
        };
        #pragma pack(pop)

        /// <summary>Class Lod</summary>
        class Lod : public AResource<LodHeader, LodEntryHeader>
        {
    	private:
    		/// <summary>Method used to dump the content of the Def object</summary>
    		/// <param name="os">The output stream where to write the dump</param>
            virtual void Dump_(std::ostream&) const override;
            
        public:
            /// <summary>
            /// Constructor of the class HoMM3::Resource::Lod. Opens the input file stream
            /// and parses the file to locate content.
            /// </summary>
            /// <param name="path">Path of the LOD file to load</param>
            Lod(const std::string&);

            /// <summary>Destructor if the class HoMM3::Resource::Lod</summary>
            ~Lod();

            /// <summary>Method used to read an entry from the LO file</summary>
            /// <param name="eh">The entry header structure to read</param>
            /// <returns>The byte vector containing the entry</returns>
            const std::vector<byte> ReadEntry(const LodEntryHeader&);
        };
    }
}
