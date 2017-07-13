#pragma once
#include "Resource/AResource.hpp"
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
        
        class HOMM3_LOD_LIB Pcx : public AResource<PcxHeader, void*>
        {
        private:
            /// <summary>Method used to dump the content of the Pcx object</summary>
            /// <param name="os">The output stream where to write the dump</param>
            virtual void Dump_(std::ostream&) const override;
            
            /// <summary>Method used to load the entries headers of the PCX file</summary>
            virtual void LoadEntriesHeaders_() override;
            
        public:
            /// <summary>
            /// Constructor of the class HoMM3::Resource::Pcx. Opens the input file stream
            /// and parses the file to locate content.
            /// </summary>
            /// <param name="path">Path of the LOD file to load</param>
            Pcx(const std::string&);
            
            /// <summary>Method used to read an entry from the PCX file</summary>
            /// <param name="eh">The frame header structure to read</param>
            /// <returns>The byte vector containing the frame</returns>
            const std::vector<byte> ReadFrame(const PcxHeader&);
        };
        
    }
}
