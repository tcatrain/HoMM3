#pragma once

#include "Resource/AResource.hpp"
#include "Compression/PackedRLEHelper.hpp"
#include "Types.hpp"

namespace HoMM3
{
    namespace Resource
    {
        #pragma pack(push, 1)
        /// <summary>
        /// Structure PcxHeader
        /// 0x00 size of the PCX frame
        /// 0x04 type of encoding of the PCX frame
        ///     0x01 ??
        ///     0x02 ??
        ///     0x03 PackedRLE encoding
        /// 0x08 full width of the sequence
        /// 0x0C full height of the sequence
        /// 0x10 width of the PCX frame
        /// 0x14 height of the PCX frame
        /// 0x18 x margin of the PCX frame
        /// 0x1C y margin of the PCX frame
        /// </summary>
        struct PcxHeader {
            /// The size of the frame
            uint size;
            /// The encoding type of the frame
            uint type;
            /// The width of the full sequence
            uint flwidth;
            /// The height of the full sequence
            uint flheight;
            /// The width of the frame
            uint fmwidth;
            /// The height of the frame
            uint fmheight;
            /// The x margin of the frame
            uint xmargin;
            /// The y margin of the frame
            uint ymargin;
        };
        #pragma pack(pop)
        
        class HOMM3_LOD_LIB Pcx : public AResource<PcxHeader, void*>
        {
        private:
            /// The RLE compressor used to extract bitmaps from the PCX
            Compression::PackedRLEHelper rlecompressor_;
            
            /// <summary>Method used to dump the content of the Pcx object</summary>
            /// <param name="os">The output stream where to write the dump</param>
            virtual void Dump_(std::ostream&) const override;
            
            /// <summary>Method used to load the entries headers of the PCX file</summary>
            virtual void LoadHeader_() override;
            
            /// <summary>Method used to load the entries headers of the PCX file</summary>
            virtual void LoadEntriesHeaders_() override;
            
            /// <summary>Method used to mould the PCX entry file to full dimensions</summary>
            /// <param name="buffer">The buffer to work on containing the extracted PCX entry file</param>
            std::vector<byte>& Mould_(std::vector<byte>&);
            
        public:
            /// <summary>
            /// Constructor of the class HoMM3::Resource::Pcx. Opens the input file stream
            /// and parses the file to locate content.
            /// </summary>
            /// <param name="path">Path of the LOD file to load</param>
            Pcx(const std::string&);
            
            /// <summary>Method used to read an entry from the PCX file</summary>
            /// <returns>The byte vector containing the bitmap frame</returns>
            std::vector<byte>& ReadFrame();
        };
        
    }
}
