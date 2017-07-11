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
		/// Structure lod_h (lod header)
		/// 0x00 key of the file
		/// 0x04 type of the file
		/// 0x08 number of file entries
		/// 0x0C unknown byte array
		/// </summary>
		struct lod_h
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
		/// Structure lod_eh (lod entry header)
		/// 0x00 name of the entry
		/// 0x10 offset of the entry into the LOD file
		/// 0x14 size of the decompressed file
		/// 0x18 type of the file
		/// 0x1C size of the compressed file
		/// </summary>
		struct lod_eh
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
        class Lod : public AResource<lod_h, lod_eh>
        {
            
        private:

        public:
            /// <summary>
            /// Constructor of the class HoMM3::Resource::Lod. Opens the input file stream
            /// and parses the file to locate content.
            /// </summary>
            /// <param name="path">Path of the LOD file to load</param>
            Lod(const std::string&);

            /// <summary>
            /// Destructor if the class HoMM3::Resource::Lod.
            /// </summary>
            ~Lod();

			/// <summary>Method used to read an entry from the LO file</summary>
            /// <param name="eh">The entry header structure to read</param>
            /// <returns>The byte vector containing the entry</returns>
            std::vector<byte> const ReadEntry(const lod_eh&) override;

            /// Declaring friend operator <<
            friend std::ostream& operator <<(std::ostream&, const Lod&);
        };

        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        /// <param name="file">LOD file object by reference</param>
        /// <returns>The outstream given in input</returns>
        std::ostream& operator <<(std::ostream&, const Lod &);
    }
}
